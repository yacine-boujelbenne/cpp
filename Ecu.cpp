#include <iostream>
#include "Ecu.hpp"
#include "Idf.hpp"

Ecu::Ecu(const std::string &name, const Idf &id, bool available)
    : ECUName(name), idf(const_cast<Idf &>(id)), availability(available)
{
    std::cout << "Ecu constructor called with name: " << ECUName << std::endl;
}