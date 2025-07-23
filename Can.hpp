#ifndef _CAN_
#define _CAN_

#include <queue>
#include <stdexcept>
#include <vector>
#include "CanManager.hpp"
#include <iostream>
#include "TransportProtocol.hpp"

/*struct
{
    uint32_t canId;           // 11-bit or 29-bit CAN ID
    std::queue<uint8_t> data; // Up to 8 bytes for standard CAN
    uint8_t dlc;              // Data Length Code
};
typedef CanFrame;*/
class TransportProtocol; // Forward declaration of TransportProtocol class
class Can : public CanManager

{
public:
    Can(uint32_t id, const std::vector<uint8_t> &datat);
    const std::vector<uint8_t> &getData() const;

    ~Can() = default;
    void sendFrame(Can canMessage) override;
    std::string receiveFrame(TransportProtocol &transportProtocol, CanManager &canManager) override;
    void print() const;
    const std::vector<uint8_t> &getData() const;
    uint32_t getId() const;
    static std::vector<uint8_t> encoder(const std::string &data);

private:
    uint32_t id_;                            // CAN ID (11-bit or 29-bit)
    std::vector<uint8_t> data_;              // Data payload (0–8 bytes for standard CAN)
    static const size_t MAX_DATA_LENGTH = 8; // Standard CAN payload limit
};

#endif