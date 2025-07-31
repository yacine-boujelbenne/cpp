#include "BusManager.hpp"
#include "Can.hpp"
#include <iostream>
#include <cstdint>
#include "CanManager.hpp"
#include "CanBus.hpp"
#ifdef __linux__
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <linux/can.h>
#include <linux/can/raw.h>
#include <net/if.h>
#endif

CanBus::CanBus()
{
#ifdef __linux__
    socket_fd = -1; // Initialisation du descripteur de socket
#endif
    simulation_mode = false;
}

CanBus::~CanBus()
{
    closeSocket();
}

bool CanBus::isSimulationMode() const
{
    return simulation_mode;
}

bool CanBus::createVCAN()
{
#ifdef __linux__
    int ret = 0;
    ret = system("modprobe vcan");
    if (ret != 0)
    {
        std::cerr << "Erreur modprobe vcan, passage en mode simulation\n";
        simulation_mode = true;
        return true; // Retourner true pour permettre la simulation
    }
    system("ip link delete vcan0 2>/dev/null");
    ret = system("ip link add dev vcan0 type vcan");
    if (ret != 0)
    {
        std::cerr << "Erreur création interface vcan0, passage en mode simulation\n";
        simulation_mode = true;
        return true; // Retourner true pour permettre la simulation
    }
    ret = system("ip link set up vcan0");
    if (ret != 0)
    {
        std::cerr << "Erreur activation interface vcan0, passage en mode simulation\n";
        simulation_mode = true;
        return true; // Retourner true pour permettre la simulation
    }
    std::cout << "Interface vcan0 créée et activée\n";
    return true;
#else
    std::cerr << "createVCAN() only supported on Linux. Mode simulation activé." << std::endl;
    simulation_mode = true;
    return true; // Retourner true pour permettre la simulation
#endif
}

bool CanBus::init()
{
#ifdef __linux__
    if (simulation_mode) {
        std::cout << "Mode simulation activé - pas de socket CAN réel\n";
        return true;
    }
    
    struct ifreq ifr{};
    struct sockaddr_can addr{};

    socket_fd = socket(PF_CAN, SOCK_RAW, CAN_RAW);
    if (socket_fd < 0)
    {
        perror("Erreur création socket CAN, passage en mode simulation");
        simulation_mode = true;
        return true; // Retourner true pour permettre la simulation
    }

    std::strncpy(ifr.ifr_name, "vcan0", IFNAMSIZ - 1);
    if (ioctl(socket_fd, SIOCGIFINDEX, &ifr) < 0)
    {
        perror("Erreur ioctl, passage en mode simulation");
        close(socket_fd);
        socket_fd = -1;
        simulation_mode = true;
        return true; // Retourner true pour permettre la simulation
    }

    addr.can_family = AF_CAN;
    addr.can_ifindex = ifr.ifr_ifindex;

    if (bind(socket_fd, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        perror("Erreur bind, passage en mode simulation");
        close(socket_fd);
        socket_fd = -1;
        simulation_mode = true;
        return true; // Retourner true pour permettre la simulation
    }

    std::cout << "Socket CAN initialisée sur vcan0\n";
    return true;
#else
    std::cerr << "Plateforme non supportée, mode simulation activé\n";
    simulation_mode = true;
    return true; // Retourner true pour permettre la simulation
#endif
}

void CanBus::send(const Can &trame)
{
#ifdef __linux__
    if (simulation_mode || socket_fd < 0)
    {
        std::cout << "Simulation d'envoi de trame CAN (ID: 0x" << std::hex << trame.getId() << std::dec << ")\n";
        return;
    }
    struct can_frame frame{};
    uint32_t raw_id = trame.getId(); // Récupération de l'ID CAN
    frame.can_id = trame.isExtendedFrame() ? (raw_id | CAN_EFF_FLAG) : raw_id;
    frame.can_dlc = trame.getData().size();
    std::memcpy(frame.data, trame.getData().data(), frame.can_dlc);
    if (write(socket_fd, &frame, sizeof(frame)) != sizeof(frame))
    {
        perror("CAN sending error !");
    }
    else
    {
        std::cout << "CAN frame sent (ID: 0x" << std::hex << frame.can_id << std::dec << ", " << (int)frame.can_dlc << " octets)\n";
    }
#else
    std::cout << "Simulation d'envoi de trame CAN (ID: 0x" << std::hex << trame.getId() << std::dec << ")\n";
#endif
}

CanManager *CanBus::receiveFrame()
{
#ifdef __linux__
    if (simulation_mode || socket_fd < 0)
    {
        std::cerr << "Mode simulation - pas de réception CAN réelle\n";
        return nullptr;
    }

    struct can_frame canFrame{};
    ssize_t nbytes = read(socket_fd, &canFrame, sizeof(canFrame));

    if (nbytes < 0)
    {
        perror("Erreur lecture CAN");
        return nullptr;
    }

    if (nbytes < sizeof(canFrame))
    {
        std::cerr << "Trame CAN incomplète reçue\n";
        return nullptr;
    }
    // Dynamically allocate Can object
    Can *frame = new Can(canFrame.can_id, std::vector<uint8_t>(canFrame.data, canFrame.data + canFrame.can_dlc));
    return frame; // Return pointer to dynamically allocated object
#else
    std::cerr << "Mode simulation - pas de réception CAN réelle\n";
    return nullptr;
#endif
}

void CanBus::closeSocket()
{
#ifdef __linux__
    if (socket_fd >= 0)
    {
        close(socket_fd);
        socket_fd = -1;
        std::cout << "Socket CAN fermée\n";
    }
#endif
}

void CanBus::receive()
{
    CanManager *receivedFrame = receiveFrame();
    if (receivedFrame != nullptr)
    {
        BusManager::setcanMan(receivedFrame); // Upcast to base pointer
    }
}

