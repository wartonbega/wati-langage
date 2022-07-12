#include "include/variables.hpp"
#include <iostream>



std::string w_variable::get_type()
{
    switch (this->type)
    {
    case 0:
        return "fonction";
    case 1:
        return "char";
    case 2:
        return "int";
    case 3:
        {
            w_object* o =  (w_object*)this->content;
            return o->name;
        }
    default:
        break;
    }
    return "none";
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
    return this->attributes[name];
}

bool w_object::attribute_exist(std::string name)
{
    if (this->attributes.find(name) != this->attributes.end())
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
    this->attributes[name] = value;    
}

w_variable::~w_variable()
{
    if (this->is_object())
        delete ((w_object *)this->content);
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