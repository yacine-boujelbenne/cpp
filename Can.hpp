#include <queue>
#include <stdexcept>
#include <vector>
#include "CanManager.hpp"
#include <iostream>

struct
{
    uint32_t canId;            // 11-bit or 29-bit CAN ID
    std::vector<uint8_t> data; // Up to 8 bytes for standard CAN
    uint8_t dlc;               // Data Length Code
};
typedef CanFrame CanFrame;

class Can : public CanManager
{
    std::queue<CanFrame> frameQueue;

public:
    Can::Can() = default;
    Can::~Can() = default;
    void sendFrame(uint32_t id, const std::vector<uint8_t> &data) override;
    void receiveFrame() override;
};