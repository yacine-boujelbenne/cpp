
#include <stdexcept>
#include <vector>
#include "CanManager.hpp"
#include <iostream>
#include "Can.hpp"
#include "TransportProtocol.hpp"
#include "CanTp.hpp"
#include <iomanip>

Can::Can(uint32_t id, const std::vector<uint8_t> &data) : id_(id), data_(data)
{ // Store the encoded data

    if (data.size() > MAX_DATA_LENGTH)
    {
        data_.resize(MAX_DATA_LENGTH); // Truncate if too long
        std::cerr << "Warning: Data truncated to " << MAX_DATA_LENGTH << " bytes" << std::endl;
    }
}

void Can::sendFrame(Can canMessage)
{
}

std::string receiveFrame(TransportProtocol &transportProtocol, CanManager &canManager)
{
    std::string receivedData = transportProtocol.receiveMessageP(transportProtocol, canManager);
    return receivedData;
}

uint32_t Can::getId() const
{
    return id_;
}

const std::vector<uint8_t> &Can::getData() const
{
    return data_;
}
void Can::print() const
{
    std::cout << "CAN Frame: ID = 0x" << std::hex << std::setw(3) << std::setfill('0') << id_
              << ", Data = ";
    for (uint8_t byte : data_)
    {
        std::cout << std::hex << std::setw(2) << std::setfill('0') << (int)byte << " ";
    }
    std::cout << std::dec << std::endl;
}
std::vector<uint8_t> Can::encoder(const std::string &data)
{
    return std::vector<uint8_t>(data.begin(), data.end());
}