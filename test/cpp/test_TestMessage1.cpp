#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "TestMessage1.h"
#include <iostream>
#include <fstream>

TEST(microbuf_cpp_TestMessage1, serialize_then_deserialize)
{
    TestMessage1_struct_t msg{};
    msg.bool_val = true;
    msg.uint8_val = 123U;
    msg.uint16_val = 123U;
    msg.uint32_val = 123U;
    msg.uint64_val = 123U;
    for(int8_t i=0; i<10; ++i)
    {
        msg.float32_arr_val[i] = i;
        msg.float64_arr_val[i] = i;
    }


    const auto serialized_bytes = msg.as_bytes();

    TestMessage1_struct_t msg2{};
    EXPECT_TRUE(msg2.from_bytes(msg.as_bytes()));

    using namespace testing;
    EXPECT_EQ(msg2.bool_val, true);
    EXPECT_EQ(msg2.uint8_val, 123U);
    EXPECT_EQ(msg2.uint16_val, 123U);
    EXPECT_EQ(msg2.uint32_val, 123U);
    EXPECT_EQ(msg2.uint64_val, 123U);

    EXPECT_THAT(msg2.float32_arr_val, ElementsAre(0., 1., 2., 3., 4., 5., 6., 7., 8., 9.));
    EXPECT_THAT(msg2.float64_arr_val, ElementsAre(0., 1., 2., 3., 4., 5., 6., 7., 8., 9.));

    // change a byte so CRC fails
    auto wrong_serialized_bytes = serialized_bytes;
    wrong_serialized_bytes[5] = 0xf0;
    EXPECT_FALSE(msg2.from_bytes(wrong_serialized_bytes));

    // print output to console so it can be used in MATLAB test
    if (true) { // NOLINT
        const std::string filename = "TestMessage1_serialized.m";
        std::cout << "Printing content of TestMessage1 for MATLAB to file " << filename << "\n";
        std::ofstream ofs(filename);
        if (!ofs) {
            std::cerr << "Cannot write to "<<filename<< " - aborting";
            return;
        }
        ofs << "bytes = uint8([";
        const auto bytes = msg.as_bytes();
        bool first = true;
        for(const auto byte : bytes) {
            if (first) first = false;
            else ofs << ", ";

            ofs << static_cast<int>(byte);
        }
        ofs<<"]);\n";
    }
}