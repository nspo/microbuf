if (POLICY CMP0048)
    cmake_policy(SET CMP0048 NEW)
endif (POLICY CMP0048)

project(microbuf)
cmake_minimum_required(VERSION 3.0)

add_subdirectory(test)
add_subdirectory(cpp)