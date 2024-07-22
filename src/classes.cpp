#include "classes.hpp"

using namespace std;
using namespace classes;

// ----- Class Exp:

Exp::Exp(ExpType expType) : expType(expType) {}

Exp::Exp(const Exp *operand, const Node *operatorNode)
{
    if (operand->expType == BOOLEAN && operatorNode.isBoolean())
    {
        this->expType = BOOLEAN;
    }
    else
    {
        /* TODO: Handle Error*/
        exit(0);
    }
}
Exp::Exp(const Exp *operand1, const Exp *operand2, const Node *operatorNode)
{
    switch (operatorNode->operatortype())
    {
    case LOGIC:
        /* code */
        break;
    
    case RELOP:
        /* code */
        break;
    
    case ARITHMETIC:
        /* code */
        break;
    
    default:
        /* TODO: Handle Error*/
        exit(0);
        break;
    }

    if (operatorNode.isLogicOperator())
    {
        /* code */
    }
    
    if ((operand1->expType == operand2->expType == BOOLEAN) && operatorNode.isBoolean())
    {
        this->expType = BOOLEAN;
    }
    else
    {
        /* TODO: Handle Error*/
        exit(0);
    }
}

// -----