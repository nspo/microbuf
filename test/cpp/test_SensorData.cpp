#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "SensorData.h" // SensorData.mmsg must have been converted before trying to compile this!
#include <iostream>

TEST(microbuf_cpp_SensorData, serialize_then_deserialize)
{
    SensorData_struct_t sensor_data {};
    for(size_t i=0; i<10; ++i)
    {
        sensor_data.distance[i] = i;
        sensor_data.angle[i] = i;
    }
    sensor_data.robot_id = 42U;

    const auto serialized_bytes = sensor_data.as_bytes();

    SensorData_struct_t sensor_data2 {};
    EXPECT_TRUE(sensor_data2.from_bytes(serialized_bytes));
    EXPECT_EQ(sensor_data2.robot_id, 42U);

    using namespace testing;
    EXPECT_THAT(sensor_data2.angle, ElementsAre(0., 1., 2., 3., 4., 5., 6., 7., 8., 9.));
    EXPECT_THAT(sensor_data2.distance, ElementsAre(0., 1., 2., 3., 4., 5., 6., 7., 8., 9.));

    // change a byte so CRC fails
    auto wrong_serialized_bytes = serialized_bytes;
    wrong_serialized_bytes[5] = 0xf0;
    EXPECT_FALSE(sensor_data2.from_bytes(wrong_serialized_bytes));
}