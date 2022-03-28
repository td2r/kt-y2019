cmake_minimum_required(VERSION 3.16)
project(lab2)

set(CMAKE_CXX_STANDARD 17)

add_executable(lab2
               main.cpp
               draw.cpp draw.h
               lexer.h lexer.cpp
               parse.h parse.cpp
               tree.cpp tree.h)

target_link_libraries(lab2 PUBLIC gvc cgraph)

include(FetchContent)
FetchContent_Declare(googletest
                     URL https://github.com/google/googletest/archive/3e0e32ba300ce8afe695ad3ba7e81b21b7cf237a.zip)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(test_parse
               test/test_parse.cc
               lexer.h lexer.cpp
               parse.h parse.cpp
               tree.cpp tree.h)

target_link_libraries(test_parse
                      gtest_main)

include(GoogleTest)
gtest_discover_tests(test_parse)