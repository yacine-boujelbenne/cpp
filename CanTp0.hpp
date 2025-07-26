#ifndef _CANTP_
#define _CANTP_
#include "TransportProtocol.hpp"
#include <cstdint>
#include <vector>
#include <string>
#include "Can.hpp"
#include "BusManager.hpp"
class CanBus; // Forward declaration of CanManager class

class CanTp : public TransportProtocol
{
private:
public:
    CanTp::CanTp(uint32_t txId, uint32_t rxId, BusManager &busManager);
    void sendMessage(const std::string &message);
    void sendSingleFrame(const std::string &message);
    void sendMultiFrame(const std::string &message);
    std::string receiveMessage(const std::vector<Can> &frames);
    ~CanTp() = default; // Default destructor
    void sendMessageP(const std::string &message, TransportProtocol &transportprotocol) override;
    std::string receiveMessageP(CanBus &canBus, Ecu &ecu) override;
    static const size_t MAX_SF_DATA; // Maximum data length for Single Frame
    static const size_t MAX_CF_DATA; // Maximum data length for Consecutive Frame
    static const size_t MAX_FC_DATA; // Maximum data length for Flow Control
    uint32_t txId_;
    uint32_t rxId_;
    BusManager &busManager_;

    std::vector<uint8_t> CreateFlowControlFrame(uint8_t flowStatus, uint8_t blockSize, uint8_t separationTime);
    std::vector<uint8_t> CreateConsecutiveFrame(const std::vector<uint8_t> &data, uint8_t sequenceNumber);
    std::vector<uint8_t> CreateFirstFrame(const std::vector<uint8_t> &data, uint16_t dataSize);
    std::vector<uint8_t> CreateSingleFrame(const std::vector<uint8_t> &data, uint8_t dataSize);
    std::vector<uint8_t> GetDataFromFirstFrame(const std::vector<uint8_t> &data);
    std::vector<uint8_t> GetDataFromConsecutiveFrame(const std::vector<uint8_t> &data);
    std::vector<uint8_t> GetDataFromSingleFrame(const std::vector<uint8_t> &data);
    uint8_t GetBlockSizeFromFlowControlFrame(const std::vector<uint8_t> &data);
    uint8_t GetFlagFromFlowControlFrame(const std::vector<uint8_t> &data);
};

#endif