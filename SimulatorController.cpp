#include <iostream>
#include "Ecu.hpp"
#include "Idf.hpp"
#include <memory>
#include <vector>

int main()
{
    const std::unique_ptr<Idf> idf1 = std::make_unique<Standartf>(); // Create concrete implementation
    std::vector<Ecu> ecuGroup;                                       // Using vector for dynamic array of Ecu objects
    Idf &idf = *idf1;                                                // Use reference to Idf

    initializer(); // Initialize the simulator controller

    return 0;
}
void initializer()
{
    std::cout << "Before we start we need to get ready and prepare our SimulatorController!" << std::endl;
    std::cout << "Creating the BusManager..." << std::endl;

    int rep;
    std::cout << "Please enter the mode (1 for receive mode, 2 for send mode): ";
    std::cin >> rep;
    switch (rep)
    {
    case 1:
        break;
    case 2:
        break;

    default:
        break;
    }

    std::cout << "SimulatorController initialized." << std::endl;
}

void runSimulation(Ecu &ecu)
{
    // Simulate some operations on the ECU
    ecu.setEcuName("ECU1");
    ecu.setAvailability(true);
    ecu.setValue(ecu.getIdf(), 42); // Set some value in the IDF

    std::cout << "Running simulation for ECU: " << ecu.getEcuName() << std::endl;
}

void showResults(const Ecu &ecu)
{
    std::cout << "ECU Name: " << ecu.getEcuName() << std::endl;
    std::cout << "Availability: " << (ecu.isAvailable() ? "Available" : "Not Available") << std::endl;
    std::cout << "IDF Value: " << ecu.getIdf().getValue() << std::endl;
}
