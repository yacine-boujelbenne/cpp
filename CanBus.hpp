
#include "BusManager.hpp"
#include <cstdint>
#include "CanManager.hpp"

#ifndef _CANBUS_HPP
#define _CANBUS_HPP

struct
{
    uint8_t flag;
    uint8_t flag2;
    uint8_t flag3;

} typedef CanFlag;

class CanBus : public BusManager
{
public:
    CanBus();  // Default constructor
    ~CanBus(); // Default destructor

    bool createVCAN();
    bool init() override;
    void closeSocket();
    void send(const CanManager &trame) override;
    CanManager *receive() override;
    Can receiveFrame();

private:
#ifdef __linux__
    int socket_fd = -1; // Descripteur du socket CAN
#endif
};

#endif
