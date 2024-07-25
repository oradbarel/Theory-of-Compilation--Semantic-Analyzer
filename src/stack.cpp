#include "hw3_output.hpp"
#include <stack>
#include <iostream>
#include "stack.hpp"
#include "parser.ypp"
#include "classes.hpp"

using namespace std;
using namespace classes;

// --------- Methods for entry ---------

basic_entry::basic_entry(std::string name, std::string type, int offset)
{
    this->name = name; 
    this->type = type;
    this->offset = offset;
}

void basic_entry::print_entry()
{
    output::printID(name, offset, type);
}


// --------- nethods for symbol table ---------

void symbol_table::add_entry(std::string name, std::string type, int offset)
{
    basic_entry new_entry(name, type, offset);
    entries_vector.push_back(new_entry);
}

bool symbol_table::is_entry_in_table(string name)
{
    for (auto entry = entries_vector.begin(); entry != entries_vector.end(); ++entry)
    {
        if (entry->name == name)
            return true;
    }
    return false;
}

string symbol_table::find_type(string name)
{
    for (auto entry = entries_vector.begin(); entry != entries_vector.end(); ++entry)
    {
        if (entry->name == name)
            return entry->type;
    }
}   

void symbol_table::print_all_entries()
{
    for (auto entry = entries_vector.begin(); entry != entries_vector.end(); ++entry)
    {
        entry->print_entry();
    }
}


// --------- nethods for stack ---------

tabels_stack::tabels_stack()
{
    symbol_table global_tabel;
    global_tabel.add_entry("print", output::makeFunctionType("string", "void"), 0);
    global_tabel.add_entry("printi", output::makeFunctionType("int", "void"), 0);
    global_tabel.add_entry("readi", output::makeFunctionType("int", "int"), 0);
    scopes_stack.push(global_tabel);
    offsets_stack.push(0);
}

void tabels_stack::add_new_table()
{
    symbol_table new_table;
    scopes_stack.push(new_table);
    int curr_offset = offsets_stack.top(); 
    offsets_stack.push(curr_offset);
} 

void tabels_stack::remove_last_table()
{
    if (scopes_stack.empty() || offsets_stack.empty())
    {
        std::cout << "error: no tabels (scopes) in the stack" << std::endl;
        exit(0);
    }
    output::endScope();
    symbol_table current_scope = scopes_stack.top();
    current_scope.print_all_entries();
    scopes_stack.pop();
    offsets_stack.pop();  
}

bool tabels_stack::is_var_in_stack(string name)
{
    std::stack<symbol_table> copiedStack(this->scopes_stack);
    while (!copiedStack.empty()) 
    {
        symbol_table curr_table = copiedStack.top();
        if (curr_table.is_entry_in_table(name))
        {
            return true;
        }
        copiedStack.pop();
    }
    return false;
}

string tabels_stack::get_var_type(string name)
{
    std::stack<symbol_table> copiedStack(this->scopes_stack);
    while (!copiedStack.empty()) 
    {
        symbol_table curr_table = copiedStack.top();
        if (curr_table.is_entry_in_table(name))
        {
            return curr_table.find_type(name);
        }
        copiedStack.pop();
    }
}

void tabels_stack::add_var(std::string name, std::string type)
{
    bool var_exist = this->is_var_in_stack(name);
    if (var_exist)
    {
        output::errorDef(yylineno, name);
        exit(0);
    }

    // update offset stack:
    int curr_offset = this->offsets_stack.top();
    curr_offset++;
    this->offsets_stack.pop();
    this->offsets_stack.push(curr_offset);

    //update scopes_stack:
    symbol_table curr_scope = this->scopes_stack.top();
    curr_scope.add_entry(name, type, curr_offset);
}

void tabels_stack::assign(const Node* id, const Exp* exp)
{
    // check matching types
    std::string id_name = id->getValue();
    if(is_var_in_stack(id_name))
    {
        string id_type = this->get_var_type(id_name);
        string exp_type = expTypeToString(exp.expType);
        if(id_type != exp_type)
        {
            output::errorMismatch(yylineno);
            exit(0);
        }
    }
    else
    {
        output::errorUndef(yylineno, id_name);
        exit(0);
    }
}

void tabels_stack::entered_while()
{
    tabels_stack::GetInstance()->add_new_table();
    this->in_while = true;
}

void tabels_stack::finished_while()
{
    tabels_stack::GetInstance()->remove_last_table();
    this->in_while = false;
}

void tabels_stack::check_in_while(string command)
{
    if (!this->in_while)
    {
        if(command == "break")
        {
            output::errorUnexpectedBreak(yylineno);
            exit(0);
        }
        else
        {
            output::errorUnexpectedContinue(yylineno);
            exit(0);
        }
    }
}


tabels_stack* tabels_stack::singleton_= nullptr;

tabels_stack* tabels_stack::GetInstance() {
    if(singleton_== nullptr){
        singleton_ = new tabels_stack();
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