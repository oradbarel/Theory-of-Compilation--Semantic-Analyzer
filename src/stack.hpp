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
    void print_entry();
};

class SymbolTable
{
public:
    std::vector<BasicEntry> entries_vector;

    void add_entry(const std::string &name, const std::string &type, int offset);
    bool is_entry_in_table(const std::string &name);
    std::vector<BasicEntry>::iterator get_entry_from_table(const string &name);
    std::string find_type(const std::string &name);
    void print_all_entries();
};

class TablesStack
{
public:
    stack<SymbolTable> scopes_stack;
    stack<int> offsets_stack;
    bool in_while = false;

    static TablesStack *singleton_;
    static TablesStack *GetInstance();
    TablesStack();
    void add_new_table();
    void remove_last_table();
    void add_var(const std::string &name, const std::string &type);
    bool is_var_in_stack(const std::string &name);
    /**
     * This method will throw an `out_of_range` error in case no such variable exists.
     */
    std::string get_var_type(const std::string &name);
    /**
     * This method will throw an `out_of_range` error in case no such variable exists.
     */
    BasicEntry& get_var_by_name(const std::string &name);
    void assign(const classes::Node *id, const classes::Exp *exp);
    void entered_while();
    void finished_while();
    void check_in_while(const std::string &command);
};

#endif // _236360_3_STACK_