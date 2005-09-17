#include "Vehicle.hpp"
#include <iostream>

void Vehicle::generateAsm()
{
    
    Assembler* x86 = new Assembler();    
    
    optimize();
    
    x86->finit();

    if(!childs.size()) return;
    
    vector<void*>::iterator iter;
    for(iter = childs.begin(); iter != childs.end(); iter++) ((Device*)*iter)->generateAsm(x86);
   
    x86->ret();
    
	update = (void(*)())x86->callable();    
    
//    if(update) cout << x86->getListing() << endl;
    
};

Device* Vehicle::addDevice(string name)
{
    
    Device* obj = new Device(name, this);
    addChild((DataContainer*) obj);
    return obj;
    
};
