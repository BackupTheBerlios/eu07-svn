#include "Expression.hpp"
#include <iostream>

using namespace std;
using namespace SoftWire;

priorItem_t tokenPriority[] =
{
    
    {'#', 0, 0},
    {'-', 3, 3},
    {'+', 3, 3},
    {'*', 4, 4},
    {'/', 4, 4},
    {'^', 5, 5},
    {'v', 9, 9},
    {'c', 9, 9},
    {'f', 2, 9},    
    {'(', 2, 9},
    {')', 9, 2}
    
};

char opers[] = "+-/*^";
char special[] = "()";

priorItem_t getTokenPriority(char token)
{
    
    int i, l = sizeof(tokenPriority) / sizeof(priorItem_t);
    for(i=0; i<l; i++) if(tokenPriority[i].token == token) return tokenPriority[i];
    
};

bool chrInSet(char chr, char* set)
{
    
    while(*set)
    {
        
        if(chr == *set) return true;
        set++;
        
    };
    
    return false;
    
};

void strToLower(string &str)
{
    
    string::iterator iter;
    
    for(iter = str.begin(); iter != str.end(); iter++) if(*iter >= 'A' && *iter <= 'Z') *iter = *iter + 0x20;
    
};

void Expression::parse()
{

    bool found;    
    double val;
    exprItem_t* item;
    
    string::iterator str;
    string::iterator tokStart;
    
    string vName;
    
    strToLower(expr);
    
    item = new exprItem_t; item->type = SPECIAL; item->prior = getTokenPriority('#');
    eiStack.push(item);
    
    str = expr.begin();
    while(str != expr.end())
    {
        
        if(*str >= '0' && *str <= '9')
        {
            
            tokStart = str;
            while(str != expr.end())
            {
        
                if(!((*str >= '0' && *str <= '9') || *str == '.')) break;
                str++;  
        
            };
            
            val = atof(string(tokStart, str).c_str());
            
            item = new exprItem_t; 
            item->type = CONST; 
            item->offset = addConst(val); 
            item->prior = getTokenPriority('c');
            
        } else if (*str >= 'a' && *str <= 'z') {
            
            tokStart = str;
            while(str != expr.end())
            {
            
                if(!((*str >= 'a' && *str <= 'z') || (*str >= '0' && *str <= '9') || *str == '.')) break;
                str++;
                
            };
            
            item = new exprItem_t; 
            item->type = VARIABLE; 
            item->offset = addVariable(string(tokStart, str)); 
            item->prior = getTokenPriority('v');
               
        } else if(chrInSet(*str, opers)) {
            
            item = new exprItem_t; 
            item->type = OPERATOR; 
            item->prior = getTokenPriority(*str);
            str++;
            
        } else if(chrInSet(*str, special)) {

            item = new exprItem_t; 
            item->type = SPECIAL; 
            item->prior = getTokenPriority(*str);
            str++;
            
        } else {
            
            item = NULL;
            
        };
        
        if(item)
        {

            if(item->prior.input > eiStack.top()->prior.stack)
            {
                
                eiStack.push(item); // push item on stack
                
            } else {
                
                while(eiStack.top()->prior.stack >= item->prior.stack)
                {
                    
                    if(!(eiStack.top()->type == SPECIAL))
                    {
                        
                        eiResult.push_front(eiStack.top()); // push top item from stack to result
                        
                    } else if(eiStack.top()->prior.token == '#') break;
                    
                    eiStack.pop(); // pop item from stack
                    
                }; // while(stack->prior)
                
                eiStack.push(item);
                
            }; // if(item->prior) .. else
            
        }; // if(item)
                                       
    }; // while(*expr)
    
    while(eiStack.size())
    {
        
        if(!(eiStack.top()->type == SPECIAL))
        {
               
            eiResult.push_front(eiStack.top()); // push item on result
                        
        } else if(eiStack.top()->prior.token == '#') break;
                    
        eiStack.pop(); // pop item from stack
        
    }; // while(*stack)
    
};

void Expression::generateAsm(Assembler* x86)
{
    
    exprItem_t* item;
    
    while(!eiResult.empty())
    {
        
        item = eiResult.back();
        eiResult.pop_back();
        
        switch(item->type)
        {
            
            case(VARIABLE):
                x86->fld((OperandMEM64) *item->offset);
                break;
                
            case(CONST):
                x86->fld((OperandMEM64) *item->offset);
                break;
                
            case(OPERATOR):
                    
                switch(item->prior.token)
                {
                    
                    case('+'): x86->faddp(); break; // faddp
                    case('-'): x86->fsubp(); break; // fsubp
                    case('*'): x86->fmulp(); break; // fmulp
                    case('/'): x86->fdivp(); break; // fdivp
                    
                };
                break;
                
        };
        
    };
    
    x86->fstp((OperandMEM64)  *result);
    
};
