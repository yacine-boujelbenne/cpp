#ifndef _SENDER_
#define _SENDER_
#include "Ecu.hpp"

class Sender : public Ecu
{
private:
    /* data */
public:
    Sender();
    ~Sender();
};

Sender::Sender() : Ecu("Receiver", "RX")
{
}

Sender::~Sender()
{
}

#endif
