#ifndef _SENDER_
#define _SENDER_

#include <string> // Add this for std::string
#include "Ecu.hpp"
#include <cstdint>
#include "TransportProtocol.hpp"
#include "CanManager.hpp"

class Sender : public Ecu
{
public:
    Sender(const std::string &name, bool available); // Add const and reference
    ~Sender();
    void sendEcuData(std::string &msg, TransportProtocol &tp, CanManager &can_manager);
    void setEcuName(const std::string &name) override;
    std::string getEcuName() const override;
    void setAvailability(bool available) override;
    bool isAvailable() const override;
    uint32_t getValue() const override;
    void setValue(int32_t value) override;
};

#endif // _SENDER_