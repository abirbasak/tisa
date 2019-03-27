cmake_minimum_required(VERSION 3.5)
project(tisa VERSION 1.0.0 LANGUAGES CXX)

find_package(unofficial-libuv CONFIG REQUIRED)
find_package(unofficial-http-parser CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)
    
file(GLOB_RECURSE HEADERS include/*.hpp)
file(GLOB_RECURSE SOURCES source/*.cpp)
add_library(tisa STATIC ${HEADERS} ${SOURCES})

target_include_directories(tisa PUBLIC include )
target_compile_definitions(tisa PUBLIC )
target_compile_features(tisa PUBLIC cxx_std_17)

target_link_libraries(tisa PRIVATE sqlite3)
target_link_libraries(tisa PRIVATE unofficial::libuv::libuv)
target_link_libraries(tisa PRIVATE unofficial::http_parser::http_parser)
target_link_libraries(tisa PRIVATE nlohmann_json nlohmann_json::nlohmann_json)

option(TISA_BUILD_TESTS "Build tests" ON)
if(TISA_BUILD_TESTS)
    project(tisa-test VERSION 1.0.0 LANGUAGES CXX)
    file(GLOB_RECURSE TEST-HEADERS test/*.hpp)
    file(GLOB_RECURSE TEST-SOURCES test/*.cpp)
    add_executable(tisa-test ${TEST-HEADERS} ${TEST-SOURCES})
    target_include_directories(tisa-test PRIVATE test)
    target_link_libraries(tisa-test tisa rusa)
endif()

option(TISA_BUILD_EXAMPLES "Build examples" ON)
if(TISA_BUILD_EXAMPLES)
    project(sqlite-example VERSION 1.0.0 LANGUAGES CXX)
    add_executable(sqlite-example examples/sqlite-example.cpp)
    target_link_libraries(sqlite-example tisa)
    
    project(persistence-example VERSION 1.0.0 LANGUAGES CXX)
    add_executable(persistence-example examples/sqlite-example.cpp)
    target_link_libraries(persistence-example tisa)

    project(svg-example VERSION 1.0.0 LANGUAGES CXX)
    add_executable(svg-example examples/svg-example.cpp)
    target_link_libraries(svg-example tisa)
endif()


project(tisa-macro VERSION 1.0.0 LANGUAGES CXX)
add_executable(tisa-macro tools/tisa-macro.cpp)
target_compile_features(tisa-macro PUBLIC cxx_std_17)
