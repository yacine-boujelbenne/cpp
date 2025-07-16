#include <cstdint>
#include <string>
#ifndef _ECU_
#define _ECU_

class ECU
{
private:
    std::string ECUName;
    std::string identifier;
    bool availability;

public:
    Ecu();
    ~Ecu();

    virtual uint32_t getValue() = 0;
    virtual void setValue(uint32_t id, int32_t value) = 0;
};

auto ECU::Ecu()
{
}

#endif // _ECU_