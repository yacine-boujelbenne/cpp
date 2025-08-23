#include <iostream>
#include <string>
#include <cstdint>


#include "../headers/Receiver.hpp"

Receiver::Receiver(std::string name, bool available)
  
{
    availability = available; // Initialize availability
    ECUName = name;
    // Initialize CanManager reference
    std::cout << "      -> Receiver intiallized with name: " << getEcuName() << std::endl;
}

Receiver::~Receiver()
{
    std::cout << "  " << std::endl;
}

void Receiver::receiveEcuData(CanTp &tp)
{

        std::cout << "      -> ECU is available!"<<std::endl<<"      -> Start receiving...." << std::endl;
        
        this->setmessage(tp.receiveMessage());
        std::cout<<this->getmessage()<<std::endl;
    
    
    if (this->isAvailable()==false)
    {
        std::cout << "      -> ECU is not available. Cannot receive data!" << std::endl;

    }
    
}

void Receiver::setEcuName(const std::string &name)
{
}
std::string Receiver::getEcuName() const
{
    return ECUName;
}
void Receiver::setAvailability(bool available)
{
    availability = available;
}
bool Receiver::isAvailable() const
{
    return availability;
}
uint32_t Receiver::getValue() const
{
    // Placeholder implementation, should be overridden in derived classes
    return 0x00000000;
}
void Receiver::setValue(int32_t value)
{
    // Placeholder implementation, should be overridden in derived classes
    std::cout << "      -> Setting value in Receiver class is not implemented!" << std::endl;
}
