#include <iostream>
#include <cstdint>
#include "./headers/CanBus.hpp"
#ifdef __linux__
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <linux/can.h>
#include <linux/can/raw.h>
#include <net/if.h>
#include <sys/time.h>  // For timeval
#include <cerrno>      // For errno
#endif

CanBus::CanBus() {
#ifdef __linux__
    socket_fd = -1;
#endif
}

CanBus::~CanBus() {
    closeSocket();
}

bool CanBus::createVCAN() {
#ifdef __linux__
    int ret = system("sudo modprobe vcan");
    if (ret != 0) {
        std::cerr << "Error loading vcan module\n";
        return false;
    }
    ret = system("sudo ip link add dev vcan0 type vcan");
    if (ret != 0) {
        std::cerr << "Error creating vcan0 interface\n";
        return false;
    }
    ret = system("sudo ip link set up vcan0");
    if (ret != 0) {
        std::cerr << "Error activating vcan0\n";
        return false;
    }
    std::cout << "vcan0 created and activated\n";
    return true;
#else
    std::cerr << "VCAN only supported on Linux\n";
    return false;
#endif
}

bool CanBus::init() {
#ifdef __linux__
    if ((socket_fd = socket(PF_CAN, SOCK_RAW, CAN_RAW)) < 0) {
        perror("Socket creation failed");
        return false;
    }

    // Set receive timeout to 100ms
    struct timeval tv;
    tv.tv_sec = 0;
    tv.tv_usec = 100000;  // 100ms
    if (setsockopt(socket_fd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv)) < 0) {
        perror("setsockopt SO_RCVTIMEO failed");
        close(socket_fd);
        return false;
    }

    struct ifreq ifr;
    std::strcpy(ifr.ifr_name, "vcan0");
    if (ioctl(socket_fd, SIOCGIFINDEX, &ifr) < 0) {
        perror("IOCTL failed");
        close(socket_fd);
        return false;
    }

    struct sockaddr_can addr;
    addr.can_family = AF_CAN;
    addr.can_ifindex = ifr.ifr_ifindex;

    if (bind(socket_fd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        perror("Bind failed");
        close(socket_fd);
        return false;
    }

    std::cout << "CAN socket initialized\n";
    return true;
#else
    std::cerr << "CAN init only supported on Linux\n";
    return false;
#endif
}

void CanBus::send(Can &frame) {
#ifdef __linux__
    if (socket_fd < 0) {
        std::cerr << "CAN socket not initialized\n";
        return;
    }

    struct can_frame raw_frame;
    raw_frame.can_id = frame.getId();
    if (frame.isExtendedFrame()) {
        raw_frame.can_id |= CAN_EFF_FLAG;
    }
    
    const auto& data = frame.getData();
    raw_frame.can_dlc = data.size();
    std::copy(data.begin(), data.end(), raw_frame.data);
    
    if (write(socket_fd, &raw_frame, sizeof(raw_frame)) != sizeof(raw_frame)) {
        perror("CAN write error");
    } else {
        std::cout << "Sent CAN frame ID: 0x" << std::hex << frame.getId() 
                  << " DLC: " << std::dec << data.size() << "\n";
    }
#endif
}

Can CanBus::receiveFrame() {
#ifdef __linux__
    if (socket_fd < 0) {
        return Can();
    }

    struct can_frame raw_frame;
    ssize_t nbytes = read(socket_fd, &raw_frame, sizeof(raw_frame));
    
    if (nbytes < 0) {
        if (errno == EAGAIN || errno == EWOULDBLOCK) {
            // Timeout occurred, no data available
            return Can();
        }
        perror("CAN read error");
        return Can();
    }
    
    if (nbytes != sizeof(raw_frame)) {
        std::cerr << "Incomplete CAN frame\n";
        return Can();
    }
    
    uint32_t id = raw_frame.can_id;
    bool isExtended = (id & CAN_EFF_FLAG) ? true : false;
    id = isExtended ? (id & CAN_EFF_MASK) : (id & CAN_SFF_MASK);
    
    std::vector<uint8_t> data(raw_frame.data, raw_frame.data + raw_frame.can_dlc);
    
    Can frame(id, data);
    frame.setFrameType(isExtended);
    return frame;
#else
    return Can();
#endif
}

void CanBus::closeSocket() {
#ifdef __linux__
    if (socket_fd >= 0) {
        close(socket_fd);
        socket_fd = -1;
        std::cout << "CAN socket closed\n";
    }
#endif
}