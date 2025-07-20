#ifndef _RECEIVER_
#define _RECEIVER_
#include "Ecu.hpp"

class Receiver : public Ecu
{
private:
    /* data */
public:
    Receiver();
    ~Receiver();
};

Receiver::Receiver() : Ecu("Receiver", "RX")
{
}

Receiver::~Receiver()
{
}

#endif
