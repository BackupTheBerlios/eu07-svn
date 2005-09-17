#include "DataContainer.hpp"
#include <iostream>

DataContainer::DataContainer(string cName, DataContainer* parent)
{
    
    if(cName.size())
    {
        
        name = cName;
        name.append(".");
        
    };
    
    this->parent = parent;
    
    maxOffset = 0;
    constCount = 0;
    
};

int* DataContainer::addVariable(string vName, double value)
{
    
    if(name.size())
    {
        
        vName.insert(0, name);
        
    };
    
    if(parent) return parent->addVariable(vName, value);
    
    maxOffset++;    
    
    values_t::iterator iter;
    valuesData_t* valueData = new valuesData_t;
    valuesEntry_t entry;
    
    valueData->name = vName;
    valueData->offset = maxOffset;
    valueData->orgOffset = maxOffset;    
    valueData->isReference = false;
    valueData->value = value;
    
    iter = values.find(vName);
    if(iter == values.end())
    {

        valuesEntry_t value(vName, valueData);    
        values.insert(value);
        
        return & valueData->offset;
        
    } else {
        
        return & iter->second->offset;
        
    };
    
};

int* DataContainer::addVariable(string vName)
{
    
    return addVariable(vName, 0.0);
    
};

int* DataContainer::addConst(double value)
{
    
    if(parent) return parent->addConst(value);
    
    string vName("const");
    vName.append(1, (char) constCount + 0x30);
    constCount++;
    
    return addVariable(vName, value);
    
};

void DataContainer::addChild(DataContainer* child)
{
    
    if(!child) return;
    childs.push_back((void*) child);
    
};

DataContainer* DataContainer::addChild(string cName)
{
    
    DataContainer* child = new DataContainer(cName, this);
    childs.push_back(child);
    return child;
    
};

bool DataContainer::connect(string from, string to)
{
    
    if(name.size())
    {
        
        from.insert(0, name);
        to.insert(0, name);
        
    };
    
    if(parent) return parent->connect(from, to);
    
    values_t::iterator iter;
    iter = values.find(from);
    
    if(iter == values.end()) return false;
    
    valuesData_t* fromItem;
    fromItem = iter->second;
    
    iter = values.find(to);
    if(iter == values.end()) return false;
    
    valuesData_t* toItem;        
    toItem = iter->second;

    toItem->offset = fromItem->offset;
    toItem->isReference = true;
    
    fromItem->references.push_back(toItem);
    
};   
        
void DataContainer::optimize()
{
    
    if(parent) return;
    
    deque<int> offsets;    
    deque<int>::iterator oIter;    
    values_t::iterator vIter;
    references_t::iterator rIter;
    valuesData_t* vData;
    double* ptr;
    int offset;
    
    for(vIter = values.begin(); vIter != values.end(); vIter++)
    {
        
        vData = vIter->second;
        
        if(vData->isReference)
        {
            
            offsets.push_front(vData->orgOffset);
            
        } else {
            
            if(!offsets.size()) continue;
            
            offsets.push_front(vData->offset);            
            vData->offset = offset = offsets.back();
            if(vData->references.size())
            {
                
                for(rIter = vData->references.begin(); rIter != vData->references.end(); rIter++)
                {
                    
                    ((valuesData_t*) *rIter)->offset = offset;
                    
                };
                
            };
            
            if(offset > maxOffset) maxOffset = offset;
            offsets.pop_back();
            
        };
        
    };
    
    data = (double*) malloc((maxOffset+1) * 8);
    
    for(vIter = values.begin(); vIter != values.end(); vIter++)
    {

        vData = vIter->second;    
        vData->offset = (int) data + (vData->offset - 1) * 8;
        
        if(!vData->isReference) *(double*)vData->offset = vData->value;
        
    };
    
    cout << "before:" << endl;
    printVariables();
    
};

void DataContainer::generateAsm(Assembler* x86)
{
    
    if(!childs.size()) return;
    
    vector<void*>::iterator iter;
    for(iter = childs.begin(); iter != childs.end(); iter++) ((DataContainer*)*iter)->generateAsm(x86);
    
};

void DataContainer::printVariable(string name)
{
    
    values_t::iterator vIter;
    
    vIter = values.find(name);
    if(vIter == values.end()) { cout << name << ": undefined" << endl; return; };
    
    cout << name << ": " << *(data+vIter->second->offset) << endl;

};

void DataContainer::printVariables()
{
    
    values_t::iterator vIter;
    
    for(vIter = values.begin(); vIter != values.end(); vIter++)
        cout << vIter->second->name << ": " << *(double*)vIter->second->offset << endl;
    
};
    
void DataContainer::print(bool onlyReferences)
{
    
    values_t::iterator vIter;
    valuesData_t* vData;
    
    for(vIter = values.begin(); vIter != values.end(); vIter++)
    {
        
        vData = vIter->second;
        
        if(onlyReferences && vData->isReference) continue;
        cout << vData->name << " " << vData->offset << endl;
        
    };
    
    cout << "size: " << maxOffset << ":" << values.size() << endl;
    
};
