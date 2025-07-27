#ifndef CAN_TP_HPP
#define CAN_TP_HPP

#include <cstdint>
#include <string>
#include <vector>
#include "Can.hpp"
#include "BusManager.hpp"

class TransportProtocol;

class CanTp : public TransportProtocol
{
public:
    CanTp(uint32_t txId, uint32_t rxId, BusManager &busManager);
    CanTp(uint32_t txId, uint32_t rxId);
    void sendMessage(const std::string &message);
    void sendMessageP(const std::string &message) override;
    std::string receiveMessage(const std::vector<Can> &frames);
    std::string receiveMessageP(TransportProtocol &tp, CanManager *cm) override;
    void setbusManager(BusManager &busManager) { busManager_ = busManager; }

    ~CanTp() = default; // Default destructor
private:
    uint32_t txId_;
    uint32_t rxId_;
    BusManager &busManager_;

    static const size_t MAX_SF_DATA;
    static const size_t MAX_CF_DATA;

    // Sending state
    bool sendingMultiFrame = false;
    std::string sendingMessage;
    size_t sendingOffset = 0;
    uint8_t sendingSequence = 1;

    void sendSingleFrame(const std::string &message);
    void sendMultiFrame(const std::string &message);
    void sendNextBlock(uint8_t blockSize);
    void sendFlowControl(uint8_t flowStatus, uint8_t blockSize, uint8_t separationTime);
};

#endif // CAN_TP_HPP