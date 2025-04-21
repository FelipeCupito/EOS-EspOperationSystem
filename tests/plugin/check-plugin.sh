#!/usr/bin/env bash
# asume que estás en eos-framework root
mkdir -p plugin/build
cmake -S plugin -B plugin/build
cmake --build plugin/build
# ahora corre lit
lit -v tests/plugin





#  corre el plugin
# chmod +x tests/plugin/check-plugin.sh
# ./tests/plugin/check-plugin.sh