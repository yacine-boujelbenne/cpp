#include <iostream>
#include "Ecu.hpp"
#include "CanBus.hpp"
#include "Sender.hpp"
#include "Receiver.hpp"
#include <memory>
#include <vector>

int main()
{ // Create concrete implementation
    std::vector<Ecu> ecuGroup;
    // Use reference to Idf

    initializer(); // Initialize the simulator controller

    return 0;
}
void initializer()
{
    std::cout << "Before we start we need to get ready and prepare our SimulatorController!\n"
              << std::endl;
    std::cout << "Creating the BusManager...\n"
              << std::endl;

    int rep;
    std::cout << "Please enter the mode (1 for receive mode, 2 for send mode): \n";
    std::cin >> rep;

    std::cout << "SimulatorController initialized.\n"
              << std::endl;
}
void createNewEcu()
{
    std::cout << "Creating a new ECU...\n"
              << std::endl;
    std::cout << "tape 1 if you needed as receiver or tape 2 if you needed as a transmitter\n";
    int rep;
    std::cin >> rep;

    std::unique_ptr<Ecu> ecu;
    switch (rep)
    {
    case 1:
        ecu = std::make_unique<Receiver>();
        break;
    case 2:
        ecu = std::make_unique<Sender>();
        break;

    default:
        break;
    };
}

void runSimulation(Ecu &ecu)
{
    // Simulate some operations on the ECU
    ecu.setEcuName("ECU1");
    ecu.setAvailability(true);

    std::cout << "Running simulation for ECU: " << ecu.getEcuName() << std::endl;
}

void showResults(const Ecu &ecu)
{
    std::cout << "ECU Name: " << ecu.getEcuName() << std::endl;
    std::cout << "Availability: " << (ecu.isAvailable() ? "Available" : "Not Available") << std::endl;
}
