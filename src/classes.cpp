// #define NDEBUG
#include <cassert>
#include "classes.hpp"
#include "stack.hpp"
#include "parser.ypp"

using namespace std;
using namespace output;
using namespace classes;

// ----- Helper Functions:

#define ASSERT_UNEXPECTED_ERROR assert("Unexpected error with args")

string classes::expTypeToString(ExpType type)
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
        ASSERT_UNEXPECTED_ERROR;
        return "";
    }
}

ExpType classes::stringToExpType(const string& type)
{
    if (type == "INT") {
        return ExpType::INT;
    }
    else if (type == "BYTE")
    {
        return ExpType::BYTE;
    }
    else if (type == "BOOL") {
        return ExpType::BOOLEAN;
    }
    else 
    {
        ASSERT_UNEXPECTED_ERROR;
        return ExpType::NONE;
    }
}

bool classes::isImplicitCastingAllowd(ExpType fromType, ExpType toType)
{
    if (fromType == toType)
    {
        return true;
    }
    if (fromType == ExpType::BYTE && toType == ExpType::INT)
    {
        return true;
    }
    return false;
}

// -----
// ----- Class Node:

Node::Node() : value("") {};

Node::Node(const std::string &text) : value(text) {}

Node::Node(const Node *other)
{
    if (other)
    {
        this->value = other->value;
    }
}

std::string Node::getValue() const
{
    return this->value;
}

// -----
// ----- Class Statement:

Statement::Statement(const Type *type, const Node *id)
{
    if (!type || !id)
    {
        ASSERT_UNEXPECTED_ERROR;
        return;
    }
    this->type = type->getType();
    this->setValue(id->getValue());
    TablesStack::GetInstance()->add_var(this->getValue(), expTypeToString(this->type));
}

// -----
// ----- Class Call:

Call::Call(const Node *func, const Exp *arg)
{
    auto funcIter = FUNCTIONS.find(func->getValue());
    if (funcIter == FUNCTIONS.end())
    {
        errorUndefFunc(yylineno, func->getValue());
        exit(0);
    }
    else if (funcIter->second.argTypes.find(arg->expType) == funcIter->second.argTypes.end())
    {
        string type = expTypeToString(*funcIter->second.argTypes.begin());
        errorPrototypeMismatch(yylineno, funcIter->first, type);
        exit(0);
    }
    this->retType = funcIter->second.retType;
    // TODO: Maybe more code regarding the new scope...
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
    if (!other)
    {
        ASSERT_UNEXPECTED_ERROR;
        return;
    }
    this->expType = other->expType;
}

Exp::Exp(const Node *id)
{
    if (!id)
    {
        ASSERT_UNEXPECTED_ERROR;
        return;
    }
    string varName = id->getValue();
    if (!TablesStack::GetInstance()->is_var_in_stack(varName))
    {
        errorUndef(yylineno, varName);
    }
    this->expType = stringToExpType(TablesStack::GetInstance()->get_var_type(varName));
}
Exp::Exp(const Call *call)
{
    if (!call)
    {
        ASSERT_UNEXPECTED_ERROR;
        return;
    }
    this->expType = call->retType;
}

Exp::Exp(ExpType expType) : expType(expType) {}

Exp::Exp(ExpType expType, const Node *node)
{
    if (!node || expType != ExpType::BYTE)
    {
        ASSERT_UNEXPECTED_ERROR;
    }
    if (stoi(node->getValue()) > MAX_BYTE)
    {
        errorByteTooLarge(yylineno, node->getValue());
        exit(0);
    }
    this->expType = expType;
}

Exp::Exp(const Exp *operand, OperatorType operatorType)
{
    if (!operand || operatorType != OperatorType::LOGIC)
    {
        ASSERT_UNEXPECTED_ERROR;
        return;
    }
    if (operand->expType == ExpType::BOOLEAN)
    {
        errorMismatch(yylineno); // TODO: check in piazza what to print...
        exit(0);
    }
    this->expType = ExpType::BOOLEAN;
}

Exp::Exp(const Exp *operand1, const Exp *operand2, OperatorType operatorType)
{
    if (!operand1 || !operand2)
    {
        ASSERT_UNEXPECTED_ERROR;
        return;
    }
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
        ASSERT_UNEXPECTED_ERROR;
        return;
        break;
    }
    errorMismatch(yylineno); // TODO: check in piazza what to print...
    exit(0);
}

Exp::Exp(const Exp *operand, const Type *type)
{
    if (!operand || !type)
    {
        ASSERT_UNEXPECTED_ERROR;
        return;
    }
    if (!operand->isNumExp() && !type->isNum())
    {
        errorMismatch(yylineno); // TODO: check in piazza what to print...
        exit(0);
    }
    this->expType = type->getType();
}

bool Exp::isNumExp() const
{
    return this->expType == ExpType::INT || this->expType == ExpType::BYTE;
}

// -----