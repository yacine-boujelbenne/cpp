
#include <stdexcept>
#include <vector>
#include "CanManager.hpp"
#include <iostream>
#include "Can.hpp"

Can::Can()
{
    CanFrame frame;
    frame.canId = 0; // Initialize with default values
    frame.dlc = 0;
    frame.data.resize(8, 0); // Resize to 8 bytes with default value
    frameQueue.push(frame);
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