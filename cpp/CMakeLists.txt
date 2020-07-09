cmake_minimum_required(VERSION 3.16)
project(foo)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(foo
        foo.cpp
        microbuf.h)
