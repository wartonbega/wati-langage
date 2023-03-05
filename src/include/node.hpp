#include <vector>
#include <string>


#ifndef NODE_H
#define NODE_H

struct w_variable
{
    // the type of the variable : 
    // 0 -> function
    // 1 -> char
    // 2 -> int
    // 3 -> object
    // 4 -> tracker
    int type;
    
    // the pointer to the real value
    void *content;
    // we can cast it according to the type of the variable
    bool lock; // if true, then the memory is being accessed by a thread. In this case, do not try to access it.

    ~w_variable();
    w_variable();
    w_variable(int64_t);
    w_variable(std::string);

    std::string get_type();
    bool is_object();
    std::string convert_to_char();
    int64_t convert_to_int();
};

struct node
{
    std::vector<node *> children;
    std::string value;
    w_variable *pre_value; // so we don't need to create lots of new variables at each number for example
    
    node *set_prevalue(w_variable *);

    std::string reference;

    node(std::string);

    std::vector<std::string> get_childs_value();
    std::vector<std::string> get_childs_type();

    std::string to_string(int tabs);
    
    node *push_child(node *);


    ~node();
};

#endif