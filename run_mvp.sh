#!/usr/bin/env bash
# run_mvp.sh  – Build & test EOSPlugin fuera de PlatformIO
# -------------------------------------------------------
# Requisitos previos (host):
#   • clang 14‑17 + clang‑dev         # sudo apt install clang-17 clang-17-dev
#   • cmake ≥3.10
#   • python3 -m pip install jinja2
# -------------------------------------------------------

set -euo pipefail
ROOT=$(pwd)

# ——————————————
# Configuración Homebrew‑LLVM
BREW_LLVM=$(brew --prefix llvm)

# Usa el clang/clang++ de Homebrew, no el AppleClang de Xcode
export CC="$BREW_LLVM/bin/clang"
export CXX="$BREW_LLVM/bin/clang++"

# Asegura que los headers/libs de Homebrew LLVM estén primero
export PATH="$BREW_LLVM/bin:$PATH"
export LDFLAGS="-L$BREW_LLVM/lib"
export CPPFLAGS="-I$BREW_LLVM/include"

# Indica a CMake dónde está LLVMConfig.cmake
export LLVM_DIR="$BREW_LLVM/lib/cmake/llvm"
# ——————————————

ROOT=$(pwd)

echo "1) Compilando el plugin LLVM …"
mkdir -p plugin/build
cmake -S plugin -B plugin/build
cmake --build plugin/build

PLUGIN=$(realpath plugin/build/EOSPlugin.*)


echo "2) Analizando el FanController.hpp con clang + plugin …"
SRC=test/FanController.hpp                     # o donde tengas tu ejemplo
OUT=generated
mkdir -p "$OUT"
clang++ -std=c++17 -fsyntax-only \
    -Iinclude \
    -Xclang -load -Xclang "$PLUGIN" \
    -Xclang -plugin -Xclang eos-plugin \
    -Xclang -plugin-arg-eos-plugin -Xclang "outdir=$OUT" \
    -Iinclude "$SRC"

echo "   → generado $OUT/FanController.json"

echo "3) Renderizando Jinja2 → header final …"
python3 scripts/render.py \
        "$OUT/FanController.json" \
        "$OUT/FanController_gen.hpp"

echo "   → generado $OUT/FanController_gen.hpp"

echo "4) Compilación sanity‑check (opcional) …"
clang++ -std=c++17 -Iinclude -Igenerated -c "$SRC" -o /dev/null
echo "   ✔ Compila limpio."

echo -e "\n🎉  MVP verificado sin PlatformIO."
