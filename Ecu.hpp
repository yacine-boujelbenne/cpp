#include <cstdint>
#include <string>
#include "Idf.hpp"
#ifndef _ECU_
#define _ECU_

class Ecu
{
private:
    std::string ECUName;
    Idf &idf;
    bool availability;

public:
    Ecu::Ecu(const std::string &name, const Idf &id, bool available);
    Ecu::~Ecu();
    virtual void setEcuName(const std::string &name) = 0;
    virtual std::string getEcuName() const = 0;
    virtual void setAvailability(bool available) = 0;
    virtual bool isAvailable() const = 0;
    virtual void setIdf(Idf &id) = 0;
    virtual Idf &getIdf() const = 0;
    virtual uint32_t getValue() = 0;
    virtual void setValue(Idf &id, int32_t value) = 0;
};

#endif // _ECU_