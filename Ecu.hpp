#ifndef _ECU_
#define _ECU_
#include "BusManager.hpp"
#include <iostream>
#include <cstdint>
struct data
{
    // Define the structure of data that will be used in the ECU
    uint32_t id;
    uint8_t data[8]; // Example for 8 bytes of data
};
typedef data Data;

class Ecu
{
protected:
    std::string ECUName;
    bool availability;
    // Reference to CanManager for sending/receiving data

public:
    Ecu(const std::string &name, bool available)
        : ECUName(name), availability(available)
    {
        // Initialize CanManager reference
        availability = available; // Initialize availability
        ECUName = name;
        std::cout << "Ecu constructor called with name: " << ECUName << std::endl;
    }
    virtual ~Ecu() = default;
    virtual void setEcuName(const std::string &name) = 0;
    virtual std::string getEcuName() const = 0;
    virtual void setAvailability(bool available) = 0;
    virtual bool isAvailable() const = 0;
    virtual uint32_t getValue() const = 0;
    virtual void setValue(int32_t value) = 0;
    static std::vector<uint8_t> encoder(const std::string &data)
    {
        return std::vector<uint8_t>(data.begin(), data.end());
    }
    static std::string decoder(const std::vector<uint8_t> &data)
    {
        return std::string(data.begin(), data.end());
    }
};

#endif // _ECU_