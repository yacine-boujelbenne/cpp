#include <gtest/gtest.h>
#include "../headers/CanBus.hpp"

TEST(CanBusTest, VirtualInterfaceCreation)
{
    CanBus bus;
    EXPECT_TRUE(bus.createVCAN());
}

TEST(CanBusTest, SendReceiveSimulation)
{
    CanBus bus;
    bus.createVCAN();
    bus.init();

    Can send_frame(0x123, {0xAA, 0xBB});
    bus.send(send_frame);

    Can receive_frame = bus.receiveFrame();
    EXPECT_NE(receive_frame.getId(), 0); // Should not be empty frame
}