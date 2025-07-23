#include <iostream>
#include "Ecu.hpp"
#include <string>
#include "Receiver.hpp"

Receiver::Receiver(std::string name, bool available) : Ecu::Ecu(name, available)
{
    std::cout << "Receiver constructor called with name: " << getEcuName() << std::endl;
}

void receiveEcuData(const Ecu &ecu)
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
