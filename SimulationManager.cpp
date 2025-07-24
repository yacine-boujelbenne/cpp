#include <iostream>
#include "Ecu.hpp"
#include "CanBus.hpp"
#include "SimulationManager.hpp"
#include "Receiver.hpp" // Add these includes
#include "Sender.hpp"   // Add these includes
#include <vector>
#include <memory>
void SimulationManager::initializer()
{
    std::cout << "Initializing Simulation Manager...\n"
              << std::endl;
}

void SimulationManager::createNewEcu(Ecu &ecu)
{
    std::cout << "Creating a new ECU...\n"
              << std::endl;
    std::cout << "Type 1 for receiver or 2 for transmitter\n";
    int rep;
    std::cin >> rep;
    std::string name;

    switch (rep)
    {
    case 1:
        std::cout << "Creating a Receiver ECU...\n";
        std::cout << "Enter the name for the Receiver ECU: ";
        std::cin >> name;

        if (auto *receiver = dynamic_cast<Receiver *>(&ecu))
        {
            receiver->setEcuName(name);
            receiver->setAvailability(true);
        }
        else
        {
            ecu = Receiver(name, true);
        }
        std::cout << "Receiver ECU created with name: " << ecu.getEcuName() << std::endl;
        break;

    case 2:
        std::cout << "Creating a Sender ECU...\n";
        std::cout << "Enter the name for the Sender ECU: ";
        std::cin >> name;

        if (auto *sender = dynamic_cast<Sender *>(&ecu))
        {
            sender->setEcuName(name);
            sender->setAvailability(true);
        }
        else
        {
            ecu = Sender(name, true);
        }
        std::cout << "Sender ECU created with name: " << ecu.getEcuName() << std::endl;
        break;

    default:
        std::cout << "Invalid choice\n";
        break;
    }
}

// ...existing code...

void SimulationManager::runSimulation(Ecu &ecu)
{
}

void SimulationManager::showResults(const Ecu &ecu)
{
    std::cout << "ECU Name: " << ecu.getEcuName() << std::endl;
    std::cout << "Availability: " << (ecu.isAvailable() ? "Available" : "Not Available") << std::endl;
}
