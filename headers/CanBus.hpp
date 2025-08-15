#ifndef _CANBUS_HPP
#define _CANBUS_HPP

#include "Can.hpp"

class CanBus {
public:
    CanBus();
    ~CanBus();

    bool createVCAN();
    bool init();
    void closeSocket();
    void send(Can &frame);
    Can receiveFrame();

private:
#ifdef __linux__
    int socket_fd = -1;
#endif
};

#endif