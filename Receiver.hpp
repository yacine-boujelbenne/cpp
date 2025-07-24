#ifndef _RECEIVER_
#define _RECEIVER_
#include "Ecu.hpp"
#include <iostream>

class Receiver : public Ecu
{
private:
    /* data */
public:
    Receiver(std::string name, bool available);
    ~Receiver();
    void receiveEcuData(const Ecu &ecu);
    // Override Ecu methods
    void setEcuName(const std::string &name) override;
    std::string getEcuName() const override;
    void setAvailability(bool available) override;
    bool isAvailable() const override;
    uint32_t getValue() const override;
    void setValue(int32_t value) override;
};

#endif
