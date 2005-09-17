#include "Device.hpp"

void Device::addExpression(string result, string expr)
{
    
    Expression* obj = new Expression(result, this, expr);
    addChild((DataContainer*) obj);
    
};

void Device::generateAsm(Assembler* x86)
{
    
    if(!childs.size()) return;
    
    vector<void*>::iterator iter;
    for(iter = childs.begin(); iter != childs.end(); iter++) ((Expression*)*iter)->generateAsm(x86);

};    
