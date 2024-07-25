#include <stack>
#include <iostream>
#include <algorithm>
#include "hw3_output.hpp"
#include "stack.hpp"
//#include "parser.ypp"
#include "classes.hpp"

using namespace std;
using namespace output;
using namespace classes;

// --------- Methods for entry ---------

BasicEntry::BasicEntry(const string &name, const string &type, int offset)
{
    this->name = name;
    this->type = type;
    this->offset = offset;
}

void BasicEntry::print_entry()
{
    printID(name, offset, type);
}

// --------- nethods for symbol table ---------

void SymbolTable::add_entry(const string &name, const string &type, int offset)
{
    BasicEntry new_entry(name, type, offset);
    entries_vector.push_back(new_entry);
}

bool SymbolTable::is_entry_in_table(const string &name)
{
    for (auto entry = entries_vector.begin(); entry != entries_vector.end(); ++entry)
    {
        if (entry->name == name)
            return true;
    }
    return false;
}

vector<BasicEntry>::iterator SymbolTable::get_entry_from_table(const string &name)
{
    return find_if(entries_vector.begin(), entries_vector.end(), [name] (const BasicEntry& e) { return e.name == name; });
}

string SymbolTable::find_type(const string &name)
{
    for (auto entry = entries_vector.begin(); entry != entries_vector.end(); ++entry)
    {
        if (entry->name == name)
            return entry->type;
    }
    throw out_of_range("A variable with such a name does not exist");
}

void SymbolTable::print_all_entries()
{
    for (auto entry = entries_vector.begin(); entry != entries_vector.end(); ++entry)
    {
        entry->print_entry();
    }
}

// --------- nethods for stack ---------

TablesStack::TablesStack()
{
    SymbolTable global_tabel;
    global_tabel.add_entry("print", makeFunctionType("string", "void"), 0);
    global_tabel.add_entry("printi", makeFunctionType("int", "void"), 0);
    global_tabel.add_entry("readi", makeFunctionType("int", "int"), 0);
    scopes_stack.push(global_tabel);
    offsets_stack.push(0);
}

void TablesStack::add_new_table()
{
    SymbolTable new_table;
    scopes_stack.push(new_table);
    int curr_offset = offsets_stack.top();
    offsets_stack.push(curr_offset);
}

void TablesStack::remove_last_table()
{
    if (scopes_stack.empty() || offsets_stack.empty())
    {
        cout << "error: no tabels (scopes) in the stack" << endl;
        exit(0);
    }
    endScope();
    SymbolTable current_scope = scopes_stack.top();
    current_scope.print_all_entries();
    scopes_stack.pop();
    offsets_stack.pop();
}

bool TablesStack::is_var_in_stack(const string &name)
{
    stack<SymbolTable> copiedStack(this->scopes_stack);
    while (!copiedStack.empty())
    {
        SymbolTable curr_table = copiedStack.top();
        if (curr_table.is_entry_in_table(name))
        {
            return true;
        }
        copiedStack.pop();
    }
    return false;
}

BasicEntry &TablesStack::get_var_by_name(const std::string &name)
{
    stack<SymbolTable> copiedStack(this->scopes_stack);
    while (!copiedStack.empty())
    {
        SymbolTable curr_table = copiedStack.top();
        if (curr_table.is_entry_in_table(name))
        {
            return *curr_table.get_entry_from_table(name);
        }
        copiedStack.pop();
    }
    throw out_of_range("A variable with such a name does not exist");
}

string TablesStack::get_var_type(const string &name)
{
    stack<SymbolTable> copiedStack(this->scopes_stack);
    while (!copiedStack.empty())
    {
        SymbolTable curr_table = copiedStack.top();
        if (curr_table.is_entry_in_table(name))
        {
            return curr_table.find_type(name);
        }
        copiedStack.pop();
    }
    throw out_of_range("A variable with such a name does not exist");
}

void TablesStack::add_var(const string &name, const string &type)
{
    bool var_exist = this->is_var_in_stack(name);
    if (var_exist)
    {
        errorDef(yylineno, name);
        exit(0);
    }

    // update offset stack:
    int curr_offset = this->offsets_stack.top();
    curr_offset++;
    this->offsets_stack.pop();
    this->offsets_stack.push(curr_offset);

    // update scopes_stack:
    SymbolTable curr_scope = this->scopes_stack.top();
    curr_scope.add_entry(name, type, curr_offset);
}

void TablesStack::assign(const classes::Node *id, const classes::Exp *exp)
{
    string id_name = id->getValue();
    if (!is_var_in_stack(id_name))
    {
        errorUndef(yylineno, id_name);
        exit(0);
    }
    ExpType id_type = stringToExpType(this->get_var_type(id_name));
    if (!isImplicitCastingAllowd(exp->expType, id_type))
    {
        errorMismatch(yylineno);
        exit(0);
    }
    TablesStack::GetInstance()->get_var_by_name(id_name).type = expTypeToString(exp->expType);
}

void TablesStack::entered_while()
{
    TablesStack::GetInstance()->add_new_table();
    this->in_while = true;
}

void TablesStack::finished_while()
{
    TablesStack::GetInstance()->remove_last_table();
    this->in_while = false;
}

void TablesStack::check_in_while(const string &command)
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

TablesStack *TablesStack::GetInstance()
{
    if (singleton_ == nullptr)
    {
        singleton_ = new TablesStack();
    }
    return singleton_;
}

// int main()
// {
//     tabels_stack my_stack;
//     my_stack.remove_last_table();
//     my_stack.remove_last_table();
//     return(1);
// }