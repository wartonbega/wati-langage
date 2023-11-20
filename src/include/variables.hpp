#include <string>
#include <vector>
#include <map>

#include "node.hpp"

#ifndef VARIABLES_H
#define VARIABLES_H


// the structure w_variable is now in node.hpp

bool accept_var_name(std::string name);

enum types 
{
    T_FONCTION, // 0
    T_CHAR, // 1
    T_INT, // 2
    T_OBJECT, // 3
    T_TRACKER // 4
};

struct variable_table  // this is like a namespace for the variables to be
{
    std::map<std::string, w_variable *> vars;
    std::vector<void *> traqueurs; // Permet de garder en mémoire tous les traqueurs qui existent
    bool lock; // Permet de verouiller l'utilisation des tables de variables

    variable_table();
    variable_table(const variable_table &);
    ~variable_table();
    void assign(std::string, w_variable *, int);
    void delete_var(std::string name);
    void assign_no_copy(std::string, w_variable *, int);
    w_variable *get(std::string);
    bool exist(std::string name);
    void garbage_collector();
    void unuse();
    void use();
    void add_track(void *t);
};

struct w_function
{
    int arg_num;     // the number of arguments
    node *arguments; // contains the name of the arguments
    node *trunc;     // Contains the actual code
    bool inbuild;

    void set_arguments(node *args);
    void set_content(node *trunc);
    std::string get_arg_name(int arg);

    w_function()
    {
        this->inbuild = false;
    }
};

struct w_object
{
    std::string name;                               // name of the 'type'
    variable_table *attributes; // all the variables

    w_object()
    {
        this->attributes = new variable_table();
    }

    ~w_object()
    {
        this->attributes->vars.clear();
        this->name.clear();
    }

    w_variable *get_attribute(std::string);
    bool attribute_exist(std::string);
    bool is_iterable();
    void attribute_attribution(std::string name, w_variable *value);
};

struct w_class_template
{
    node *trunc;
    w_object *create_object(); // return a newly generated object
};


#endif