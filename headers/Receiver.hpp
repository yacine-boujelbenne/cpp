#ifndef _RECEIVER_
#define _RECEIVER_


#include <cstdint>
#include <string>
#include "CanTp.hpp"

class Receiver 
{
private:
std::string ECUName;
bool availability;
std::string message;
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
    void setmessage(std::string m){
        this->message = m;
    }
    std::string getmessage(){
        return this->message;
    }
};

#endif
