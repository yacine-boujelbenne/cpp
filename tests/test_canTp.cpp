#include <gtest/gtest.h>
#include "../headers/CanTp.hpp"
#include "../headers/CanBus.hpp"

class CanTpTest : public ::testing::Test
{
protected:
    CanBus bus;

    void SetUp() override
    {
        bus.createVCAN();
        bus.init();
    }
};

TEST_F(CanTpTest, SingleFrameTransmission)
{
    CanTp tp(0x700, 0x700, bus);
    std::string message = "Test";
    tp.sendMessage(message);

    std::string received = tp.receiveMessage();
    EXPECT_EQ(received, message);
}

TEST_F(CanTpTest, MultiFrameTransmission)
{
    CanTp tp(0x700, 0x700, bus);
    std::string long_message(50, 'X'); // 50-character message

    tp.sendMessage(long_message);
    std::string received = tp.receiveMessage();

    EXPECT_EQ(received, long_message);
}