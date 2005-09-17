#include "Vehicle.hpp"
#include <iostream>

int main()
{
    
    Vehicle* vehicle = new Vehicle("eu07");
    
    Device* dv1 = vehicle->addDevice("dv1");
    dv1->addVariable("b", 2.0);
    dv1->addVariable("a", 10.0);
    dv1->addVariable("e", 10.0);
    dv1->addExpression(string("x"), string("b/e*(a+c)"));
    dv1->addExpression(string("y"), string("a+c*d"));
        
    Device* dv2 = vehicle->addDevice("dv2");
    dv2->addExpression(string("n"), string("1.12*3.7+2.2"));
    dv2->addExpression(string("o"), string("1/2/2"));

    vehicle->generateAsm();
    
    vehicle->update();
    cout << "after:" << endl;
    vehicle->printVariables();
    
    cout << "bool size: " << sizeof(bool) << endl;
    
    return 1;
     
};
