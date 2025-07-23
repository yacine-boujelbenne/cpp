#ifndef _SENDER_
#define _SENDER_
#include "Ecu.hpp"

class Sender : public Ecu
{
private:
    /* data */
public:
    Sender(std::string name, bool available);
    ~Sender();
    void setEcuName(const std::string &name) override;
    std::string getEcuName() const override;
    void setAvailability(bool available) override;
    bool isAvailable() const override;
    uint32_t getValue() const override;
    void setValue(int32_t value) override;
};

#endif
