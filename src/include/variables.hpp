#include <string>
#include <vector>
#include <map>
#include "node.hpp"

#ifndef VARIABLES_H
#define VARIABLES_H

// the structure w_variable is now in node.hpp


struct w_function
{
    int arg_num;     // the number of arguments
    node *arguments; // contains the name of the arguments
    node *trunc;     // Contains the actual code
    bool inbuild;

    void set_arguments(node *args);
    void set_content(node *trunc);
    w_function()
    {
        this->inbuild = false;
    }
};

struct w_object
{
    std::string name;                               // name of the 'type'
    std::vector<std::string> methods;               // all the names of the methods
    std::map<std::string, w_variable *> attributes; // all the variables

    ~w_object()
    {
        this->methods.clear();
        this->attributes.clear();
        this->name.clear();
    }

    w_variable *get_attribute(std::string);
    bool attribute_exist(std::string);
    bool methods_exist(std::string);

    void attribute_attribution(std::string name, w_variable *value);
};

struct w_class_template
{
    node *trunc;

    w_object *create_object(); // return a newly generated object
};

#endif