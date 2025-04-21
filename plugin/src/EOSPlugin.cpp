// // plugin/src/EOSPlugin.cpp
// // ————————— Detecta clases eos::peripheral y genera un JSON —————————
// #include "clang/AST/AST.h"
// #include "clang/ASTMatchers/ASTMatchers.h"
// #include "clang/ASTMatchers/ASTMatchFinder.h"
// #include "clang/Frontend/FrontendPluginRegistry.h"
// #include "llvm/Support/JSON.h"
// #include "llvm/Support/raw_ostream.h"
// #include "llvm/Support/FileSystem.h"
// #include <string>

// using namespace clang;
// using namespace clang::ast_matchers;

// namespace {

// static std::string getTypeAsString(const QualType &QT, ASTContext &Ctx) {
//     PrintingPolicy Policy(Ctx.getLangOpts());
//     Policy.adjustForCPlusPlus();
//     return QT.getAsString(Policy);
// }

// class PeripheralCB : public MatchFinder::MatchCallback {
//     std::string OutDir_;
//   public:
//     explicit PeripheralCB(std::string OutDir) : OutDir_(std::move(OutDir)) {}

//     void run(const MatchFinder::MatchResult &Result) override {
//         auto *C = Result.Nodes.getNodeAs<CXXRecordDecl>("periph");
//         if (!C || !C->hasDefinition()) return;

//         ASTContext &Ctx = *Result.Context;
//         std::string name = C->getNameAsString();

//         llvm::json::Object j;
//         j["name"] = name;

//         // Campos
//         llvm::json::Array fields;
//         for (auto *F : C->fields()) {
//             llvm::json::Object jf;
//             jf["name"]   = F->getNameAsString();
//             jf["type"]   = getTypeAsString(F->getType(), Ctx);
//             bool shared=false, ro=false;
//             for (auto *A : F->attrs())
//                 if (auto *Ann = dyn_cast<AnnotateAttr>(A)) {
//                     auto tag = Ann->getAnnotation();
//                     if (tag=="eos::shared")    shared=true;
//                     if (tag=="eos::shared_ro") { shared=true; ro=true; }
//                 }
//             jf["shared"]   = shared;
//             jf["readonly"] = ro;
//             fields.push_back(std::move(jf));
//         }
//         j["fields"] = std::move(fields);

//         // Servicios
//         llvm::json::Array svcs;
//         for (auto *M : C->methods()) {
//             bool isSvc = false;
//             for (auto *A : M->attrs())
//                 if (auto *Ann = dyn_cast<AnnotateAttr>(A))
//                     if (Ann->getAnnotation()=="eos::service")
//                         isSvc = true;
//             if (!isSvc) continue;

//             llvm::json::Object js;
//             js["name"] = M->getNameAsString();
//             llvm::json::Array params;
//             for (auto *P : M->parameters()) {
//                 llvm::json::Array p{ getTypeAsString(P->getType(), Ctx),
//                                      P->getNameAsString() };
//                 params.push_back(std::move(p));
//             }
//             js["params"] = std::move(params);
//             svcs.push_back(std::move(js));
//         }
//         j["services"] = std::move(svcs);

//         // Solo primer ctor de usuario
//         for (auto *K : C->ctors()) {
//             if (!K->isUserProvided()) continue;
//             llvm::json::Object ctor;
//             llvm::json::Array params, init;
//             for (auto *P : K->parameters()) {
//                 llvm::json::Object p;
//                 p["type"] = getTypeAsString(P->getType(), Ctx);
//                 p["name"] = P->getNameAsString();
//                 params.push_back(std::move(p));
//             }
//             for (auto *I : K->inits())
//                 init.push_back(I->getSourceRange()
//                                  .getBegin()
//                                  .printToString(Ctx.getSourceManager()));
//             ctor["params"] = std::move(params);
//             ctor["init"]   = std::move(init);
//             j["ctors"]     = llvm::json::Array{std::move(ctor)};
//             break;
//         }

//         // Escribe JSON
//         std::string path = OutDir_ + "/" + name + ".json";
//         std::error_code EC;
//         llvm::raw_fd_ostream os(path, EC, llvm::sys::fs::OF_None);
//         llvm::json::OStream J(os);
//         J.value(std::move(j));
//     }
// };

// class EOSAction : public PluginASTAction {
//     std::string OutDir_;
//     std::unique_ptr<MatchFinder> Finder_;
//     PeripheralCB *Handler_{};
//   protected:
//     std::unique_ptr<ASTConsumer>
//     CreateASTConsumer(CompilerInstance &CI, llvm::StringRef) override {
//         Finder_ = std::make_unique<MatchFinder>();
//         Finder_->addMatcher(
//             cxxRecordDecl(
//                 isExpansionInMainFile(),
//                 annotateAttr("eos::peripheral")
//             )
//             .bind("periph"),
//             Handler_);
//         return Finder_->newASTConsumer();
//     }

