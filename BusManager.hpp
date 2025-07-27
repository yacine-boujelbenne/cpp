#ifndef BUSMANAGER_HPP
#define BUSMANAGER_HPP

#include <vector>
#include <iostream>

class Can; // Forward declaration

class BusManager
{
public:
    BusManager() = default;
    ~BusManager() = default;

    virtual bool init() = 0;
    virtual void send(const Can &frame);
    virtual CanManager *receive();
};
#endif // BUSMANAGER_HPP