#include <iostream>
#include <memory>
#include <vector>
#include "CanBus.hpp"
#include "Ecu.hpp"
#include <time.h>
#include <cstdint>

#ifndef _SIMULATION_MANAGER_HPP
#define _SIMULATION_MANAGER_HPP

class SimulationManager
{
    CanBus bus;
    long silenceTime = time(nullptr);
    // std::vector<std::unique_ptr<CanTpSession>> nodes;

public:
    SimulationManager() = default;
    ~SimulationManager() = default;
    void addNode(uint32_t txId, uint32_t rxId);
    void runSimulation(Ecu &ecu);
    void createNewEcu(Ecu &ecu);
    void initializer();
    void showResults(const Ecu &ecu);
};

#endif // _SIMULATION_MANAGER_HPP