#include <queue>
#include <stdexcept>
#include "Can.hpp"
#include "CanManager.hpp"
#include <iostream>

Can::Can()
{
    // Constructor implementation
}

void Can::sendFrame(uint32_t id, const std::vector<uint8_t> &data)
{
    std::cout << "Sending CAN Frame with ID: " << std::hex << id << "\nData: ";
    if (data.size() > 8)
    {
        throw std::runtime_error("Data size exceeds maximum allowed for CAN frame");
    }
    for (auto byte : data)
    {
        std::cout << std::hex << static_cast<int>(byte) << " ";
    }
    std::cout << std::dec << std::endl;
}
void Can::receiveFrame()
{
    std::cout << "Receiving CAN Frame (simulation)\n"
              << std::endl;
    // Simulate receiving a frame
    throw std::runtime_error("No frames available");
}