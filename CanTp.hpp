#ifndef _CANTP_
#define _CANTP_
#include "TransportProtocol.hpp"

struct
{
    uint32_t id;
    uint8_t data[8];
    bool ack;
};
typedef singleFrame;

struct
{
    uint32_t id;
    uint8_t data[8];
    bool ack;
};
typedef firstFrame;

struct
{
    uint32_t id;
    uint8_t data[8];
    bool ack;
};
typedef consectiveFrame;

struct
{
    uint32_t id;
    uint8_t data[8];
    bool ack;
};
typedef flowControl;

class CanTp : public TransportProtocol
{
private:
public:
    CanTp();
    ~CanTp();
};

CanTp::CanTp()
{
}

CanTp::~CanTp()
{
}

#endif