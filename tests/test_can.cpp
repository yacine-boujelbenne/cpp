#include <gtest/gtest.h>
#include "../headers/Can.hpp"

TEST(CanTest, ConstructorAndGetters)
{
    std::vector<uint8_t> data = {0x01, 0x02, 0x03};
    Can frame(0x123, data);

    EXPECT_EQ(frame.getId(), 0x123);
    ASSERT_EQ(frame.getData().size(), 3);
    EXPECT_EQ(frame.getData()[0], 0x01);
    EXPECT_EQ(frame.getData()[1], 0x02);
    EXPECT_EQ(frame.getData()[2], 0x03);
}

TEST(CanTest, EncoderDecoder)
{
    std::string test_str = "Hello CAN!";
    auto encoded = Can::encoder(test_str);
    std::string decoded = Can::decoder(encoded);

    EXPECT_EQ(decoded, test_str);
}