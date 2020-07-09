if (POLICY CMP0048)
    cmake_policy(SET CMP0048 NEW)
endif (POLICY CMP0048)

project(microbuf_UnitTests)
cmake_minimum_required(VERSION 3.0)

add_subdirectory(submodules/googletest)
include_directories(../output ../cpp
        ${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR}
)

add_executable(microbuf_tests example_test.cpp)
target_link_libraries(microbuf_tests gtest gtest_main)