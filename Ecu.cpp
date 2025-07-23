#include <iostream>
#include "Ecu.hpp"

Ecu::Ecu(const std::string &name, bool available)
    : ECUName(name), availability(available)
{
    std::cout << "Ecu constructor called with name: " << ECUName << std::endl;
}

Ecu::~Ecu()
{
    std::cout << "Ecu destructor called for: " << ECUName << std::endl;
}

std::string Ecu::getEcuName() const
{
    return ECUName;
}
void Ecu::setEcuName(const std::string &name)
{
    ECUName = name;
}
bool Ecu::isAvailable() const
{
    return availability;
}
void Ecu::setAvailability(bool available)
{
    availability = available;
}
uint32_t Ecu::getValue() const
{
    // Placeholder implementation, should be overridden in derived classes
    return 0;
}
void Ecu::setValue(int32_t value)
{
    // Placeholder implementation, should be overridden in derived classes
    std::cout << "Setting value in Ecu base class is not implemented." << std::endl;
}