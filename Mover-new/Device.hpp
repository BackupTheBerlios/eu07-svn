#ifndef _DEVICE_HPP_
#define _DEVICE_HPP_
#include "Expression.hpp"

class Device : public DataContainer
{
    
    public:
        
        Device(string name, DataContainer* parent) : DataContainer(name, parent) { };
        
//        int* addVariable(string vName);
//        int* addVariable(string vName, double value);
//        int* addConst(double value);
        void addExpression(string result, string expr);
        void generateAsm(Assembler* x86);
    
};

#endif
