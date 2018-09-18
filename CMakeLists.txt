cmake_minimum_required(VERSION 3.5)
project(tisa VERSION 1.0.0 LANGUAGES CXX)


file(GLOB_RECURSE HEADERS include/*.hpp)
file(GLOB_RECURSE SOURCES source/*.cpp)
add_library(tisa STATIC ${HEADERS} ${SOURCES})

target_include_directories(tisa PUBLIC include )
target_compile_definitions(tisa PUBLIC )
target_compile_features(tisa PUBLIC cxx_std_17)

option(TISA_BUILD_TESTS "Build tests" ON)
if(TISA_BUILD_TESTS)
    project(tisa-tests VERSION 1.0.0 LANGUAGES CXX)
    file(GLOB_RECURSE TEST-SOURCES test/*.cpp)
    add_executable(tisa-test ${TEST-SOURCES})
    target_link_libraries(tisa-test tisa rusa)
endif()

project(tisa-macro VERSION 1.0.0 LANGUAGES CXX)
add_executable(tisa-macro tools/tisa-macro.cpp)
target_compile_features(tisa-macro PUBLIC cxx_std_17)
