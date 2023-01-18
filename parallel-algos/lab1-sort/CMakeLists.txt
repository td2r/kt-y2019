cmake_minimum_required(VERSION 3.23)
project(lab1_sort)

# Doesn't work for some reason
# Instead add -D CMAKE_CXX_COMPILER=/opt/opencilk/bin/clang++
# to cmake invocation
#set(CMAKE_CXX_COMPILER /opt/opencilk/bin/clang++)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "-fopencilk -O3")

add_executable(lab1_sort main.cpp sequential.h parallel.h sort_utils.h)

#add_executable(cs main.cpp sequential.h sort_utils.h parallel.h)
#set_target_properties(cs PROPERTIES COMPILE_FLAGS "-fcilktool=cilkscale")

#add_executable(cs_bench main.cpp sequential.h parallel.h sort_utils.h)
#set_target_properties(cs_bench PROPERTIES COMPILE_FLAGS -fcilktool=cilkscale-benchmark)

include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(tests tests.cc parallel.h sort_utils.h)
target_link_libraries(tests GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(tests)

