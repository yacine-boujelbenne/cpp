#ifndef _CAN_
#define _CAN_

#include <cstdint>
#include <queue>
#include <stdexcept>
#include <vector>
#include "CanManager.hpp"
#include <iostream>
#include "TransportProtocol.hpp"

class TransportProtocol; // Forward declaration of TransportProtocol class
// Forward declaration of CanManager class
class Can : public CanManager
{
public:
    Can();
    Can(uint32_t id, const std::vector<uint8_t> &data);
    virtual ~Can();

    void sendFrame(const std::vector<uint8_t> &data) override;
    std::string receiveFrame(TransportProtocol &tp, CanManager &cm) override;

    const std::vector<uint8_t> &getData() const;
    const uint32_t getId() const; // Make this const
    void print();

    static std::vector<uint8_t> encoder(const std::string &data);

private:
    uint32_t id_;                            // CAN ID (11-bit or 29-bit)
    std::vector<uint8_t> data_;              // Data payload (0–8 bytes for standard CAN)
    static const size_t MAX_DATA_LENGTH = 8; // Standard CAN payload limit
};

#endif