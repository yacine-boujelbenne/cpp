#ifndef _SENDER_
#define _SENDER_

#include <string> // Add this for std::string

#include <cstdint>

#include "CanTp.hpp"

class Sender 
{
private:
    std::string ECUName;
    bool availability;
public:
    Sender(const std::string &name, bool available); // Add const and reference
    ~Sender();
    void sendEcuData(std::string &msg, CanTp &tp);
    void setEcuName(const std::string &name) ;
    std::string getEcuName() const ;
    void setAvailability(bool available) ;
    bool isAvailable() const ;
    uint32_t getValue() const ;
    void setValue(int32_t value) ;
};

#endif // _SENDER_