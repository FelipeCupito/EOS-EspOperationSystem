cmake_minimum_required(VERSION 3.10)
project(EOSPlugin)

find_package(LLVM REQUIRED CONFIG)
list(APPEND CMAKE_MODULE_PATH "${LLVM_CMAKE_DIR}")
include(HandleLLVMOptions)
include(AddLLVM)

add_definitions(${LLVM_DEFINITIONS})
include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(-D_GLIBCXX_USE_CXX11_ABI=0)

add_library(EOSPlugin MODULE src/EOSPlugin.cpp)
set_target_properties(EOSPlugin PROPERTIES
    PREFIX ""
    COMPILE_FLAGS "-fno-rtti"
)

target_link_libraries(EOSPlugin
    PRIVATE
    clangAST
    clangASTMatchers
    clangBasic
    clangFrontend
    clangTooling
)
