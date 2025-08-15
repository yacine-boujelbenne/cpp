#ifndef CAN_TP_HPP
#define CAN_TP_HPP

#include <cstdint>
#include <string>

#include "CanBus.hpp"

class CanTp 
{
public:
    CanTp(uint32_t txId, uint32_t rxId, CanBus &bus);
    CanTp(uint32_t txId, uint32_t rxId);
    ~CanTp();

    void sendMessage(const std::string &message);
    void sendMessageP(const std::string &message);
    std::string receiveMessage();

    void setbusManager(CanBus &bus) { busTp_ = bus; }
    CanBus &getbus() { return busTp_; }

private:
    std::string receiveCon(int& retryCount);
    void sendSingleFrame(const std::string &message);
    void sendMultiFrame(bool first);
    void sendNextBlock(uint8_t blockSize);
    void sendFlowControl(uint8_t flowStatus, uint8_t blockSize, 
                         uint8_t separationTime, uint32_t targetId);

    uint32_t txId_;
    uint32_t rxId_;
    CanBus& busTp_;
    bool ownsBus_;

    // Sending state variables
    bool sendingMultiFrame = false;
    std::string sendingMessage;
    size_t sendingOffset = 0;
    uint8_t sendingSequence = 1;

    // Receiving state variables
    bool receivingMultiFrame = false;
    size_t receivingTotalLength = 0;
    size_t receivingOffset = 0;
    uint8_t receivingSequence = 1;
    std::vector<uint8_t> receivingBuffer;
};

#endif // CAN_TP_HPP