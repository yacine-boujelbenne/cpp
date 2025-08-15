#include <gtest/gtest.h>
#include "../headers/Sender.hpp"
#include "../headers/CanTp.hpp"
#include "../headers/CanBus.hpp"

TEST(SenderTest, BasicFunctionality)
{
    Sender sender("TestSender", true);
    CanBus bus;
    CanTp tp(0x700, 0x700, bus);

    EXPECT_EQ(sender.getEcuName(), "TestSender");
    EXPECT_TRUE(sender.isAvailable());

    // Test send functionality
    testing::internal::CaptureStdout();
    std::string msg = "Hello";
    sender.sendEcuData(msg, tp);
    std::string output = testing::internal::GetCapturedStdout();

    EXPECT_NE(output.find("Sending data from Sender ECU: TestSender"), std::string::npos);
    EXPECT_NE(output.find("ECU is available. Sent data: Hello"), std::string::npos);
}