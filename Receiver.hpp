#ifndef _RECEIVER_
#define _RECEIVER_


#include <cstdint>
#include "CanTp.hpp"

class Receiver 
{
private:
std::string ECUName;
bool availability;
public:
    Receiver(std::string name, bool available);
    ~Receiver();
    void receiveEcuData(CanTp &tp);
    // Override Ecu methods
    void setEcuName(const std::string &name);
    std::string getEcuName() const;
    void setAvailability(bool available);
    bool isAvailable() const;
    uint32_t getValue() const;
    void setValue(int32_t value);
};

#endif
