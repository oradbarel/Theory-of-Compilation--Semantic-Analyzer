#include "hw3_output.hpp"
#include <stack>
#include <iostream>
#include "stack.hpp"

using namespace std;


// --------- nethods for entry ---------

basic_entry::basic_entry(std::string name, std::string type, int offset)
{
    name = name; 
    type = type;
    offset = offset;
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
    return("error: entry does not exist");
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
}

void tabels_stack::add_new_table()
{
    symbol_table new_table;
    scopes_stack.push(new_table);
} 

void tabels_stack::remove_last_table()
{
    if (scopes_stack.empty())
    {
        std::cout << "error: no tabels (scopes) in the stack" << std::endl;
        exit(0);
    }
    output::endScope();
    symbol_table current_scope = scopes_stack.top();
    current_scope.print_all_entries();
    scopes_stack.pop(); //should we first print all the variables in the scope?    
}


int main()
{
    basic_entry my_entry("x", "int", 1);
    my_entry.print_entry();
    return(1);
}