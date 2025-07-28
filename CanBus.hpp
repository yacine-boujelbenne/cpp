#include "BusManager.hpp"

#include <cstdint>

#include "Can.hpp"

#ifndef _CANBUS_HPP
#define _CANBUS_HPP

class CanManager;

class CanBus : public BusManager
{
public:
    CanBus();  // Default constructor
    ~CanBus(); // Default destructor

    bool createVCAN() override;
    bool init() override;
    void closeSocket() override;
    void send(const Can &trame) override;
    void receive() override;

    CanManager *receiveFrame();
};

#endif
