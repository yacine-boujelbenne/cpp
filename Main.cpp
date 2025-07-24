#include "Can.hpp"

int main()
{
    Can canDevice;
    std::vector<uint8_t> data = {0x01, 0x02};

    canDevice.sendFrame(data);
    auto frame = canDevice.getData();

    // if you had a TransportProtocol object:
    // std::string msg = canDevice.receiveFrame(tp, canDevice);

    return 0;
}
