#include <iostream>
#include <memory>
#include <vector>
#include "CanBus.hpp"
#include "Ecu.hpp"
#include <time.h>
#include "CanManager.hpp"
#include "BusManager.hpp"
#include "TransportProtocol.hpp"
#include "CanTp.hpp"

#ifndef _SIMULATION_MANAGER_HPP
#define _SIMULATION_MANAGER_HPP

class SimulationManager
{
    long silenceTime = time(nullptr);
    // std::vector<std::unique_ptr<CanTpSession>> nodes;

public:
    SimulationManager() = default;
    ~SimulationManager() = default;
    void addNode(uint32_t txId, uint32_t rxId);
    void runSimulation(Ecu &ecu);
    Ecu *createNewEcu(Ecu *ecu, CanManager &canManager);
    void initializer();
    void showResults(const Ecu &ecu);
    int main();

    CanManager &cm;        // Reference to CanManager for sending/receiving data
    TransportProtocol &tp; // Reference to TransportProtocol for CAN communication
    Ecu &ecu1;             // Reference to Ecu for simulation
    BusManager &bus;       // Reference to BusManager for CAN bus management
};

#endif // _SIMULATION_MANAGER_HPP