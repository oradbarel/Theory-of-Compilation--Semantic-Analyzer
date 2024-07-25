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
        NONE = -1,
        INT,
        BYTE,
        STRING,
        BOOLEAN,
        VOID
    };

    enum class OperatorType
    {
        NONE = -1,
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

    const std::map<std::string, Func> FUNCTIONS = {
        {"print", {"print", {ExpType::STRING}, ExpType::VOID}},
        {"printi", {"printi", {ExpType::INT, ExpType::BYTE}, ExpType::VOID}},
        {"reai", {"readi", {ExpType::INT, ExpType::BYTE}, ExpType::INT}}};

    // -----

    // ----- Helper Functions:
    // -----

    std::string expTypeToString(ExpType type);
    ExpType stringToExpType(const string& type);
    bool isImplicitCastingAllowd(ExpType fromType, ExpType toType);

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

    class Exp;

    class Statements : public Node
    {
    public:
        Statements(/* args */) = default;
        ~Statements() = default;
    };

    class Type;
    class Statement : public Node
    {
    public:
        ExpType type;
        Statement(const Exp* exp);
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
        void validateIsBoolean();
    };

    // -----
}


#define YYSTYPE classes::Node*

#endif // _236360_3_CLASSES_