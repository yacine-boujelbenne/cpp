#include <iostream>
#include "Ecu.hpp"
#include "CanBus.hpp"
#include "SimulationManager.hpp"
#include "Receiver.hpp"
#include "Sender.hpp"
#include <vector>
#include <memory>
#include <cstdint>
#include <typeinfo>
#include "CanTp.hpp"
#include "CanManager.hpp" // Added for CanManager
#include "BusManager.hpp"
#include "Can.hpp"

void SimulationManager::initializer()
{
    std::cout << "Initializing Simulation Manager...\n"
              << std::endl;
    // Set the bus manager for the transport protocol (handled via constructor)
}

Ecu *SimulationManager::createNewEcu(Ecu *ecu, CanManager &canManager)
{
    std::cout << "Creating a new ECU...\n"
              << std::endl;
    std::string name;
    std::cout << "Enter the name for the ECU: \n";
    std::cin >> name;
    std::cout << "Type 1 for receiver or 2 for transmitter\n";
    int rep;
    std::cin >> rep;

    if (rep == 1)
    {
        std::cout << "Creating a Receiver ECU...\n";
        Receiver *ecu1 = new Receiver(name, true); // Dynamic allocation
        ecu = ecu1;                                // Assign to output pointer
        std::cout << "Receiver ECU created with name: " << name << std::endl;
    }
    else if (rep == 2)
    {
        std::cout << "Creating a Sender ECU...\n";
        Sender *ecu2 = new Sender(name, true); // Dynamic allocation
        ecu = ecu2;                            // Assign to output pointer
        std::cout << "Sender ECU created with name: " << name << std::endl;
    }
    else
    {
        std::cout << "Invalid choice, no ECU created.\n"
                  << std::endl;
        ecu = nullptr;
    }
    return ecu;
}

void SimulationManager::runSimulation(Ecu &ecu)
{

    if (typeid(ecu) == typeid(Receiver))
    {
        std::cout << "\nRunning simulation for Receiver ECU: " << ecu.getEcuName() << std::endl;
        Receiver &exEcu = dynamic_cast<Receiver &>(ecu);
        std::string receivedMessage = exEcu.receiveEcuData(&exEcu, &tp, this->cm); // Pass references
        if (!receivedMessage.empty())
        {
            std::cout << "Received message: " << receivedMessage << std::endl;
        }
        else
        {
            std::cout << "No message received." << std::endl;
        }
    }
    else if (typeid(ecu) == typeid(Sender))
    {
        std::cout << "\nRunning simulation for Sender ECU: " << ecu.getEcuName() << std::endl;
        Sender &exEcu = dynamic_cast<Sender &>(ecu);
        std::string message;
        std::cout << "Enter message to send: \n";
        std::cin >> message;

        std::cout << "Select The available CanManager protocol : \n -1- Can" << std::endl;
        int rep;
        std::cin >> rep;
        if (rep == 1)
        {
            Can cm(0x1234, Ecu::encoder(message));
            this->cm = cm;
        }
        std::cout << "Select The available Transport protocol : \n -1- CanTp" << std::endl;
        rep = 0;
        std::cin >> rep;
        if (rep == 1)
        {
            CanTp ct(0x123, 0x234, this->bus);
            this->tp = ct;
        }

        exEcu.sendEcuData(message, this->tp, this->cm); // Pass by value or reference as needed
        std::cout << "Message sent: " << message << std::endl;
    }
    else
    {
        std::cerr << "Unknown ECU type.\n"
                  << std::endl;
    }
}

void SimulationManager::showResults(const Ecu &ecu)
{
    std::cout << "ECU Name: " << ecu.getEcuName() << std::endl;
    std::cout << "Availability: " << (ecu.isAvailable() ? "Available" : "Not Available") << std::endl;
}

// Main function added
int main()
{
    SimulationManager *simManager;
    std::cout << "Select The available Bus protocol : \n -1- CanBus" << std::endl;
    int repi;
    std::cin >> repi;
    if (repi = 1)
    {
        std::cout << "CanBus 3";

        simManager->bus = *new CanBus();
    }

    // Create and simulate an ECU
    Ecu *ecu = nullptr;

    ecu = simManager->createNewEcu(ecu, simManager->cm); // Use simManager's CanManager
    std::cout << ecu->getEcuName();
    simManager->runSimulation(*ecu);

    // Clean up resources
    delete &simManager->cm;  // Delete Can instance (careful with pointer management)
    delete &simManager->bus; // Delete CanBus instance

    return 0;
}