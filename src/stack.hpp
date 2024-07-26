#ifndef _236360_3_STACK_
#define _236360_3_STACK_

#include "hw3_output.hpp"
#include <stack>
#include <iostream>
#include "classes.hpp"

class BasicEntry
{
public:
    std::string name;
    std::string type; // maybe change to enum? maybe need different entries for functions
    int offset;

    BasicEntry(const std::string &name, const std::string &type, int offset);
    void printEntry() const;
};

class SymbolTable
{
public:
    std::vector<BasicEntry> entries_vector;

    void addEntry(const std::string &name, const std::string &type, int offset);
    bool isEntryInTable(const std::string &name);
    std::vector<BasicEntry>::iterator getEntryFromTable(const string &name);
    std::string findType(const std::string &name);
    void printAllEntries() const;
};

class TablesStack
{
public:
    stack<SymbolTable> scopes_stack;
    stack<int> offsets_stack;
    bool in_while = false;

    static TablesStack *singleton_;
    static TablesStack *getInstance();
    TablesStack();
    void addNewTable();
    void removeLastTable();
    void addVar(const std::string &name, const std::string &type);
    bool isVarInStack(const std::string &name);
    /**
     * This method will throw an `out_of_range` error in case no such variable exists.
     */
    std::string getVarType(const std::string &name);
    /**
     * This method will throw an `out_of_range` error in case no such variable exists.
     */
    BasicEntry& getVarByName(const std::string &name);
    void assign(const classes::Node *id, const classes::Exp *exp);
    void enteredWhile();
    void finishedWhile();
    void checkInWhile(const std::string &command);
};

#endif // _236360_3_STACK_