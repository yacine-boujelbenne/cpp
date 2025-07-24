#include <stdexcept>
#include <vector>
#include "CanManager.hpp"
#include <iostream>
#include "Can.hpp"
#include "TransportProtocol.hpp"
#include "CanTp.hpp"
#include <iomanip>

Can::Can() : id_(0), data_() {}

Can::Can(uint32_t id, const std::vector<uint8_t> &datat) : id_(id), data_(datat)
{
    if (data_.size() > MAX_DATA_LENGTH)
    {
        throw std::runtime_error("Data size exceeds maximum CAN frame length");
    }
}

std::string receiveFrame(TransportProtocol &tp, CanManager &cm)
{
    std::string receivedData = tp.receiveMessageP(tp, cm);
    return receivedData;
}

Can::~Can()
{
    std::cout << "CAN Frame with ID = 0x" << std::hex << std::setw(3) << std::setfill('0') << id_ << " destroyed." << std::dec << std::endl;
}

const std::vector<uint8_t> &Can::getData() const
{
    return data_;
}

void Can::print()
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

void Can::sendFrame(const std::vector<uint8_t> &data)
{
    if (data.size() > 8)
    {
        throw std::runtime_error("Data size exceeds maximum CAN frame length");
    }
    // Implementation will be handled by BusManager
}

const uint32_t Can::getId() const
{
    return id_;
}

std::string Can::receiveFrame(TransportProtocol &tp, CanManager &cm)
{
    return tp.receiveMessageP(tp, cm);
}