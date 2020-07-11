#include "gtest/gtest.h"
#include "A.h"
#include "microbuf_debug.h"
#include <cstdint>
#include <iostream>
#include <iomanip>
#include <ios>

TEST(microbuf_cpp_serialization, fixarray)
{
    using namespace microbuf::debug;

    std::vector<uint8_t> bytes;

    microbuf::append_array(bytes, 1);
    std::vector<uint8_t> expected_bytes {0x91};
    EXPECT_EQ(bytes, expected_bytes);
    bytes.clear();


    microbuf::append_array(bytes, 15);
    expected_bytes = {0x9f};
    EXPECT_EQ(bytes, expected_bytes);
    bytes.clear();
}

TEST(microbuf_cpp_serialization, MessageA)
{
    A_struct_t a;
    a.foo = 1.23;
    a.bar = 4.56;

    std::vector<uint8_t> expected_bytes {0x91};

    std::cout<<"Num bytes in A after serialization: "<<a.as_bytes().size()<<"\n";
    EXPECT_EQ(a.as_bytes(), expected_bytes);
}