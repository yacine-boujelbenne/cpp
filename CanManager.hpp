
#ifndef _CANMANAGER_HPP
#define _CANMANAGER_HPP
#include "Can.hpp"
#include <vector>
#include <cstdint>
#include "TransportProtocol.hpp" // if TransportProtocol is used
#include "Can.hpp"               // if Can is used
class TransportProtocol;         // Forward declaration of TransportProtocol class
class CanManager
{
public:
    CanManager() = default;  // Default constructor
    ~CanManager() = default; // Default destructor
    virtual void sendFrame(Can canMessage) = 0;
    virtual std::string receiveFrame(TransportProtocol &transportProtocol, CanManager &canManager) = 0;
};

#endif