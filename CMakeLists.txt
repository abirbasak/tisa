cmake_minimum_required(VERSION 3.5)
project(tisa VERSION 1.0.0 LANGUAGES CXX)


file(GLOB_RECURSE HEADERS include/*.hpp)
file(GLOB_RECURSE SOURCES source/*.cpp)
add_library(tisa INTERFACE)

add_custom_target(tisa-code SOURCES ${HEADERS} ${SOURCES})
target_include_directories(tisa INTERFACE include )
target_compile_definitions(tisa INTERFACE )
target_compile_features(tisa INTERFACE cxx_std_17)
target_compile_options(tisa INTERFACE -Wno-gnu-string-literal-operator-template -fmodules-ts)
