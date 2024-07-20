#ifndef _236360_3_CLASSES_
#define _236360_3_CLASSES_

#include "hw3_output.hpp"

/**
 * Base class for each nonterminal.
 * This will redfine the `YYSTYPE`.
 */
class Node
{
protected:
    std::string value;

public:
    // Node() : value("") {};
    Node(const std::string &value) : value(value) {}
    Node(const Node* other) : value(other->value) {}
    virtual ~Node() = default;
};

class Program : public Node
{
private:
    /* data */
public:
    Program(/* args */) = default;
    ~Program() = default;
};

class Statements : public Node
{
private:
    /* data */
public:
    Statements(/* args */) = default;
    ~Statements() = default;
};

class Statement : public Node
{
private:
    /* data */
public:
    Statement(/* args */) = default;
    ~Statement() = default;
};

class Call : public Node
{
private:
    /* data */
public:
    Call(/* args */) = default;
    ~Call() = default;
};

class Type : public Node
{
public:
    Type(const Node* type) : Node(type) {}
    ~Type() = default;
};

class Exp : public Node
{
protected:
    std::string type;
public:
    //Exp(const Node* exp) {}
    ~Exp() = default;
};

class BoolExp : public Exp
{
private:
    bool bool_value;
public:
    //BoolExp(const Node* BoolExp) : Node(BoolExp) {}
    ~BoolExp() = default;
};

/*
class NumType
{
public:
    typedef enum
    {
        INT,
        BYTE
    } type;

private:
    int val;
    type numType;

public:
    NumType(int val, type numType);
    ~NumType();
};

class BoolType
{
private:
    bool val;

public:
    BoolType(bool val);
    ~BoolType();
};

class StringType
{
private:
    std::string val;

public:
    StringType(const std::string &val);
    ~StringType();
};

// Semantic value type definition

union YYSTYPE
{
    NumType numType;   // Example for integer value
    BoolType boolType; // Example for string value
    StringType stringType;
};
*/

#define YYSTYPE Node

#endif // _236360_3_CLASSES_