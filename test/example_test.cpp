#include "gtest/gtest.h"
#include "A.h"
#include "microbuf_debug.h"
#include <cstdint>
#include <iostream>
#include <iomanip>
#include <ios>

TEST(example, subtract)
{
double res = 10 - 8.;
ASSERT_NEAR(res, 2.0, 1e-9);
}

TEST(microbuf_cpp_serialization, FixArray)
{
    using namespace microbuf::debug;

    std::vector<uint8_t> bytes;
    microbuf::append_array(bytes, 1);
    append_multiple_uint8(bytes, 1);

    std::vector<uint8_t> expected_bytes {0x91, 0xcc, 0x00};
    EXPECT_EQ(bytes, expected_bytes);
    bytes.clear();


    microbuf::append_array(bytes, 15);
    append_multiple_uint8(bytes, 15);

    expected_bytes = {0x9f, 0xcc, 0x00, 0xcc, 0x01, 0xcc, 0x02, 0xcc, 0x03, 0xcc, 0x04, 0xcc, 0x05, 0xcc, 0x06, 0xcc,
                      0x07, 0xcc, 0x08, 0xcc, 0x09, 0xcc, 0x0a, 0xcc, 0x0b, 0xcc, 0x0c, 0xcc, 0x0d, 0xcc, 0x0e};
    EXPECT_EQ(bytes, expected_bytes);
    bytes.clear();
}