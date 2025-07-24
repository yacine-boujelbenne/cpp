#include <iostream>
#include "Ecu.hpp"
#include <string>
#include "Receiver.hpp"

Receiver::Receiver(std::string name, bool available)
    : Ecu(name, available)
{
    std::cout << "Receiver constructor called with name: " << getEcuName() << std::endl;
}

Receiver::~Receiver()
{
    std::cout << "Receiver destructor called" << std::endl;
}

void Receiver::receiveEcuData(const Ecu &ecu)
{
    std::cout << "Receiving data for ECU: " << ecu.getEcuName() << std::endl;
    // Simulate receiving data
    if (ecu.isAvailable())
    {
        std::cout << "ECU is available. Received value: " << ecu.getValue() << std::endl;
    }
    else
    {
        std::cout << "ECU is not available. Cannot receive data." << std::endl;
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
    return 0;
}
