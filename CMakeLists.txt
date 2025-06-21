cmake_minimum_required(VERSION 3.10)
project(JsonLibrary VERSION 1.0 LANGUAGES C CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

include_directories(${PROJECT_SOURCE_DIR}/include)

set(SOURCES
    src/json_parser.cpp
    src/json_tokenizer.cpp
    src/json.cpp
)

add_library(json_static STATIC ${SOURCES})
set_target_properties(json_static PROPERTIES
    OUTPUT_NAME json
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
)

add_library(json_shared SHARED ${SOURCES})
set_target_properties(json_shared PROPERTIES
    OUTPUT_NAME json
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
)

install(TARGETS json_static json_shared
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin
)

install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/
    DESTINATION include/json
    FILES_MATCHING PATTERN "*.h"
)

enable_testing()

find_package(GTest)
if (NOT GTest_FOUND)
    add_subdirectory(tests/lib/googletest)
    include_directories(tests/lib/googletest/googletest/include)
endif()

set(TEST_SOURCES
    tests/test_json_parser.cpp
)

add_executable(test_json_parser ${TEST_SOURCES})
target_link_libraries(test_json_parser json_static GTest::gtest GTest::gtest_main pthread)

set_target_properties(test_json_parser PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/tests
)

add_test(NAME JsonTests COMMAND test_json_parser)