#ifndef BUSMANAGER_HPP
#define BUSMANAGER_HPP

#include <vector>
#include <iostream>
class CanManager; // Forward declaration of CanManager class

class Can; // Forward declaration

class BusManager
{
public:
    BusManager() = default;
    ~BusManager() = default;

    virtual bool init() = 0;
    virtual void send(const CanManager &frame);
    virtual CanManager *receive();
};
#endif // BUSMANAGER_HPP