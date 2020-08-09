#include "gtest/gtest.h"
#include <cstdint>
#include "microbuf.h"
#include "microbuf_debug.h"
#include <iostream>

TEST(microbuf_cpp_deserialization, uint8)
{
    uint8_t result {};
    EXPECT_TRUE((microbuf::parse_uint8<0>(microbuf::array<uint8_t,2>{0xcc, 0x20}, result)));
    EXPECT_EQ(result, 32);
    EXPECT_TRUE((microbuf::parse_uint8<0>(microbuf::array<uint8_t,3>{0xcc, 0xff}, result)));
    EXPECT_EQ(result, 255);
    EXPECT_FALSE((microbuf::parse_uint8<0>(microbuf::array<uint8_t,3>{0xcd, 0xff}, result)));
}

TEST(microbuf_cpp_deserialization, uint16)
{
    uint16_t result {};
    EXPECT_TRUE((microbuf::parse_uint16<0>(microbuf::array<uint8_t,3>{0xcd, 0x00, 0xff}, result)));
    EXPECT_EQ(result, 255);
    EXPECT_TRUE((microbuf::parse_uint16<0>(microbuf::array<uint8_t,3>{0xcd, 0xa4, 0x10}, result)));
    EXPECT_EQ(result, 42000);
}

TEST(microbuf_cpp_deserialization, uint32)
{
    uint32_t result {};
    EXPECT_TRUE((microbuf::parse_uint32<0>(microbuf::array<uint8_t,5>{0xce, 0x00, 0x00, 0xa4, 0x10}, result)));
    EXPECT_EQ(result, 42000);
    EXPECT_TRUE((microbuf::parse_uint32<0>(microbuf::array<uint8_t,5>{0xce, 0x00, 0x06, 0x68, 0xa0}, result)));
    EXPECT_EQ(result, 420000);
}

TEST(microbuf_cpp_deserialization, uint64)
{
    uint64_t result {};
    EXPECT_TRUE((microbuf::parse_uint64<0>(microbuf::array<uint8_t,9>{0xcf, 0x11, 0x22, 0x10, 0xf4, 0x7D, 0xe9, 0x81, 0x15}, result)));
    EXPECT_EQ(result, 1234567890123456789U);
    EXPECT_TRUE((microbuf::parse_uint64<0>(microbuf::array<uint8_t,9>{0xcf, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff}, result)));
    EXPECT_EQ(result, 18446744073709551615U);
}