# This CMake configuration file sets up a project named "eos_framework" with C++17 as the required standard.
# It defines the following targets:
# - `eos_core`: A library target built from the source file `src/eos_core.cpp`.
# - `host_tests`: An executable target built from the source file `src/main.cpp`.
# The `host_tests` target is linked privately to the `eos_core` library.
# The minimum required CMake version for this configuration is 3.20.
cmake_minimum_required(VERSION 3.20)

project(eos_framework LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(eos_core src/eos_core.cpp)
add_executable(host_tests src/main.cpp)

target_link_libraries(host_tests PRIVATE eos_core)