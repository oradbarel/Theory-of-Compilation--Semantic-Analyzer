#include "hw3_output.hpp"
#include <stack>

using namespace std;

// create an entry in the table (for each symbol)
// entry should contain: name, type, offset
// methods: compare?

// create the table (for each scope)
// methods: maketable(), search()?
// new entry

// create a stack (one for tabels and one for offsets)
// methods: push(), pop(), top(), get(i) - get i'th table from the stack


class basic_entry
{
    public:
        std::string name;
        std::string type; // maybe change to enum? maybe need different entries for functions
        int offset;
        basic_entry(std::string name, std::string type, int offset)
        {
            name = name; 
            type = type;
            offset = offset;
        }
};

class symbol_table
{
    public:
    std::vector<basic_entry> entries_vector;

    void add_entry(std::string name, std::string type, int offset)
    {
        basic_entry new_entry(name, type, offset);
        entries_vector.push_back(new_entry);
    }

    bool is_entry_in_table(string name)
    {
        for (auto entry = entries_vector.begin(); entry != entries_vector.end(); ++entry)
        {
            if (entry->name == name)
                return true;
        }
        return false;
    }

    string find_type(string name)
    {
        for (auto entry = entries_vector.begin(); entry != entries_vector.end(); ++entry)
        {
            if (entry->name == name)
                return entry->type;
        }
    }    
};

class tabels_stack
{
    public:
    stack<symbol_table> tabels_stack;

    // do we need a constructor?
    void add_new_table()
    {
        symbol_table new_table;
        tabels_stack.push(new_table);
    } 

    void remove_last_table()
    {
        if (tabels_stack.empty())
        {
            cout << "error: no tabels (scopes) in the stack" << endl;
            exit(0)
        }

        tabels_stack.pop(); //should we first print all the variables in the scope?        
    }

};

