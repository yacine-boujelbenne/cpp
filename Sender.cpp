#include <iostream>
#include "Ecu.hpp"
#include <String>
#include <cstdint>
#include <vector>
#include "Sender.hpp"

Sender::Sender(std::string name, bool available) : Ecu::Ecu(name, available)
{

    std::cout << "Sender constructor called with name: " << getEcuName() << std::endl;
}

Sender::~Sender()
{
}
void sendEcuData(const Ecu &ecu)

{
    std::cout << "Sending data for ECU: " << ecu.getEcuName() << std::endl;
    // Simulate sending data
    if (ecu.isAvailable())
    {
        std::cout << "ECU is available. Sending value: " << ecu.getValue() << std::endl;
    }
    else
    {
        std::cout << "ECU is not available. Cannot send data." << std::endl;
    }
}