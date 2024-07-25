#include "classes.hpp"
#include "stack.hpp"
#include "parser.ypp"

using namespace std;
using namespace output;
using namespace classes;

// ----- enums
std::string expTypeToString(ExpType type)
{
    switch (type)
    {
    case ExpType::INT:
        return "INT";
    case ExpType::BYTE:
        return "BYTE";
    case ExpType::BOOLEAN:
        return "BOOL";
    default:
        exit(0);
    }
}

// ----- Class Node:

Node::Node() : value("") {};

Node::Node(const std::string & text) : value(text) {}

Node::Node(const Node *other)
{
    if(other)
    {
        this->value = other->value;
    }
    
}

std::string Node::getValue() const
{
    return this->value;
}

// -----

// ----- Class Call:

Call::Call(const Node* func, const Exp* arg)
{
    auto funcIter = FUNCTIONS.find(func->getValue());
    if (funcIter == FUNCTIONS.end())
    {
        // Hadle error - function does not exist
        errorUndefFunc(yylineno, func->getValue());
        exit(0);
    }
    else if (funcIter->second.argTypes.find(arg->expType) == funcIter->second.argTypes.end())
    {
        // Handle error - arg mismatch
        auto x = *funcIter->second.argTypes.begin();
        errorPrototypeMismatch(yylineno, funcIter->first, *funcIter->second.argTypes.begin());
        exit(0);
    }
    else
    {
        this->retType = funcIter->second.retType;
        // TODO: Maybe more code regarding the new scope...
    }
}


// ----- Class Type:

Type::Type(ExpType type) : type(type) {}

ExpType Type::getType() const
{
    return type;
}

bool Type::isNum() const
{
    return this->type == ExpType::INT || this->type == ExpType::BYTE;
}

// -----

// ----- Class Exp:

Exp::Exp(const Exp *other)
{
    if (other)
    {
        this->expType = other->expType;
    }
}

Exp::Exp(const Node* id)
{
    if(id)
    {
        string varName = id->getValue();
        /*
        TODO: Find id by varName and if exists get its type and save in this->expType 
        If does not exist, handle error
        */
    }
}
Exp::Exp(const Call* call)
{
    if (call)
    {
        this->expType = call->retType;
    }
}

Exp::Exp(ExpType expType) : expType(expType) {}

Exp::Exp(ExpType expType, const Node *node)
{
    if (expType == ExpType::BYTE && node && stoi(node->getValue()) < MAX_BYTE)
    {
        this->expType = expType;
    }
    else
    {
        /* TODO: Handle Error*/
    }
}

Exp::Exp(const Exp *operand, OperatorType operatorType)
{
    if (operand && operand->expType == ExpType::BOOLEAN && operatorType == OperatorType::LOGIC)
    {
        this->expType = ExpType::BOOLEAN;
    }
    else
    {
        /* TODO: Handle Error*/
    }
}

Exp::Exp(const Exp *operand1, const Exp *operand2, OperatorType operatorType)
{
    if (operand1 && operand2)
    {
        switch (operatorType)
        {
        case OperatorType::LOGIC:
            if (operand1->expType == ExpType::BOOLEAN && operand2->expType == ExpType::BOOLEAN)
            {
                this->expType = ExpType::BOOLEAN;
                return;
            }
            break;

        case OperatorType::RELOP:
            if (operand1->isNumExp() && operand2->isNumExp())
            {
                this->expType = ExpType::BOOLEAN;
                return;
            }
            break;

        case OperatorType::ARITHMETIC:
            if (operand1->isNumExp() && operand2->isNumExp())
            {
                if (operand1->expType == ExpType::INT || operand2->expType == ExpType::INT)
                {
                    this->expType = ExpType::INT;
                }
                else
                {
                    this->expType = ExpType::BYTE;
                }
                return;
            }
            break;
        default:
            break;
        }
    }
    /* TODO: Handle Error*/
}

Exp::Exp(const Exp *operand, const Type *type)
{
    if (operand && operand->isNumExp() && type && type->isNum())
    {
        this->expType = type->getType();
    }
    else
    {
        /* TODO: Handle Error*/
    }
}

bool Exp::isNumExp() const
{
    return this->expType == ExpType::INT || this->expType == ExpType::BYTE;
}



// ----- Class Statement:
Statement::Statement(const Type* type, const Node* id)
{
    if (type && id)
    {
        this->type = type->getType();
        this->setValue(id->getValue());
        tabels_stack::GetInstance()->add_var(this->getValue(), expTypeToString(this->type));
    }
    else
    {
        //handle error
    }
}