#include "gtest/gtest.h"
#include <cstdint>
#include "microbuf_debug.h"
#include <iostream>

TEST(microbuf_cpp_serialization, fixarray)
{
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

TEST(microbuf_cpp_serialization, uint)
{
    std::vector<uint8_t> bytes, expected_bytes;

    microbuf::append_uint8(bytes, 5);
    microbuf::append_uint8(bytes, 42);
    microbuf::append_uint8(bytes, 255);
    expected_bytes = {0xcc, 0x05, 0xcc, 0x2a, 0xcc, 0xff};
    EXPECT_EQ(bytes, expected_bytes);
    bytes.clear();


    microbuf::append_uint16(bytes, 5);
    microbuf::append_uint16(bytes, 42);
    microbuf::append_uint16(bytes, 255);
    microbuf::append_uint16(bytes, 42000);
    expected_bytes = {0xcd, 0x00, 0x05, 0xcd, 0x00, 0x2a, 0xcd, 0x00, 0xff, 0xcd, 0xa4, 0x10};
    EXPECT_EQ(bytes, expected_bytes);
    bytes.clear();

    microbuf::append_uint32(bytes, 5);
    microbuf::append_uint32(bytes, 42);
    microbuf::append_uint32(bytes, 255);
    microbuf::append_uint32(bytes, 42000);
    microbuf::append_uint32(bytes, 420000);
    expected_bytes = {0xce, 0x00, 0x00, 0x00, 0x05, 0xce, 0x00, 0x00, 0x00, 0x2a, 0xce, 0x00, 0x00, 0x00, 0xff,
                      0xce, 0x00, 0x00, 0xa4, 0x10, 0xce, 0x00, 0x06, 0x68, 0xa0};
    EXPECT_EQ(bytes, expected_bytes);
    bytes.clear();



    microbuf::append_uint64(bytes, 5);
    microbuf::append_uint64(bytes, 420000);
    microbuf::append_uint64(bytes, 1234567890123456789);
    expected_bytes = {0xcf, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x05,
                      0xcf, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x68, 0xa0,
                      0xcf, 0x11, 0x22, 0x10, 0xf4, 0x7D, 0xe9, 0x81, 0x15
    };
    EXPECT_EQ(bytes, expected_bytes);
    bytes.clear();
}

TEST(microbuf_cpp_serialization, floating_point)
{
    std::vector<uint8_t> bytes, expected_bytes;

    microbuf::append_array(bytes, 2);
    microbuf::append_float32(bytes, 1.23);
    microbuf::append_float64(bytes, 4.56);
    expected_bytes = {0x92, 0xca, 0x3f, 0x9d, 0x70, 0xa4,
                            0xcb, 0x40, 0x12, 0x3D, 0x70, 0xA3, 0xD7, 0x0A, 0x3D};
    EXPECT_EQ(bytes, expected_bytes);
    bytes.clear();
}

TEST(microbuf_cpp_serialization, CRC16_AUG_CCITT)
{
    // test CRC16-AUG/CCITT implementation
    // cmp. http://srecord.sourceforge.net/crc16-ccitt.html

    EXPECT_EQ(microbuf::internal::crc16_aug_ccitt(nullptr, 0), 0x1D0F);
    EXPECT_EQ(microbuf::internal::crc16_aug_ccitt((const uint8_t*)("A"), 1), 0x9479);
    EXPECT_EQ(microbuf::internal::crc16_aug_ccitt((const uint8_t*)("123456789"), 9), 0xE5CC);

    // https://stackoverflow.com/a/50857431/997151
    EXPECT_EQ(microbuf::internal::crc16_aug_ccitt((const uint8_t*)("1234567890"), 10), 0x57d8);

    std::vector<uint8_t> bytes {};
    for(size_t i=0; i<256; ++i)
    {
        bytes.push_back('A');
    }
    EXPECT_EQ(microbuf::internal::crc16_aug_ccitt(&bytes[0], 256), 0xE938);
}

TEST(microbuf_cpp_serialization, append_crc)
{
    std::vector<uint8_t> bytes, expected_bytes;

    microbuf::append_array(bytes, 1);
    microbuf::append_uint64(bytes, 1234567890123456789);
    microbuf::append_crc(bytes);

    expected_bytes = {0x91,  0xcf, 0x11, 0x22, 0x10, 0xf4,  0x7d,  0xe9,  0x81,  0x15, 0xcd,  0x14, 0xe9 };
    EXPECT_EQ(bytes, expected_bytes);
    bytes.clear();
}