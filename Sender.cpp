#include <iostream>

#include <vector>
#include "Sender.hpp"
#include <cstdint>

class Ecu; // Forward declaration of CanManager class

Sender::Sender(const std::string &name, bool available)
: Ecu(name, available) // Remove redundant Ecu::
{
    std::cout << "Sender constructor called with name: " << getEcuName() << std::endl;
}

Sender::~Sender()
{
    std::cout << "Sender destructor called" << std::endl;
}
void Sender::sendEcuData(const Ecu &ecu)

{
    std::cout << "Sending data for ECU: " << ecu.getEcuName() << std::endl;
    // Simulate sending data
    if (ecu.isAvailable())
    {
        std::cout << "ECU is available. Sending value: " << static_cast<int>(ecu.getValue()) << std::endl;
    }
    else
    {
        std::cout << "ECU is not available. Cannot send data." << std::endl;
    }
}
void Sender::setEcuName(const std::string &name)
{
}
std::string Sender::getEcuName() const
{
    return ECUName;
}
void Sender::setAvailability(bool available)
{
    availability = available;
}
bool Sender::isAvailable() const
{
    return availability;
}
uint32_t Sender::getValue() const
{
    // Placeholder implementation, should be overridden in derived classes
    return 0x00000000;
}
void Sender::setValue(int32_t value)
{
    // Implement your specific logic here
    std::cout << "Setting value in Sender: " << value << std::endl;
}
