#ifndef _VEHICLE_HPP_
#define _VEHICLE_HPP_

#include "Device.hpp"

class Vehicle : public DataContainer
{
    
    public:
  
        Vehicle(string name) : DataContainer(name, NULL) {};
        void (*update)();
        
        void generateAsm();
        Device* addDevice(string name);
        
};

#endif
