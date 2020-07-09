project(microbuf_UnitTests)
cmake_minimum_required(VERSION 3.0)

add_subdirectory(submodules/googletest)

include_directories(../cpp/ ../output/
        ${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR}
)

add_executable(microbuf_tests test_serialization.cpp)
target_link_libraries(microbuf_tests gtest gtest_main)