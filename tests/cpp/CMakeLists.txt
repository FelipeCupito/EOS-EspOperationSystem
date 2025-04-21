cmake_minimum_required(VERSION 3.10)
project(EOSFrameworkRuntimeTests)

# Ruta a tus headers
include_directories(${CMAKE_SOURCE_DIR}/../include)

find_package(GTest REQUIRED)
enable_testing()

add_executable(runtime_tests
    SharedBase_test.cpp
    # otros test files...
)

target_link_libraries(runtime_tests
    GTest::GTest
    GTest::Main
)

add_test(NAME runtime_tests COMMAND runtime_tests)

