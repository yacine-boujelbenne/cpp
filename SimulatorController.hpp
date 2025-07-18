#include <iostream>
#include <memory>
#include <vector>
#include "CanBus.hpp"
#ifndef _SIMULATION_MANAGER_HPP
#define _SIMULATION_MANAGER_HPP

class SimulationManager
{
    CanBus bus;
    std::vector<std::unique_ptr<CanTpSession>> nodes;

public:
    void addNode(uint32_t txId, uint32_t rxId)
    {
        nodes.push_back(std::make_unique<CanTpSession>(txId, rxId));
    }
    void run()
    {
        // Simulate event loop with timing
        // Process frames, timeouts, etc.
    }
};

#endif // _SIMULATION_MANAGER_HPP