#include <iostream>
#include "Ecu.hpp"
#include "CanBus.hpp"
#include "SimulationManager.hpp"
#include "Receiver.hpp" // Add these includes
#include "Sender.hpp"   // Add these includes
#include <vector>
#include <memory>
#include <cstdint>
#include <typeinfo>
#include "CanTp.hpp"

void SimulationManager::initializer()
{
    std::cout << "Initializing Simulation Manager...\n"
              << std::endl;
    Ecu *ecu1;
    this->createNewEcu(ecu1, cm);
    std::cout << ecu1->getEcuName() << " created successfully.\n"
              << std::endl;
    cm = *new Can();     // Create a CanManager instance
    bus = *new CanBus(); // Create a CanBus instance
    tp = CanTp(0x123, 0x456);
    // Set the bus manager for the transport protocol
}

void SimulationManager::createNewEcu(Ecu *ecu, CanManager &canManager)
{
    std::cout << "Creating a new ECU...\n"
              << std::endl;
    std::string name;
    std::cout << "Enter the name for the Receiver ECU: \n";
    std::cin >> name;
    std::cout << "Type 1 for receiver or 2 for transmitter\n";
    int rep;
    std::cin >> rep;

    if (rep == 1)
    {
        std::cout << "Creating a Receiver ECU...\n";

        // Create a Receiver object
        Receiver ecu1 = Receiver(name, true);
        ecu = &ecu1; // Upcast to Ecu pointer
        std::cout << "Receiver ECU created with name:\n"
                  << name << std::endl;
    }
    else if (rep == 2)
    {
        std::cout << "Creating a Sender ECU...\n";
        // Create a Sender object
        std::cout << "Sender ECU created with name: \n"
                  << name << std::endl;
        Sender ecu2 = Sender(name, true);
        // Upcast to Ecu reference
        ecu = &ecu2; // Upcast to Ecu pointer
    }
}

void SimulationManager::runSimulation(Ecu &ecu)
{

    if (typeid(ecu) == typeid(Receiver))
    {
        std::cout << "Running simulation for Receiver ECU: \n"
                  << ecu.getEcuName() << std::endl;
        Receiver &exEcu = dynamic_cast<Receiver &>(ecu);
        std::string receivedMessage = exEcu.receiveEcuData(&exEcu, &this->tp, this->cm); // Pass CanManager reference
        if (!receivedMessage.empty())
        {
            std::cout << "Received message: \n"
                      << receivedMessage << std::endl;
        }
        else
        {
            std::cout << "No message received." << std::endl;
        }
    }
    else if (typeid(ecu) == typeid(Sender))
    {
        // Derived type match
        std::cout << "Running simulation for Sender ECU: \n"
                  << ecu.getEcuName() << std::endl;
        Sender &exEcu = dynamic_cast<Sender &>(ecu);
        std::string message;
        std::cout << "Enter message to send: \n";
        std::cin >> message;
        exEcu.sendEcuData(message, this->tp, this->cm); // Pass CanManager reference
        std::cout << "start sending message: \n"
                  << message << std::endl;
    }
    else
    {
        std::cerr << "Unknown ECU type.\n"
                  << std::endl;
        return;
    }
}

void SimulationManager::showResults(const Ecu &ecu)
{
    std::cout << "ECU Name: " << ecu.getEcuName() << std::endl;
    std::cout << "Availability: " << (ecu.isAvailable() ? "Available" : "Not Available") << std::endl;
}
