#ifndef _236360_3_CLASSES_
#define _236360_3_CLASSES_

#include "hw3_output.hpp"
#include <set>
#include <map>

extern int yylineno;

namespace classes
{
    // ----- Enums:

    enum class ExpType
    {
        INT,
        BYTE,
        STRING,
        BOOLEAN,
        VOID
    };

    std::string expTypeToString(ExpType type);

    enum class OperatorType
    {
        LOGIC,
        RELOP,
        ARITHMETIC,
    };

    // -----
    // ----- Constants:

    const unsigned int MAX_BYTE = 255;

    typedef struct
    {
        std::string funcName;
        std::set<ExpType> argTypes;
        ExpType retType;
    } Func;

    // const struct {
    //     Func printFunc = {
    //         "print",
    //         ExpType::STRING,
    //         ExpType::VOID
    //     };
    //     Func printiFunc = {
    //         "printi",
    //         ExpType::INT,
    //         ExpType::VOID
    //     };
    //     Func readiFunc = {
    //         "readi",
    //         ExpType::INT,
    //         ExpType::INT
    //     };
    // } Functions;

    const std::map<std::string, Func> FUNCTIONS = {
        {"print", {"print", {ExpType::STRING}, ExpType::VOID}},
        {"printi", {"printi", {ExpType::INT, ExpType::BYTE}, ExpType::VOID}},
        {"reai", {"readi", {ExpType::INT, ExpType::BYTE}, ExpType::INT}}};

    // -----
    // ----- Classes:

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
        Node(const std::string &text);
        Node(const Node *other);
        virtual ~Node() = default;
        std::string getValue() const;
        void setValue(std::string val);
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
        ExpType type;

        Statement(const Type* type, const Node* id);
        ~Statement() = default;
    };

    class Call : public Node
    {
    private:
        //static Func getFuncByName(const std::string &name);

    public:
        ExpType retType;

        Call(const Node *func, const Exp *arg);
        ~Call() = default;
    };

    class Type : public Node
    {
    public:
        ExpType type;

        Type(ExpType type);
        ~Type() = default;
        ExpType getType() const;
        bool isNum() const;
    };

    class Exp : public Node
    {

    public:
        ExpType expType;

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

    // -----
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

#define YYSTYPE classes::Node*

#endif // _236360_3_CLASSES_