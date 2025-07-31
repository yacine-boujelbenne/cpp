#ifndef CANBUS_HPP
#define CANBUS_HPP

#include "BusManager.hpp"
#include <cstdint>
#include "Can.hpp"

class CanManager;

class CanBus : public BusManager
{
public:
    CanBus();  // Default constructor
    ~CanBus(); // Default destructor

    bool createVCAN() override;
    bool init() override;
    void closeSocket() override;
    void send(const Can &trame) override;
    void receive() override;

    CanManager *receiveFrame();
    bool isSimulationMode() const; // Déclaration de la méthode

private:
#ifdef __linux__
    int socket_fd = -1; // Descripteur du socket CAN
#endif
    bool simulation_mode = false; // Déclaration du membre ici, en dehors du #ifdef
};

#endif // CANBUS_HPP


