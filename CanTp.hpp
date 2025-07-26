#ifndef _CANTP_
#define _CANTP_
#include "TransportProtocol.hpp"
#include <cstdint>
#include <vector>
#include <string>
#include "Can.hpp"
/*
struct singleFrame
{
    uint32_t id;
    uint8_t data[8];
    bool ack;
};
typedef singleFrame singleFrame;

struct firstFrame
{
    uint32_t id;
    uint8_t data[8];
    bool ack;
};
typedef firstFrame firstFrame;

struct consectiveFrame
{
    uint32_t id;
    uint8_t data[8];
    bool ack;
};
typedef consectiveFrame consectiveFrame;

struct flowControl
{
    uint32_t id;
    uint8_t data[8];
    bool ack;
};
typedef flowControl flowControl;

*/
//------------------------------
/*struct
{
    flowControl flowctr;
    consectiveFrame consectivefrm;
    firstFrame firstfrm;
    singleFrame snglfrm;

} typedef CanTpFrame;*/
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