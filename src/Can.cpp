
#include <vector>

#include <iostream>

#include <iostream>
#include <iomanip>

#include <iomanip>

#include <cstdint>
#include "../headers/Can.hpp"




Can::Can() : id_(0), data_()
{
}

Can::Can(uint32_t id, const std::vector<uint8_t> &data) 
{
    this->id_=id;
    this->data_= data;
}


void Can::print()
{
    std::cout << "      ~> CAN Frame: (+) ID = 0x" << std::hex << std::setw(3) << std::setfill('0') << id_
              << "  (+) DLC = "<< data_.size()<<", (+) Data = ";
    for (uint8_t byte : data_)
    {
        std::cout << std::hex << std::setw(2) << std::setfill('0') << (int)byte << " ";
    }
    std::cout << std::dec << std::endl;
}
