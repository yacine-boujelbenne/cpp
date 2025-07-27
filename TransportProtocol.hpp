#ifndef _TRANSPORTPROTOCOL_HPP
#define _TRANSPORTPROTOCOL_HPP

#include <iostream>
#include <vector>
#include <cstdint>
// <-- REQUIRED for std::string
// <-- REQUIRED if you use CanManager&

class CanBus; // Forward declaration of Can class
class Ecu;    // Forward declaration of Ecu class

class TransportProtocol
{
public:
    TransportProtocol() = default;
    virtual ~TransportProtocol() = default;
    virtual void sendMessageP(const std::string &message) = 0;
    virtual std::string receiveMessageP(TransportProtocol &tp, CanManager *cm) = 0;
    uint32_t txId__;
    uint32_t rxId__;
};

#endif // _TRANSPORTPROTOCOL_HPP