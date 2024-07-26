#include <stack>
#include <iostream>
#include <algorithm>
#include "hw3_output.hpp"
#include "stack.hpp"
// #include "parser.ypp"
#include "classes.hpp"

using namespace std;
using namespace output;
using namespace classes;

// --------- Methods for entry ---------

BasicEntry::BasicEntry(const string &name, const string &type, int offset)
    : name(name), type(type), offset(offset) {}

void BasicEntry::printEntry() const
{
    printID(name, offset, type);
}

// --------- Methods for symbol table ---------

void SymbolTable::addEntry(const string &name, const string &type, int offset)
{
    entries_vector.push_back(BasicEntry(name, type, offset));
}

bool SymbolTable::isEntryInTable(const string &name)
{
    return getEntryFromTable(name) != entries_vector.end();
}

vector<BasicEntry>::iterator SymbolTable::getEntryFromTable(const string &name)
{
    return find_if(entries_vector.begin(), entries_vector.end(), [name](const BasicEntry &e)
                   { return e.name == name; });
}

string SymbolTable::findType(const string &name)
{
    auto it = getEntryFromTable(name);
    if (it != entries_vector.end())
    {
        return it->type;
    }
    throw out_of_range("In `find_type`: A variable with such a name does not exist");
}

void SymbolTable::printAllEntries() const
{
    for (const auto & entry : entries_vector)
    {
        entry.printEntry();
    }
}

// --------- nethods for stack ---------

TablesStack::TablesStack()
{
    SymbolTable global_tabel;
    global_tabel.addEntry("print", makeFunctionType("STRING", "VOID"), 0);
    global_tabel.addEntry("printi", makeFunctionType("INT", "VOID"), 0);
    global_tabel.addEntry("readi", makeFunctionType("INT", "INT"), 0);
    scopes_stack.push(global_tabel);
    offsets_stack.push(0);
}

void TablesStack::addNewTable()
{
    scopes_stack.push(SymbolTable());
    offsets_stack.push(offsets_stack.top());
}

void TablesStack::removeLastTable()
{
    if (scopes_stack.empty() || offsets_stack.empty())
    {
        cout << "error: no tabels (scopes) in the stack" << endl;
        exit(0);
    }
    endScope();
    scopes_stack.top().printAllEntries();
    scopes_stack.pop();
    offsets_stack.pop();
}

bool TablesStack::isVarInStack(const string &name)
{
    stack<SymbolTable> copiedStack(this->scopes_stack);
    while (!copiedStack.empty())
    {
        SymbolTable curr_table = copiedStack.top();
        if (curr_table.isEntryInTable(name))
        {
            return true;
        }
        copiedStack.pop();
    }
    return false;
}

BasicEntry &TablesStack::getVarByName(const std::string &name)
{
    stack<SymbolTable> copiedStack(this->scopes_stack);
    while (!copiedStack.empty())
    {
        SymbolTable curr_table = copiedStack.top();
        if (curr_table.isEntryInTable(name))
        {
            return *curr_table.getEntryFromTable(name);
        }
        copiedStack.pop();
    }
    throw out_of_range("In `get_var_by_name`: A variable with such a name does not exist");
}

string TablesStack::getVarType(const string &name)
{
    stack<SymbolTable> copiedStack(this->scopes_stack);
    while (!copiedStack.empty())
    {
        SymbolTable curr_table = copiedStack.top();
        if (curr_table.isEntryInTable(name))
        {
            return curr_table.findType(name);
        }
        copiedStack.pop();
    }
    throw out_of_range("In `get_var_type`: A variable with such a name does not exist");
}

void TablesStack::addVar(const string &name, const string &type)
{
    bool var_exist = this->isVarInStack(name);
    if (var_exist)
    {
        errorDef(yylineno, name);
        exit(0);
    }
    int curr_offset = this->offsets_stack.top();
    this->offsets_stack.pop();
    this->scopes_stack.top().addEntry(name, type, curr_offset);
    this->offsets_stack.push(curr_offset + 1);
}

void TablesStack::assign(const classes::Node *id, const classes::Exp *exp)
{
    string id_name = id->getValue();
    if (!isVarInStack(id_name))
    {
        errorUndef(yylineno, id_name);
        exit(0);
    }
    ExpType id_type;
    try
    {
        id_type = stringToExpType(this->getVarType(id_name));
    }
    catch (const std::out_of_range &e)
    {
        errorUndef(yylineno, id_name);
        exit(0);
    }
    if (!isImplicitCastingAllowd(exp->expType, id_type))
    {
        // cout << "print errorMismatch from assign with lineno" << yylineno << endl;
        errorMismatch(yylineno);
        exit(0);
    }
    TablesStack::getInstance()->getVarByName(id_name).type = expTypeToString(exp->expType);
}

void TablesStack::enteredWhile()
{
    TablesStack::getInstance()->addNewTable();
    this->in_while = true;
}

void TablesStack::finishedWhile()
{
    TablesStack::getInstance()->removeLastTable();
    this->in_while = false;
}

void TablesStack::checkInWhile(const string &command)
{
    if (!this->in_while)
    {
        if (command == "break")
        {
            errorUnexpectedBreak(yylineno);
            exit(0);
        }
        else
        {
            errorUnexpectedContinue(yylineno);
            exit(0);
        }
    }
}

TablesStack *TablesStack::singleton_ = nullptr;

TablesStack *TablesStack::getInstance()
{
    if (singleton_ == nullptr)
    {
        singleton_ = new TablesStack();
    }
    return singleton_;
}