#include <iostream>
#include "Ecu.hpp"
#include "CanBus.hpp"
#include "Sender.hpp"
#include "Receiver.hpp"

#include <vector>

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
void createNewEcu(Ecu &ecu)
{
    std::cout << "Creating a new ECU...\n"
              << std::endl;
    std::cout << "tape 1 if you needed as receiver or tape 2 if you needed as a transmitter\n";
    int rep;
    std::cin >> rep;
    std::string name;

    // Example of creating an Ecu object
    switch (rep)
    {
    case 1:
        std::cout << "Creating a Receiver ECU...\n";

        std::cout << "Enter the name for the Receiver ECU: ";
        std::cin >> name;
        ;
        ecu = dynamic_cast<Receiver &>(ecu);
        ecu = Receiver(name, true);
        std::cout << "Receiver ECU created with name: " << ecu.getEcuName() << std::endl;
        break;
    case 2:
        std::cout << "Creating a Sender ECU...\n";
        std::cout << "Enter the name for the Sender ECU: ";
        std::cin >> name;
        ecu = dynamic_cast<Sender &>(ecu);
        ecu = Sender(name, true);
        std::cout << "Sender ECU created with name: " << ecu.getEcuName() << std::endl;

        break;

    default:
        break;
    };
}

void runSimulation(Ecu &ecu)
{
}

void showResults(const Ecu &ecu)
{
    std::cout << "ECU Name: " << ecu.getEcuName() << std::endl;
    std::cout << "Availability: " << (ecu.isAvailable() ? "Available" : "Not Available") << std::endl;
}

int main()
{ // Create concrete implementation
    std::vector<Ecu> ecuGroup;
    // Use reference to Idf

    initializer(); // Initialize the simulator controller

    return 0;
}