//     bool ParseArgs(const CompilerInstance &CI,
//                    const std::vector<std::string> &Args) override {
//         for (auto &s : Args)
//             if (s.rfind("outdir=", 0) == 0)
//                 OutDir_ = s.substr(7);
//         if (OutDir_.empty()) OutDir_ = ".";
//         Handler_ = new PeripheralCB(OutDir_);
//         return true;
//     }
// };

// } // namespace

// static FrontendPluginRegistry::Add<EOSAction>
// X("eos-plugin", "Genera JSON para EOS DSL");
// plugin/src/EOSPlugin.cpp
// ————————— Detecta clases eos::peripheral y genera un JSON —————————

#include "clang/AST/AST.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/Frontend/FrontendPluginRegistry.h"
#include "llvm/Support/JSON.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Support/FileSystem.h"
#include <string>

using namespace clang;
using namespace clang::ast_matchers;

namespace {

static std::string getTypeAsString(const QualType &QT, ASTContext &Ctx) {
    PrintingPolicy Policy(Ctx.getLangOpts());
    Policy.adjustForCPlusPlus();
    return QT.getAsString(Policy);
}

class PeripheralCB : public MatchFinder::MatchCallback {
    std::string OutDir_;
  public:
    explicit PeripheralCB(std::string OutDir)
      : OutDir_(std::move(OutDir)) {}

    void run(const MatchFinder::MatchResult &Result) override {
        auto *C = Result.Nodes.getNodeAs<CXXRecordDecl>("periph");
        if (!C || !C->hasDefinition()) return;

        // Filtra solo los que tienen annotate("eos::peripheral")
        bool isPeripheral = false;
        for (auto *A : C->attrs()) {
            if (auto *Ann = dyn_cast<AnnotateAttr>(A)) {
                if (Ann->getAnnotation() == "eos::peripheral") {
                    isPeripheral = true;
                    break;
                }
            }
        }
        if (!isPeripheral) return;

        ASTContext &Ctx = *Result.Context;
        std::string name = C->getNameAsString();

        llvm::json::Object j;
        j["name"] = name;

        // Campos
        llvm::json::Array fields;
        for (auto *F : C->fields()) {
            llvm::json::Object jf;
            jf["name"] = F->getNameAsString();
            jf["type"] = getTypeAsString(F->getType(), Ctx);
            bool shared = false, ro = false;
            for (auto *A : F->attrs()) {
                if (auto *Ann = dyn_cast<AnnotateAttr>(A)) {
                    auto tag = Ann->getAnnotation();
                    if (tag == "eos::shared")    shared = true;
                    if (tag == "eos::shared_ro") { shared = true; ro = true; }
                }
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
            for (auto *A : M->attrs()) {
                if (auto *Ann = dyn_cast<AnnotateAttr>(A)) {
                    if (Ann->getAnnotation() == "eos::service")
                        isSvc = true;
                }
            }
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

        // Primer ctor
        for (auto *K : C->ctors()) {
            if (!K->isUserProvided()) continue;
            llvm::json::Object ctor;
            llvm::json::Array params, init;
            for (auto *P : K->parameters()) {
                llvm::json::Object p;
                p["type"] = getTypeAsString(P->getType(), Ctx);
                p["name"] = P->getNameAsString();
                params.push_back(std::move(p));
            }
            for (auto *I : K->inits()) {
                init.push_back(
                  I->getSourceRange()
                   .getBegin()
                   .printToString(Ctx.getSourceManager())
                );
            }
            ctor["params"] = std::move(params);
            ctor["init"]   = std::move(init);
            j["ctors"]     = llvm::json::Array{std::move(ctor)};
            break;
        }

        // Escribe JSON
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
    PeripheralCB *Handler_{nullptr};
  protected:
    std::unique_ptr<ASTConsumer>
    CreateASTConsumer(CompilerInstance &CI, llvm::StringRef) override {
        Finder_ = std::make_unique<MatchFinder>();
        // Match de todos los tipos; filtramos en el callback
        Finder_->addMatcher(
            cxxRecordDecl(isExpansionInMainFile()).bind("periph"),
            Handler_
        );
        return Finder_->newASTConsumer();
    }

    bool ParseArgs(const CompilerInstance &CI,
                   const std::vector<std::string> &Args) override {
        for (auto &s : Args) {
            if (s.rfind("outdir=", 0) == 0)
                OutDir_ = s.substr(7);
        }
        if (OutDir_.empty()) OutDir_ = ".";
        Handler_ = new PeripheralCB(OutDir_);
        return true;
    }
};

} // namespace

static FrontendPluginRegistry::Add<EOSAction>
X("eos-plugin", "Genera JSON para EOS DSL");
