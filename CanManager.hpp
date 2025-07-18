
#ifndef _CANMANAGER_HPP
#define _CANMANAGER_HPP
#include "Can.hpp"
#include <vector>
#include <cstdint>

class CanManager
{
public:
    virtual void sendFrame(uint32_t id, const std::vector<uint8_t> &data) = 0;
    virtual void receiveFrame() = 0;

    virtual ~CanManager() = default; // Always add virtual destructor for base classes
};

#endif