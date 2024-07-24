#include "classes.hpp"

using namespace std;
using namespace classes;

// ----- Class Node:

Node::Node() : value("") {};

Node::Node(std::string text)
{
    this->value = text;
}

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

// -----
