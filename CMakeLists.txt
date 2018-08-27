cmake_minimum_required(VERSION 3.5)
project(tisa VERSION 1.0.0 LANGUAGES CXX)


file(GLOB_RECURSE HEADERS include/*.hpp)
file(GLOB_RECURSE SOURCES source/*.cpp)
add_library(tisa STATIC ${HEADERS} ${SOURCES})

target_include_directories(tisa PUBLIC include )
target_compile_definitions(tisa PUBLIC )
target_compile_features(tisa PUBLIC cxx_std_17)
target_compile_options(tisa PUBLIC -Wno-gnu-string-literal-operator-template -fmodules-ts)
