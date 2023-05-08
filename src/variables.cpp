#include "include/variables.hpp"
#include "include/types.hpp"
#include "include/visitor.hpp"
#include <iostream>


bool accept_var_name(std::string name)
{
    if (name[0] == '!')
        return false;
    if (is_digit(name))
        return false;
    if (is_explicit(name))
        return false;

    return true;
}

void variable_table::garbage_collector()
{
    return;
}

void variable_table::assign(std::string name, w_variable *value, int thread_id)
{
    if (!accept_var_name(name))
    {
        std::string err = "nom de variable incorrecte : '" + name + "'";
        error(err, what_reference(thread_id)->top(), thread_id);
    }
    if (!this->exist(name))
        value->use += 1;
    
    this->vars[name] = value;
}

void variable_table::unuse()
{
    // Décrémente de 1 chaque utilisation de variable (le namespace n'est donc plus utilisé)   
    for (auto const& [key, val] : this->vars)
    {
        val->use -= 1;
    }
}

void variable_table::use()
{
    // Décrémente de 1 chaque utilisation de variable (le namespace n'est donc plus utilisé)   
    for (auto const& [key, val] : this->vars)
    {
        val->use += 1;
    }
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

w_variable::w_variable(int64_t content)
{
    int64_t *r = new int64_t(content);
    this->content = (void *)r;
    this->type = T_INT;
    this->use = 0;
}

w_variable::w_variable(std::string content)
{
    std::string *r = new std::string(content);
    this->content = (void *)r;
    this->type = T_CHAR; 
    this->use = 0;
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

int64_t w_variable::convert_to_int()
{
    return *(int64_t *)this->content;
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
    this->attributes->assign(name, value, 0);
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