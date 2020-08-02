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

    microbuf::debug::print_debug(a.as_bytes());

    std::cout<<"Num bytes in A after serialization: "<<a.as_bytes().size()<<"\n";
//    EXPECT_EQ(a.as_bytes(), expected_bytes);
}


TEST(microbuf_cpp_serialization, CRC16_AUG_CCITT)
{
    // test CRC16-AUG/CCITT implementation
    // cmp. http://srecord.sourceforge.net/crc16-ccitt.html

    EXPECT_EQ(microbuf::internal::crc16_aug_ccitt(nullptr, 0), 0x1D0F);
    EXPECT_EQ(microbuf::internal::crc16_aug_ccitt((const uint8_t*)("A"), 1), 0x9479);
    EXPECT_EQ(microbuf::internal::crc16_aug_ccitt((const uint8_t*)("123456789"), 9), 0xE5CC);

    std::vector<uint8_t> bytes {};
    for(size_t i=0; i<256; ++i)
    {
        bytes.push_back('A');
    }
    EXPECT_EQ(microbuf::internal::crc16_aug_ccitt(&bytes[0], 256), 0xE938);
}