#include "gtest/gtest.h"
#include <cstdint>
#include "microbuf.h"
#include "microbuf_debug.h"
#include <iostream>

TEST(microbuf_cpp_serialization, fixarray)
{
    EXPECT_EQ(microbuf::gen_fixarray(1), (microbuf::array<uint8_t,1>{0x91}));
    EXPECT_EQ(microbuf::gen_fixarray(15), (microbuf::array<uint8_t,1>{0x9f}));
}

TEST(microbuf_cpp_serialization, array16)
{
    EXPECT_EQ(microbuf::gen_array16(1), (microbuf::array<uint8_t,3>{0xdc, 0x00, 0x01}));
    EXPECT_EQ(microbuf::gen_array16(42000), (microbuf::array<uint8_t,3>{0xdc, 0xa4, 0x10}));
}


TEST(microbuf_cpp_serialization, array32)
{
    EXPECT_EQ(microbuf::gen_array32(1), (microbuf::array<uint8_t,5>{0xdd, 0x00, 0x00, 0x00, 0x01}));
    EXPECT_EQ(microbuf::gen_array32(420000), (microbuf::array<uint8_t,5>{0xdd, 0x00, 0x06, 0x68, 0xa0}));
}

TEST(microbuf_cpp_serialization, boolean)
{
    EXPECT_EQ(microbuf::gen_bool(true), (microbuf::array<uint8_t,1>{0xc3}));
    EXPECT_EQ(microbuf::gen_bool(false), (microbuf::array<uint8_t,1>{0xc2}));
}

TEST(microbuf_cpp_serialization, uint8)
{
    EXPECT_EQ(microbuf::gen_uint8(5), (microbuf::array<uint8_t,2>{0xcc, 0x05}));
    EXPECT_EQ(microbuf::gen_uint8(42), (microbuf::array<uint8_t,2>{0xcc, 0x2a}));
    EXPECT_EQ(microbuf::gen_uint8(255), (microbuf::array<uint8_t,2>{0xcc, 0xff}));
}

TEST(microbuf_cpp_serialization, uint16)
{
    EXPECT_EQ(microbuf::gen_uint16(5), (microbuf::array<uint8_t, 3>{0xcd, 0x00, 0x05}));
    EXPECT_EQ(microbuf::gen_uint16(42), (microbuf::array<uint8_t, 3>{0xcd, 0x00, 0x2a}));
    EXPECT_EQ(microbuf::gen_uint16(255), (microbuf::array<uint8_t, 3>{0xcd, 0x00, 0xff}));
    EXPECT_EQ(microbuf::gen_uint16(42000), (microbuf::array<uint8_t, 3>{0xcd, 0xa4, 0x10}));
}

TEST(microbuf_cpp_serialization, uint32)
{
    EXPECT_EQ(microbuf::gen_uint32(5), (microbuf::array<uint8_t, 5>{0xce, 0x00, 0x00, 0x00, 0x05}));
    EXPECT_EQ(microbuf::gen_uint32(42), (microbuf::array<uint8_t, 5>{0xce, 0x00, 0x00, 0x00, 0x2a}));
    EXPECT_EQ(microbuf::gen_uint32(255), (microbuf::array<uint8_t, 5>{0xce, 0x00, 0x00, 0x00, 0xff}));
    EXPECT_EQ(microbuf::gen_uint32(42000), (microbuf::array<uint8_t, 5>{0xce, 0x00, 0x00, 0xa4, 0x10}));
    EXPECT_EQ(microbuf::gen_uint32(420000), (microbuf::array<uint8_t, 5>{0xce, 0x00, 0x06, 0x68, 0xa0}));
}

TEST(microbuf_cpp_serialization, uint64)
{
    EXPECT_EQ(microbuf::gen_uint64(5), (microbuf::array<uint8_t, 9>{0xcf, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x05}));
    EXPECT_EQ(microbuf::gen_uint64(42), (microbuf::array<uint8_t, 9>{0xcf, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x2a}));
    EXPECT_EQ(microbuf::gen_uint64(255), (microbuf::array<uint8_t, 9>{0xcf, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff}));
    EXPECT_EQ(microbuf::gen_uint64(42000), (microbuf::array<uint8_t, 9>{0xcf, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xa4, 0x10}));
    EXPECT_EQ(microbuf::gen_uint64(420000), (microbuf::array<uint8_t, 9>{0xcf, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x68, 0xa0}));
    EXPECT_EQ(microbuf::gen_uint64(1234567890123456789U), (microbuf::array<uint8_t, 9>{0xcf, 0x11, 0x22, 0x10, 0xf4, 0x7D, 0xe9, 0x81, 0x15}));
    EXPECT_EQ(microbuf::gen_uint64(18446744073709551615U), (microbuf::array<uint8_t, 9>{0xcf, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff}));
}

TEST(microbuf_cpp_serialization, float32)
{
    EXPECT_EQ(microbuf::gen_float32(1.23), (microbuf::array<uint8_t, 5>{0xca, 0x3f, 0x9d, 0x70, 0xa4}));
}

TEST(microbuf_cpp_serialization, float64)
{
    EXPECT_EQ(microbuf::gen_float64(4.56), (microbuf::array<uint8_t, 9>{0xcb, 0x40, 0x12, 0x3D, 0x70, 0xA3, 0xD7, 0x0A, 0x3D}));
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
    bytes.reserve(256);
    for(size_t i=0; i<256; ++i)
    {
        bytes.push_back('A');
    }
    EXPECT_EQ(microbuf::internal::crc16_aug_ccitt(&bytes[0], 256), 0xE938);
}

TEST(microbuf_cpp_serialization, append_crc)
{
    microbuf::array<uint8_t,13> bytes {};
    microbuf::insert_bytes<0>(bytes, microbuf::gen_fixarray(1));
    microbuf::insert_bytes<1>(bytes, microbuf::gen_uint64(1234567890123456789U));
    microbuf::append_crc(bytes);

    EXPECT_EQ(bytes, (microbuf::array<uint8_t,13>{0x91,  0xcf, 0x11, 0x22, 0x10, 0xf4,  0x7d,  0xe9,  0x81,  0x15, 0xcd,  0x14, 0xe9}));
}

TEST(microbuf_cpp_serialization, gen_multiple)
{
    const uint16_t source_data1[] {1, 2, 3, 100, 200, 300};
    EXPECT_EQ(microbuf::gen_multiple<6>(source_data1, microbuf::gen_uint16),
              (microbuf::array<uint8_t, 18>{0xcd, 0x00, 0x01, 0xcd, 0x00, 0x02, 0xcd, 0x00, 0x03, 0xcd, 0x00, 0x64,
                                            0xcd, 0x00, 0xc8, 0xcd, 0x01, 0x2c}));

    const float source_data2[] {1.23, 4.56, 7.89};
    EXPECT_EQ(microbuf::gen_multiple<3>(source_data2, microbuf::gen_float32),
              (microbuf::array<uint8_t, 15>{0xca, 0x3f, 0x9d, 0x70, 0xa4, 0xca, 0x40, 0x91, 0xeb, 0x85, 0xca, 0x40,
                                            0xfc, 0x7a, 0xe1}));

    const float source_data3[] {1.23, 4.56, 7.89};
    EXPECT_EQ(microbuf::gen_multiple_unsafe<3>(source_data3, microbuf::gen_float32),
              (microbuf::array<uint8_t, 15>{0xca, 0x3f, 0x9d, 0x70, 0xa4, 0xca, 0x40, 0x91, 0xeb, 0x85, 0xca, 0x40,
                                            0xfc, 0x7a, 0xe1}));
}