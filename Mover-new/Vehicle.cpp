#include "Vehicle.hpp"
#include <iostream>

void Vehicle::generateAsm()
{
    
    Assembler* x86 = new Assembler();    
    
    optimize();

    x86->push(ebx);
    x86->mov(ebx, dword_ptr[&data]);
    x86->finit();

    if(!childs.size()) return;
    
    vector<void*>::iterator iter;
    for(iter = childs.begin(); iter != childs.end(); iter++) ((Device*)*iter)->generateAsm(x86);
   
    x86->pop(ebx);
    x86->ret();
    
	update = (void(*)())x86->callable();    
    
};

Device* Vehicle::addDevice(string name)
{
    
    Device* obj = new Device(name, this);
    addChild((DataContainer*) obj);
    return obj;
    
};
