#ifndef BUSMANAGER_HPP
#define BUSMANAGER_HPP

#include <vector>
#include <iostream>
#include <cstdio>

class CanManager; // Forward declaration
class Can;

class BusManager
{
public:
    BusManager() = default;
    virtual ~BusManager() = default;
    virtual bool createVCAN() = 0;
    virtual bool init() = 0;
    virtual void send(const Can &frame) = 0;
    virtual void receive() = 0;
    virtual void closeSocket() = 0;
    void setcanMan(CanManager *cm)
    {
        this->canmanager = cm;
    }
    CanManager *getcanMan()
    {
        return this->canmanager;
    }

private:
    CanManager *canmanager = nullptr;
};
#endif // BUSMANAGER_HPP