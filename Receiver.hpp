#ifndef _RECEIVER_
#define _RECEIVER_
#include "Ecu.hpp"
#include <iostream>
#include <cstdint>
#include "CanManager.hpp"

class Receiver : public Ecu
{
private:
    /* data */
public:
    Receiver(std::string name, bool available);
    ~Receiver();
    std::string receiveEcuData(Ecu *ecu, TransportProtocol *tp, CanManager &can_manager);
    // Override Ecu methods
    void setEcuName(const std::string &name) override;
    std::string getEcuName() const override;
    void setAvailability(bool available) override;
    bool isAvailable() const override;
    uint32_t getValue() const override;
    void setValue(int32_t value) override;
};

#endif
