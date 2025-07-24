
#include "BusManager.hpp"
#include <cstdint>

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
    CanBus() = default;  // Default constructor
    ~CanBus() = default; // Default destructor
    void initializer();
};

#endif
