#ifndef _CANTP_
#define _CANTP_
#include "TransportProtocol.hpp"
#include <cstdint>
#include <vector>
#include <string>
#include "Can.hpp"

class CanManager; // Forward declaration of CanManager class

class CanTp : public TransportProtocol
{
private:
public:
    CanTp(uint32_t txId, uint32_t rxId);
    void sendMessage(const std::string &message);
    void sendSingleFrame(const std::string &message);
    void sendMultiFrame(const std::string &message);
    std::string receiveMessage(const std::vector<Can> &frames);
    ~CanTp() = default; // Default destructor
    void sendMessageP(const std::string &message, TransportProtocol &transportprotocol) override;
    std::string receiveMessageP(TransportProtocol &transportprotocol, CanManager &canManager) override;
    static const size_t MAX_SF_DATA; // Maximum data length for Single Frame
    static const size_t MAX_CF_DATA; // Maximum data length for Consecutive Frame
    static const size_t MAX_FC_DATA; // Maximum data length for Flow Control
    uint32_t txId_;
    uint32_t rxId_;
};

#endif