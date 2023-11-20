#include "include/variables.hpp"
#include "include/types.hpp"
#include "include/visitor.hpp"
#include "include/parser.hpp"
#include "include/trackers.hpp"
#include "include/computations.hpp"
#include <iostream>



bool accept_var_name(std::string name)
{
    if (name[0] == '!')
        return false;
    if (is_digit(name))
        return false;
    if (is_explicit(name))
        return false;
    if (name == COMP_KEYWORD)
        return false;
    return true;
}

void variable_table::garbage_collector()
{
    for (auto const &[key, val] : this->vars)
    {
        val->check_garbage();
    }
    return;
}

variable_table::variable_table()
{
    this->lock = false;
    //this->use();
}

variable_table::variable_table(const variable_table &t)
{
    this->lock = false;
    this->vars = t.vars;
    this->use();
}

variable_table::~variable_table()
{
    this->unuse();
    for (auto t : this->traqueurs)
        ((tracker *)t)->usable = 0;
    this->garbage_collector(); // Puis on supprimes les variables
}

void variable_table::assign_no_copy(std::string name, w_variable *value, int thread_id)
{
    this->lock = true;

    check_value_integrity(value, "", thread_id); // On vérifie que la valeur qu'on veut traiter existe bien

    // Ici on ne fait pas de copie de la variable (comme indiquée dans le nom)

    if (!accept_var_name(name))
    {
        std::string err = "nom de variable incorrecte : '" + name + "'";
        error(err, ref_pop(thread_id), thread_id);
    }

    value->use += 1;       // Dans tous les cas, la variable est utilisée une fois de plus
    if (this->exist(name)) // Mais dans le cas ou la variable existe déjà
    {                      // alors il faut décrémenter son nombre d'utilisation
        this->get(name)->use -= 1;
        this->get(name)->check_garbage();
    }

    if (this->exist(name) && this->get(name)->constante)
        error("Ne peut pas modifier une variable constante", ref_pop(thread_id), thread_id);

    if (name[0] == '_')
    { // alors on définit la variable comme constante, cad qu'elle ne pourras pas être modifié
        name = remove_prefix(name); // enlève le '_'
        value->constante = 1;
    }

    this->vars[name] = value;
    this->lock = false;
    this->garbage_collector();
}

void variable_table::assign(std::string name, w_variable *value, int thread_id)
{
    // while (this->lock) // On attend que le verrou soit inactif sur cette table de variables
    //     continue;

    this->lock = true;

    check_value_integrity(value, "", thread_id); // On vérifie que la valeur qu'on veut traiter existe bien
    
    if (!accept_var_name(name))
    {
        std::string err = "nom de variable incorrecte : '" + name + "'";
        error(err, ref_pop(thread_id), thread_id);
    }

    value->use += 1;       // Dans tous les cas, la variable est utilisée une fois de plus
    if (this->exist(name)) // Mais dans le cas ou la variable existe déjà
    {                      // alors il faut décrémenter son nombre d'utilisation
        this->get(name)->use -= 1;
        this->get(name)->check_garbage();
    }

    if (this->exist(name) && this->get(name)->constante)
        error("Ne peut pas modifier une variable constante", ref_pop(thread_id), thread_id);

    if (name[0] == '_')
    { // alors on définit la variable comme constante, cad qu'elle ne pourras pas être modifié
        name = remove_prefix(name); // enlève le '_'
        value->constante = 1;
    }

    this->vars[name] = value;
    this->lock = false;
    this->garbage_collector();
}

void variable_table::delete_var(std::string name)
{
    this->vars[name]->use -= 1;
    this->vars[name]->check_garbage();
    this->vars.erase(name);
}

void variable_table::unuse()
{
    // Décrémente de 1 chaque utilisation d'un pointeur vers une variable.
    // Attention : c'est un algorithme en O(n^2) Donc pas ouf
    // mais à ce jour nécessaire si on veux éviter la duplication d'un même pointeur
    std::vector<w_variable *> l;
    for (auto const &[key, val] : this->vars)
    {
        bool pass = 0;
        for (auto e : l)
        {
            if (e == val)
            {
                pass = 1;
                break;
            }
        }
        if (! pass)
        {
            l.push_back(val);
            val->use -= 1;
        }
    }
}

void variable_table::use()
{
    for (auto const &[key, val] : this->vars)
    {
        val->use += 1;
    }
}

w_variable *variable_table::get(std::string name)
{
    // while (this->lock) // On attend que le verrou soit inactif sur cette table de variables
    //     continue;
    this->lock = true;

    w_variable *r = this->vars[name];

    this->lock = false;

    return r;
}

void variable_table::add_track(void *t)
{
    this->traqueurs.push_back(t);
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
        w_object *o = (w_object *)this->content;
        return o->name;
    }
    case T_TRACKER:
        return "traqueur";
    default:
        break;
    }
    return "none";
}

w_variable::w_variable()
{
    this->use = 0;
    this->constante = 0;
}

w_variable::w_variable(int64_t content)
{
    int64_t *r = new int64_t(content);
    this->content = (void *)r;
    this->type = T_INT;
    this->use = 0;
    this->constante = 0;
}

w_variable::w_variable(std::string content)
{
    std::string *r = new std::string(content);
    this->content = (void *)r;
    this->type = T_CHAR;
    this->use = 0;
    this->constante = 0;
}

w_variable::~w_variable()
{
    if (this->explicit_definition)
        return;
    if (this->get_type() == "int")
    {
        int64_t *p = (int64_t *)this->content;
        delete p;
    }
    else if (this->get_type() == "char" || this->get_type() == "fonction")
    {
        std::string *p = (std::string *)this->content;
        delete p;
    }
    else if (this->get_type() == "traqueur")
    {
        tracker *t = (tracker *)this->content;
        delete t;
    }
    //else
    //{
    //    w_object *o = (w_object *)this->content;
    //    delete o;
    //}
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

void delete_w_var(w_variable *ptr)
{
    free((void *)ptr);
}

bool w_variable::check_garbage()
{
    if (this->use == 0)
    {
        this->~w_variable();
        return true; // on a bien détruit la valeur
    }
    if (this->use < 0)
    {
        std::cout << "Erreur : Tentative de libération d'une variable non existente" << std::endl;
        exit(0);
        //this->~w_variable();
    }
    return false; // On ne l'a pas détruite
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

void w_object::attribute_attribution(std::string name, w_variable *value)
{
    this->attributes->assign(name, value, 0);
}
bool w_object::is_iterable()
{
    if (!this->attribute_exist("iterable"))
        return false;
    return is_int_true(this->get_attribute("iterable"));       
}

void w_function::set_arguments(node *args)
{
    this->arguments = args;
    this->arg_num = args->children.size();
}

void w_function::set_content(node *trunc)
{
    this->trunc = trunc;
}

std::string w_function::get_arg_name(int arg)
{
    return this->arguments->children[arg]->children[0]->children[0]->value;
}

w_object *w_class_template::create_object()
{
    return new w_object();
}

