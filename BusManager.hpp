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
    ~BusManager() = default;

    virtual bool init() = 0;
    virtual void send(const Can &frame);
    virtual void receive();
    void setcanMan(CanManager *cm)
    {
        this->canmanager = cm;
    }
    CanManager *getcanMan()
    {
        return this->canmanager;
    }

private:
    CanManager *canmanager;
};
#endif // BUSMANAGER_HPP