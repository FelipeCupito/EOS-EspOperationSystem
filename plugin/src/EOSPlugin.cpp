// plugin/src/EOSPlugin.cpp
// — Detecta clases eos::peripheral y genera un JSON incluyendo body limpio sin etiquetas DSL —

#include "clang/AST/AST.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/Frontend/FrontendPluginRegistry.h"
#include "clang/Lex/Lexer.h"
#include "llvm/Support/JSON.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Support/FileSystem.h"
#include <string>
#include <regex>

using namespace clang;
using namespace clang::ast_matchers;

namespace {

// Devuelve el texto crudo de un rango fuente
static std::string getSourceText(SourceRange R, ASTContext &Ctx) {
    auto &SM = Ctx.getSourceManager();
    bool invalid = false;
    auto beginLoc = SM.getSpellingLoc(R.getBegin());
    auto endLoc   = SM.getSpellingLoc(R.getEnd());
    const char *beginData = SM.getCharacterData(beginLoc, &invalid);
    if (invalid) return {};
    unsigned startOffset = SM.getFileOffset(beginLoc);
    unsigned endOffset   = SM.getFileOffset(endLoc)
                            + Lexer::MeasureTokenLength(R.getEnd(), SM, Ctx.getLangOpts());
    return std::string(beginData, endOffset - startOffset);
}

// Formatea un QualType como string
static std::string getTypeAsString(QualType QT, ASTContext &Ctx) {
    PrintingPolicy Policy(Ctx.getLangOpts());
    Policy.adjustForCPlusPlus();
    return QT.getAsString(Policy);
}

// Elimina macros DSL del cuerpo: SharedVar, SharedVarRO, Service, Periodic(...)
static std::string stripDSL(const std::string &input) {
    static const std::regex pat(R"((\bSharedVarRO\b|\bSharedVar\b|\bService\b|Periodic\([^)]*\))\s*)");
    return std::regex_replace(input, pat, "");
}

class PeripheralCB : public MatchFinder::MatchCallback {
    std::string OutDir_;
public:
    explicit PeripheralCB(std::string OutDir)
      : OutDir_(std::move(OutDir)) {}

    void run(const MatchFinder::MatchResult &Result) override {
        auto *C = Result.Nodes.getNodeAs<CXXRecordDecl>("periph");
        if (!C || !C->hasDefinition()) return;
        ASTContext &Ctx = *Result.Context;
        // Filtrar annotate("eos::peripheral")
        bool isPeripheral = false;
        for (auto *A : C->attrs()) {
            if (auto *Ann = dyn_cast<AnnotateAttr>(A)) {
                if (Ann->getAnnotation() == "eos::peripheral") { isPeripheral = true; break; }
            }
        }
        if (!isPeripheral) return;

        std::string name = C->getNameAsString();
        llvm::json::Object j;
        j["name"] = name;

        // Capturar solo el bloque class { ... } con getBraceRange
        SourceRange br = C->getBraceRange();
        auto rawBody = getSourceText(br, Ctx);
        // Quitar etiquetas DSL
        auto stripped = stripDSL(rawBody);
        // Sanitizar caracteres no-ASCII
        std::string bodySan;
        bodySan.reserve(stripped.size());
        for (unsigned char ch : stripped) {
            bodySan.push_back(ch < 0x80 ? ch : '?');
        }
        j["body"] = bodySan;

        // Campos
        llvm::json::Array fields;
        for (auto *F : C->fields()) {
            llvm::json::Object jf;
            jf["name"] = F->getNameAsString();
            jf["type"] = getTypeAsString(F->getType(), Ctx);
            bool shared=false, ro=false;
            for (auto *A : F->attrs())
                if (auto *Ann = dyn_cast<AnnotateAttr>(A)) {
                    auto tag = Ann->getAnnotation();
                    if (tag == "eos::shared")    shared = true;
                    if (tag == "eos::shared_ro") { shared = true; ro = true; }
                }
            jf["shared"]   = shared;
            jf["readonly"] = ro;
            fields.push_back(std::move(jf));
        }
        j["fields"] = std::move(fields);

        // Servicios
        llvm::json::Array svcs;
        for (auto *M : C->methods()) {
            bool isSvc = false;
            for (auto *A : M->attrs())
                if (auto *Ann = dyn_cast<AnnotateAttr>(A))
                    if (Ann->getAnnotation() == "eos::service") { isSvc = true; break; }
            if (!isSvc) continue;
            llvm::json::Object js;
            js["name"] = M->getNameAsString();
            llvm::json::Array params;
            for (auto *P : M->parameters()) {
                llvm::json::Array p{
                    getTypeAsString(P->getType(), Ctx),
                    P->getNameAsString()
                };
                params.push_back(std::move(p));
            }
            js["params"] = std::move(params);
            svcs.push_back(std::move(js));
        }
        j["services"] = std::move(svcs);

        // Primer ctor de usuario
        llvm::json::Array ctors;
        for (auto *K : C->ctors()) {
            if (!K->isUserProvided()) continue;
            llvm::json::Object ctor;
            llvm::json::Array params;
            for (auto *P : K->parameters()) {
                llvm::json::Object p;
                p["type"] = getTypeAsString(P->getType(), Ctx);
                p["name"] = P->getNameAsString();
                params.push_back(std::move(p));
            }
            ctor["params"] = std::move(params);
            ctor["init"]   = llvm::json::Array();
            ctors.push_back(std::move(ctor));
            break;
        }
        j["ctors"] = std::move(ctors);

        // Dump JSON
        std::string path = OutDir_ + "/" + name + ".json";
        std::error_code EC;
        llvm::raw_fd_ostream os(path, EC, llvm::sys::fs::OF_None);
        llvm::json::OStream J(os);
        J.value(std::move(j));
    }
};

class EOSAction : public PluginASTAction {
    std::string OutDir_;
    std::unique_ptr<MatchFinder> Finder_;
    PeripheralCB *CB_ = nullptr;
  protected:
    std::unique_ptr<ASTConsumer>
    CreateASTConsumer(CompilerInstance &CI, llvm::StringRef) override {
        Finder_ = std::make_unique<MatchFinder>();
        Finder_->addMatcher(
            cxxRecordDecl(isExpansionInMainFile()).bind("periph"),
            CB_
        );
        return Finder_->newASTConsumer();
    }

    bool ParseArgs(const CompilerInstance &CI,
                   const std::vector<std::string> &Args) override {
        for (auto &s : Args)
            if (s.rfind("outdir=", 0) == 0)
                OutDir_ = s.substr(7);
        if (OutDir_.empty()) OutDir_ = ".";
        CB_ = new PeripheralCB(OutDir_);
        return true;
    }
};

static FrontendPluginRegistry::Add<EOSAction>
X("eos-plugin", "Extracción JSON EOS DSL v0.3 sin etiquetas DSL");

} //  