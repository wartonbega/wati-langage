#include <vector>
#include <string>

#ifndef NODE_H
#define NODE_H
struct node
{
    std::vector<node *> children;
    std::string value;
    
    node(std::string);

    std::vector<std::string> get_childs_value();
    std::vector<std::string> get_childs_type();

    std::string to_string(int tabs);
    
    void push_child(node *);
};

#endif