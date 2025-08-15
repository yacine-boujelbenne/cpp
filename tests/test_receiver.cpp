#include <gtest/gtest.h>
#include "../headers/Receiver.hpp"

TEST(ReceiverTest, BasicFunctionality)
{
    Receiver receiver("TestReceiver", true);

    EXPECT_EQ(receiver.getEcuName(), "TestReceiver");
    EXPECT_TRUE(receiver.isAvailable());

    receiver.setAvailability(false);
    EXPECT_FALSE(receiver.isAvailable());
}