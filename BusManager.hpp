
#ifndef _BUSMANAGER_HPP
#define _BUSMANAGER_HPP
#include <cstddef>
#include <cstdint>

class BusManager

{
private:
public:
    void brodcastMessage(uint32_t id, const uint8_t *data, size_t length);
    void acceptMessage(uint32_t id, uint8_t *data, size_t &length);
    BusManager();
    ~BusManager();
};

#endif
