#include <vector>
#include <string>


#ifndef NODE_H
#define NODE_H

struct w_variable
{
    int type;
    void *content;

    ~w_variable();
    std::string get_type();
    bool is_object();
    std::string convert_to_char();
    int convert_to_int();
};

struct node
{
    std::vector<node *> children;
    std::string value;
    w_variable *pre_value; // so we don't need to create lots of new variables at each number for example
    
    std::string reference;

    node(std::string);

    std::vector<std::string> get_childs_value();
    std::vector<std::string> get_childs_type();

    std::string to_string(int tabs);
    
    void push_child(node *);


    ~node();
};

#endif