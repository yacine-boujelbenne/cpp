#include <iostream>
#include "Ecu.hpp"

Ecu::Ecu(const std::string &name, bool available)
    : ECUName(name), availability(available)
{
    std::cout << "Ecu constructor called with name: " << ECUName << std::endl;
}