
#ifndef _CANMANAGER_HPP
#define _CANMANAGER_HPP
#include <iostream>
#include <vector>
#include <cstdint>
// if TransportProtocol is used

// if Can is used
class TransportProtocol; // Forward declaration of TransportProtocol class
class CanManager
{
public:
    CanManager() = default;
    virtual ~CanManager() = default;
    virtual void sendFrame(const std::vector<uint8_t> &data) = 0;
    virtual std::string receiveFrame(TransportProtocol &tp, CanManager &cm) = 0;
};
#endif

/*    Can frame(Can::getId(), data);
    std::cout << "Sending CAN Frame:" << std::endl;
    frame.print(); // Print the frame details*/