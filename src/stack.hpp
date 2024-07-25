#ifndef _stack_
#define _stack_

#include "hw3_output.hpp"
#include <stack>
#include <iostream>
#include "classes.hpp"

using namespace std;

class basic_entry
{
    public:
        std::string name;
        std::string type; // maybe change to enum? maybe need different entries for functions
        int offset;

        basic_entry(std::string name, std::string type, int offset);
        void print_entry();
};

class symbol_table
{
    public:
    std::vector<basic_entry> entries_vector;

    void add_entry(std::string name, std::string type, int offset);
    bool is_entry_in_table(string name);
    string find_type(string name);
    void print_all_entries();
};

class tabels_stack
{
    public:
    stack<symbol_table> scopes_stack;
    stack<int> offsets_stack;
    bool in_while = false;

    static tabels_stack* singleton_;
    static tabels_stack* GetInstance();
    tabels_stack();
    void add_new_table();
    void remove_last_table();
    void add_var(std::string name, std::string type);
    bool is_var_in_stack(string name);
    string get_var_type(string name);
    void assign(const Node* id, const Exp* exp);
    void entered_while();
    void finished_while();
    void check_in_while(string command);
};


#endif // _stack_