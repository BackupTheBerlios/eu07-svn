#ifndef _EXPRESSION_HPP_
#define _EXPRESSION_HPP_

#include <string>
#include <vector>
#include <stack>
#include <deque>

#include "DataContainer.hpp"

using namespace std;

typedef struct
{
        
    char token;
    int stack, input;
        
} priorItem_t;

typedef enum {VARIABLE, CONST, OPERATOR, FUNCTION, SPECIAL} exprItemType_t;

typedef struct
{

    exprItemType_t type;    
    int* offset;
 
    priorItem_t prior;    
    
} exprItem_t;

typedef stack<exprItem_t*> exprItemStack_t;
typedef deque<exprItem_t*> exprItemDeque_t;

class Expression : public DataContainer
{
    
    private:

        int* result;        
        string expr;
        
        exprItemStack_t eiStack;
        exprItemDeque_t eiResult;
        
        void parse();
        
    public:
        
        Expression(string result, DataContainer* parent, string expr) : DataContainer(string(), parent)
        {

            this->expr = expr;
            this->result = addVariable(result);            
            parse();
           
        };
        
        void generateAsm(Assembler* x86);
        
};

#endif
