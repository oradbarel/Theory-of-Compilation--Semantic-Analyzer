#include "hw3_output.hpp"
#include <stack>
#include <iostream>
#include "stack.hpp"

using namespace std;

// --------- nethods for entry ---------

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
    return("error: " + name + " is not in the table");
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
    offsets_stack.push(0); // maybe should be 1?
}

void tabels_stack::add_new_table()
{
    symbol_table new_table;
    scopes_stack.push(new_table);
    offsets_stack.push(0);
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

void tabels_stack::add_var(std::string name, std::string type)
{
    // update offset stack:
    int curr_offset = this->offsets_stack.top();
    curr_offset++;
    this->offsets_stack.pop();
    this->offsets_stack.push(curr_offset);

    //update scopes_stack:
    symbol_table curr_scope = this->scopes_stack.top();
    curr_scope.add_entry(name, type, curr_offset);
}

bool tabels_stack::is_var_in_stack(string name)
{
    bool found_var = false;
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