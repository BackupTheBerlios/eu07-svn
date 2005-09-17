#ifndef _DATACONTAINER_H_
#define _DATACONTAINER_H_

#include <vector>
#include <stack>
#include <map>
#include <string>

#include "softwire/CodeGenerator.hpp"

using namespace std;
using namespace SoftWire;

typedef vector<void*> references_t;

typedef struct
{

    string name;
    int offset;
    int orgOffset;
    double value;
    bool isReference;
    references_t references;
    
} valuesData_t;

typedef pair<string, valuesData_t*> valuesEntry_t;
typedef map<string, valuesData_t*> values_t;

class DataContainer // : public CodeGenerator
{
    
    protected:
        
        string name;
        DataContainer* parent;
        vector<void*> childs;
        int maxOffset;
        int constCount;        
        
        values_t values;
        double* data;
        
    public:
        
        DataContainer(string cName, DataContainer* parent); // constructor                

        void optimize();        
        void generateAsm(Assembler* x86); 
        void print(bool onlyReferences);
    
        int* addVariable(string vName); // return reference to offset
        int* addVariable(string vName, double value);
        int* addConst(double value);
                
        DataContainer* addChild(string cName);
        void addChild(DataContainer*);
        void printVariable(string name);
        void printVariables();
        
        bool connect(string from, string to); // connect variable from to variable to
        
};
#endif    
