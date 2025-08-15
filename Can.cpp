
#include <vector>

#include <iostream>



#include <iomanip>

#include <cstdint>
#include "./headers/Can.hpp"




Can::Can() : id_(0), data_()
{
}

Can::Can(uint32_t id, const std::vector<uint8_t> &data) 
{
    this->id_=id;
    this->data_= data;

    /*if (data_.size() > MAX_DATA_LENGTH)
    {
        std::cout << "Data size exceeds maximum CAN frame length";
    }*/
}


void Can::print()
{
    std::cout << "CAN Frame: ID = 0x" << std::hex << std::setw(3) << std::setfill('0') << id_
              << ", Data = ";
    for (uint8_t byte : data_)
    {
        std::cout << std::hex << std::setw(2) << std::setfill('0') << (int)byte << " ";
    }
    std::cout << std::dec << std::endl;
}
/*

void Can::sendFrame(CanTp &tp)
{
    std::cout << "The can start sending";
    if (data_.size() > 8)
    {
        std::cout << "Data size exceeds maximum CAN frame length";
        // Implementation will be handled by BusManager
    }
    // tp.sendMessageP(Ecu::decoder(data)); // Use TransportProtocol to send the frame


}

//this receive methode eleminates the usage of the transport protocol
void Can::receiveFrame(CanTp &tp)
{
    this->setData(Can::encoder(tp.receiveMessage()));
    
}*/