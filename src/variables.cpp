#include "include/variables.hpp"
#include <iostream>


void variable_table::assign(std::string name, w_variable *value)
{
    this->vars[name] = value;
}

w_variable *variable_table::get(std::string name)
{
    return this->vars[name];
}

bool variable_table::exist(std::string name)
{
    return this->vars.find(name) != this->vars.end();
}

std::string w_variable::get_type()
{
    switch (this->type)
    {
    case T_FONCTION:
        return "fonction";
    case T_CHAR:
        return "char";
    case T_INT:
        return "int";
    case T_OBJECT:
        {
            w_object* o =  (w_object*)this->content;
            return o->name;
        }
    case T_TRACKER:
        return "traqueur";
    default:
        break;
    }
    return "none";
}

w_variable::w_variable(){}

w_variable::w_variable(int content)
{
    int *r = new int(content);
    this->content = (void *)r;
    this->type = 2; // int
}

w_variable::w_variable(std::string content)
{
    std::string *r = new std::string(content);
    this->content = (void *)r;
    this->type = 1; // char
}


w_variable::~w_variable()
{
    if (this->is_object())
    {
        delete (w_object *)this->content;
    }
    else if (this->get_type() == "char" || this->get_type() == "fonction")
    {
        delete (std::string *)this->content;
    }
}

bool w_variable::is_object()
{
    if (this->get_type() != "char" and this->get_type() != "int" and this->get_type() != "none" and this->get_type() != "fonction")
    {
        return true;
    }
    return false;
}

std::string w_variable::convert_to_char()
{
    return *(std::string *)this->content;
}

int w_variable::convert_to_int()
{
    return *(int *)this->content;
}

w_variable *w_object::get_attribute(std::string name)
{
    return this->attributes->get(name);
}

bool w_object::attribute_exist(std::string name)
{
    if (this->attributes->exist(name))
    {
        return true;
    }
    return false;
}

bool w_object::methods_exist(std::string name)
{
    for (auto i : this->methods)
    {
        if (i == name)
        {
            return true;
        }
    }
    return false;
}
void w_object::attribute_attribution(std::string name, w_variable * value)
{
    this->attributes->assign(name, value);
}

void w_function::set_arguments(node *args)
{
    // () :
    //      arg1
    //      arg...     
    this->arguments = args;
    this->arg_num = args->children.size();
}
void w_function::set_content(node *trunc)
{
    this->trunc = trunc;
}



w_object *w_class_template::create_object()
{
    return new w_object();
}