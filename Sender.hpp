#ifndef _SENDER_
#define _SENDER_

#include <string> // Add this for std::string
#include "Ecu.hpp"

class Sender : public Ecu
{
private:
    /* data */
public:
    Sender(const std::string &name, bool available); // Add const and reference
    ~Sender();
    void sendEcuData(const Ecu &ecu);
    void setEcuName(const std::string &name) override;
    std::string getEcuName() const override;
    void setAvailability(bool available) override;
    bool isAvailable() const override;
    uint32_t getValue() const override;
    void setValue(int32_t value) override;
};

#endif // _SENDER_