//#include "DataContainer.hpp"
//#include "Expression.hpp"
#include "Vehicle.hpp"
#include <iostream>

int main()
{
    
    Vehicle* vehicle = new Vehicle("eu07");
    
    Device* dv1 = vehicle->addDevice("dv1");
    dv1->addExpression(string("n"), string("2.11*3.5+2.2"));
    
    Device* dv2 = vehicle->addDevice("dv2");
    dv2->addExpression(string("n"), string("1.12*3.7+2.2"));

    vehicle->generateAsm();
    
    vehicle->update();
    vehicle->printVariables();
    
    return 1;
     
};
