#include <iostream>

#include <vector>
#include "Sender.hpp"
#include <cstdint>
#include "CanManager.hpp"

#include "Ecu.hpp"

Sender::Sender(const std::string &name, bool available) : Ecu(name, available) // Assuming CanManager is a member of Sender class
{

    availability = available; // Initialize availability
    ECUName = name;

    // Initialize CanManager reference

    std::cout << "Sender constructor called with name: " << getEcuName() << std::endl;
}

Sender::~Sender()
{
    std::cout << "Sender destructor called" << std::endl;
}
void Sender::sendEcuData(std::string &msg, TransportProtocol &tp, CanManager &can_manager)
{
    std::cout << "Sending data from Sender ECU: " << getEcuName() << std::endl;
    // Simulate sending data
    if (isAvailable())
    {
        std::cout << "ECU is available. Sent data: " << msg << std::endl;
        can_manager.sendFrame(Ecu::encoder(msg), tp); // Assuming CanManager has an encoder method

        std::cout << "Ecu sender has sent the data to the CanManager\n";
    }
    else
    {
        std::cout << "ECU is not available. Cannot send data." << std::endl;
    }
}
void Sender::setEcuName(const std::string &name)
{
    ECUName = name;
    std::cout << "Sender ECU name set to: " << ECUName << std::endl;
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
