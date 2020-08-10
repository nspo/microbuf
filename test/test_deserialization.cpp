#include "gtest/gtest.h"
#include <cstdint>
#include "microbuf.h"
#include "microbuf_debug.h"
#include <iostream>

TEST(microbuf_cpp_deserialization, fixarray)
{
    EXPECT_TRUE(microbuf::check_fixarray<0>(microbuf::array<uint8_t,1>{0x91}, 1));
    EXPECT_TRUE(microbuf::check_fixarray<0>(microbuf::array<uint8_t,1>{0x9f}, 15));
    EXPECT_FALSE(microbuf::check_fixarray<0>(microbuf::array<uint8_t,1>{0x9f}, 14));
}

TEST(microbuf_cpp_deserialization, array16)
{
    EXPECT_TRUE(microbuf::check_array16<0>(microbuf::array<uint8_t,3>{0xdc, 0x00, 0x01}, 1));
    EXPECT_TRUE(microbuf::check_array16<0>(microbuf::array<uint8_t,3>{0xdc, 0xa4, 0x10}, 42000));
    EXPECT_FALSE(microbuf::check_array16<0>(microbuf::array<uint8_t,3>{0xdc, 0xa4, 0x10}, 42001));
}


TEST(microbuf_cpp_deserialization, array32)
{
    EXPECT_TRUE(microbuf::check_array32<0>(microbuf::array<uint8_t,5>{0xdd, 0x00, 0x00, 0x00, 0x01}, 1));
    EXPECT_TRUE(microbuf::check_array32<0>(microbuf::array<uint8_t,5>{0xdd, 0x00, 0x06, 0x68, 0xa0}, 420000));
    EXPECT_FALSE(microbuf::check_array32<0>(microbuf::array<uint8_t,5>{0xdd, 0x00, 0x06, 0x68, 0xa0}, 420001));
}

TEST(microbuf_cpp_deserialization, boolean)
{
    bool result {};
    EXPECT_TRUE((microbuf::parse_bool<0>(microbuf::array<uint8_t,1>{0xc3}, result)));
    EXPECT_EQ(result, true);
    EXPECT_TRUE((microbuf::parse_bool<0>(microbuf::array<uint8_t,1>{0xc2}, result)));
    EXPECT_EQ(result, false);
    EXPECT_FALSE((microbuf::parse_bool<0>(microbuf::array<uint8_t,1>{0xc4}, result)));
}


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

TEST(microbuf_cpp_deserialization, float32)
{
    float result {};
    EXPECT_TRUE((microbuf::parse_float32<0>(microbuf::array<uint8_t,5>{0xca, 0x3f, 0x9d, 0x70, 0xa4}, result)));
    EXPECT_EQ(result, static_cast<float>(1.23));
    EXPECT_FALSE((microbuf::parse_float32<0>(microbuf::array<uint8_t,5>{0xcb, 0x3f, 0x9d, 0x70, 0xa4}, result)));
}

TEST(microbuf_cpp_deserialization, float64)
{
    double result {};
    EXPECT_TRUE((microbuf::parse_float64<0>(microbuf::array<uint8_t,9>{0xcb, 0x40, 0x12, 0x3D, 0x70, 0xA3, 0xD7, 0x0A, 0x3D}, result)));
    EXPECT_EQ(result, static_cast<double>(4.56));
    EXPECT_FALSE((microbuf::parse_float64<0>(microbuf::array<uint8_t,9>{0xcc, 0x40, 0x12, 0x3D, 0x70, 0xA3, 0xD7, 0x0A, 0x3D}, result)));
}

TEST(microbuf_cpp_deserialization, verify_crc)
{
    const microbuf::array<uint8_t, 13> valid_bytes{0x91, 0xcf, 0x11, 0x22, 0x10, 0xf4, 0x7d, 0xe9, 0x81, 0x15, 0xcd,
                                                   0x14, 0xe9};
    microbuf::array<uint8_t,13> bytes = valid_bytes;

    EXPECT_TRUE(microbuf::verify_crc(bytes));
    bytes[0] = 0x90;
    EXPECT_FALSE(microbuf::verify_crc(bytes));
    bytes = valid_bytes; bytes[12] = 0xe8;
    EXPECT_FALSE(microbuf::verify_crc(bytes));
    bytes = valid_bytes; bytes[10] = 0xcc;
    EXPECT_FALSE(microbuf::verify_crc(bytes));
}
