cmake_minimum_required(VERSION 3.14)
project(my_project LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build." FORCE)
endif()

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_library(db_engine STATIC)

target_sources(db_engine
  PRIVATE
    storage/storage.cpp
    storage/storage.h
    storage/dbwriter.cpp
    storage/dbwriter.h
    storage/dbmetadata.h
    storage/dbmetadata.cpp
    metadata/cell.h
    metadata/cell.cpp
    utils/formatter.cpp
    utils/formatter.h
    utils/serialization.cpp
    utils/serialization.h
    operators/full_scan.cpp
    operators/full_scan.h
    operators/abstract_iterator.h
    operators/where_operator.cpp
    operators/where_operator.h
    operators/projection_operator.cpp
    operators/projection_operator.h
    operators/expressions.h
    operators/execution_plan_base.h
    operators/single_row_operator.cpp
    operators/single_row_operator.h
    operators/union_all_operator.cpp
    operators/union_all_operator.h
    sql_interface/scanner.h
    sql_interface/scanner.cpp
    sql_interface/parser.h
    sql_interface/parser.cpp
    query_engine/compiler.h
    query_engine/compiler.cpp
    utils/test_dbengine.h
    utils/test_dbengine.cpp
    types/type_interface.h
    types/type_interface.cpp
    types/type_base.h
    types/type_base.cpp
)

target_include_directories(db_engine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

if(MSVC)
    target_compile_options(db_engine PRIVATE /W4 /WX)
else()
    target_compile_options(db_engine PRIVATE -Wall -Wextra -Wpedantic -Wshadow -Werror)
endif()

add_compile_options(-fsanitize=address,undefined)
add_link_options(-fsanitize=address,undefined)

enable_testing()

add_executable(dbcpp_test)

target_sources(dbcpp_test
  PRIVATE
    tests/storage_test.cpp
    tests/formatter_test.cpp
    tests/serialization_test.cpp
    tests/full_scan_test.cpp
    tests/scanner_test.cpp
    tests/sql_test.cpp
)

target_link_libraries(dbcpp_test
  PRIVATE
    db_engine
    GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(dbcpp_test)

# add_executable(db_cli main.cpp)
# target_link_libraries(db_cli PRIVATE db_engine)