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

void SimulationManager::initializer()
{
    std::cout << "Initializing Simulation Manager...\n"
              << std::endl;
    Ecu *ecu1 = nullptr;
    this->createNewEcu(ecu1, cm); // Pass cm by reference
    if (ecu1)
    {
        std::cout << ecu1->getEcuName() << " created successfully.\n"
                  << std::endl;
    }
    else
    {
        std::cerr << "Failed to create ECU.\n"
                  << std::endl;
    }
    cm = *new Can();               // Create a CanManager instance (assuming Can is meant here)
    bus = *new CanBus();           // Create a CanBus instance
    tp = CanTp(0x123, 0x456, bus); // Pass CanBus as BusManager reference
    // Set the bus manager for the transport protocol (handled via constructor)
}

void SimulationManager::createNewEcu(Ecu *ecu, CanManager &canManager)
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
}

void SimulationManager::runSimulation(Ecu &ecu)
{
    if (typeid(ecu) == typeid(Receiver))
    {
        std::cout << "Running simulation for Receiver ECU: " << ecu.getEcuName() << std::endl;
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
        std::cout << "Running simulation for Sender ECU: " << ecu.getEcuName() << std::endl;
        Sender &exEcu = dynamic_cast<Sender &>(ecu);
        std::string message;
        std::cout << "Enter message to send: \n";
        std::cin >> message;
        exEcu.sendEcuData(message, tp, cm); // Pass by value or reference as needed
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

    // Create and simulate an ECU
    Ecu *ecu = nullptr;
    simManager->createNewEcu(ecu, simManager->cm); // Use simManager's CanManager
    if (ecu)
    {
        simManager->runSimulation(*ecu); // Run simulation with the created ECU
        simManager->showResults(*ecu);   // Show results
        delete ecu;                      // Clean up dynamically allocated ECU
    }
    else
    {
        std::cerr << "No ECU created to simulate.\n"
                  << std::endl;
    }

    // Clean up resources
    delete &simManager->cm;  // Delete Can instance (careful with pointer management)
    delete &simManager->bus; // Delete CanBus instance

    return 0;
}