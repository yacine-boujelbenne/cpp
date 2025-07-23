#ifndef _TRANSPORTPROTOCOL_HPP
#define _TRANSPORTPROTOCOL_HPP

#include <cstdint>
#include <vector>
#include "BusManager.hpp"
#include <string>         // <-- REQUIRED for std::string
#include "CanManager.hpp" // <-- REQUIRED if you use CanManager&
class CanManager;         // Forward declaration of Can class

class TransportProtocol
{
public:
    virtual void sendMessageP(const std::string &message, TransportProtocol &transportprotocol) = 0;
    virtual std::string receiveMessageP(TransportProtocol &transportprotocol, CanManager &canManager) = 0;
    TransportProtocol() {}
    ~TransportProtocol() {}
    uint32_t txId__;
    uint32_t rxId__;
};

#endif // _TRANSPORTPROTOCOL_HPP