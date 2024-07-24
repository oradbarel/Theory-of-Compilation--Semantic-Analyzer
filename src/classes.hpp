#ifndef _236360_3_CLASSES_
#define _236360_3_CLASSES_

#include "hw3_output.hpp"

unsigned int const MAX_BYTE = 255;
namespace classes
{
    enum class ExpType
    {
        // NONE = -1,
        INT,
        BYTE,
        STRING,
        BOOLEAN,
        VOID
    };

    enum class OperatorType
    {
        // NONE = -1,
        LOGIC,
        RELOP,
        ARITHMETIC,
    };

    /**
     * Base class for each nonterminal.
     * This will redfine the `YYSTYPE`.
     */
    class Node
    {
    private:
        std::string value;

    public:
        Node();
        Node(std::string text);
        Node(const Node *other);
        virtual ~Node() = default;
        std::string getValue() const;
        // OperatorType operatorType() const;
    };

    class Program : public Node
    {
    public:
        Program(/* args */) = default;
        ~Program() = default;
    };

    class Statements : public Node
    {
    public:
        Statements(/* args */) = default;
        ~Statements() = default;
    };

    class Statement : public Node
    {
    public:
        Statement(/* args */) = default;
        ~Statement() = default;
    };

    class Call : public Node
    {
    public:
        // Data:
        ExpType retType;
        // Methods:
        
        //Call(/* args */) = default;
        ~Call() = default;
    };

    class Type : public Node
    {
    public:
        // Data:
        ExpType type;

        // Methods:
        Type(ExpType type);
        ~Type() = default;
        ExpType getType() const;
        bool isNum() const;
    };

    class Exp : public Node
    {

    public:
        // Data:
        ExpType expType;

        // Methods:
        Exp(const Exp *other);
        Exp(const Node *id);
        Exp(const Call *call);
        Exp(ExpType expType);
        Exp(ExpType expType, const Node *node);
        Exp(const Exp *operand, OperatorType operatorType);
        Exp(const Exp *operand1, const Exp *operand2, OperatorType operatorType);
        Exp(const Exp *operand, const Type *type);
        virtual ~Exp() = default;
        bool isNumExp() const;
    };
}

/*
class BoolExp : public Exp
{
private:
    bool boolValue;
public:
    //BoolExp(const BoolExp* boolExp) : Node(BoolExp) {}
    ~BoolExp() = default;
};

class NumExp : public Exp
{
private:
    int numValue;
    std::string type;
public:
    //NumExp(const NumExp* numExp) {}
    ~NumExp() = default;
};
*/

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

#define YYSTYPE classes::Node

#endif // _236360_3_CLASSES_