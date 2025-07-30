#include "SimulationManager.hpp"
#include "CanBus.hpp"
#include "CanTp.hpp"
#include <iostream>
#include "CanTp.hpp"
#include "Ecu.hpp"
#include "Sender.hpp"

Ecu *SimulationManager::createNewEcu(Ecu *ecu, CanManager &canManager)
{
    // Implementation for creating new ECU
    return nullptr;
}

void SimulationManager::runSimulation(Ecu &exEcu)
{
    // Implementation for running simulation
    std::string message = "Test message";
    if (true) // Simplified condition for demonstration
    {
        CanBus *bs = new CanBus(); // Allocate on heap
        if (bs->createVCAN() && bs->init())
        {
            std::cout << "Message sent:11111\n ";
            CanTp ct(0x123, 0x234, bs); // Pass pointer
            this->tp = ct;
        }
        else
        {
            std::cerr << "Failed to initialize CanBus for simulation." << std::endl;
            delete bs; // Clean up if initialization fails
            return;
        }
    }
    Sender ecuu(exEcu.getEcuName(), exEcu.isAvailable());

    ecuu.sendEcuData(message, this->tp, this->cm); // Pass by value or reference as needed
    std::cout << "Message sent: " << message << std::endl;
}
/*
int main()
{
    // This main function should ideally be in Main.cpp, not SimulationManager.cpp
    // For now, it's here to allow compilation of SimulationManager.cpp
    return 0;
}
*/