#include <iostream>

#include "include/visitor.hpp"
#include "include/variables.hpp"
#include "include/types.hpp"
#include "include/inbuilds.hpp"

std::map<std::string, w_function *> functions;
std::map<std::string, w_class_template *> classes;
bool is_initialized_inbuild = false;

void error(std::string err)
{
    std::cout << err << std::endl;
    exit(1);
}

bool function_exist(std::string name, std::map<std::string, w_function *> variables_t)
{
    if (variables_t.find(name) != variables_t.end() || visitor_is_inbuild(name))
    {
        return true;
    }
    return false;
}

bool class_exist(std::string name, std::map<std::string, w_class_template *> variables_t)
{
    if (variables_t.find(name) != variables_t.end() || visitor_is_inbuild(name))
    {
        return true;
    }
    return false;
}

bool variable_exist(std::string name, std::map<std::string, w_variable *> variables_t)
{
    if (variables_t.find(name) != variables_t.end())
    {
        return true;
    }
    return false;
}

void visitor_init_inbuild_functions()
{
    is_initialized_inbuild = true;
    // The inbuilds functions for the value char
    w_function *char_plus = new w_function();
    char_plus->inbuild = true;
    functions["!char.plus"] = char_plus; // +

    w_function *char_equals = new w_function();
    char_equals->inbuild = true;
    functions["!char.equals"] = char_equals; // ==

    w_function *char_ne = new w_function();
    char_ne->inbuild = true;
    functions["!char.ne"] = char_ne; // !=

    // The inbuilds functions for the value int
    w_function *int_plus = new w_function();
    int_plus->inbuild = true;
    functions["!int.plus"] = int_plus; // +

    w_function *int_minus = new w_function();
    int_minus->inbuild = true;
    functions["!int.minus"] = int_minus; // -

    w_function *int_times = new w_function();
    int_times->inbuild = true;
    functions["!int.times"] = int_times; // *

    w_function *int_div = new w_function();
    int_div->inbuild = true;
    functions["!int.div"] = int_div; // /

    w_function *int_equals = new w_function();
    int_equals->inbuild = true;
    functions["!int.equals"] = int_equals; // ==

    w_function *int_lt = new w_function();
    int_lt->inbuild = true;
    functions["!int.lt"] = int_lt; // <

    w_function *int_gt = new w_function();
    int_gt->inbuild = true;
    functions["!int.gt"] = int_gt; // >

    w_function *int_ge = new w_function();
    int_ge->inbuild = true;
    functions["!int.ge"] = int_ge; // >=

    w_function *int_le = new w_function();
    int_le->inbuild = true;
    functions["!int.le"] = int_le; // <=

    w_function *int_mod = new w_function();
    int_mod->inbuild = true;
    functions["!int.mod"] = int_mod; // %

    w_function *int_pow = new w_function();
    int_pow->inbuild = true;
    functions["!int.power"] = int_pow; // ^

    w_function *int_ne = new w_function();
    int_ne->inbuild = true;
    functions["!int.ne"] = int_ne; // !=
}

node *visitor_separate_listed(node *parent)
{ // parent = ()
    node *total = new node("()");
    node *a = new node("*");
    for (auto n : parent->children)
    {
        if (n->value == ",")
        {
            total->push_child(a);
            a = new node("*");
        }
        else
        {
            a->push_child(n);
        }
    }
    if (a->children.size() != 0)
    {
        total->push_child(a);
    }
    return total;
}

bool visitor_is_inbuild(std::string name)
{
    if (name == "!print" || name == "!type" || name == "!system" || name == "!input")
    {
        return true;
    }
    return false;
}

w_variable *visitor_function_inbuild(std::string name, node *args, std::map<std::string, w_variable *> variables_t)
{
    if (name == "!print")
    {
        for (auto arg_t : args->children)
        {
            w_variable *arg = visitor_compute(arg_t, variables_t);
            print(arg, variables_t);
        }
        w_variable *r = new w_variable();
        r->type = 2;
        r->content = (void *)(new int(1));

        return r;
    }
    if (name == "!type")
    {
        if (args->children.size() != 1)
        {
            std::string err = "la fonction !type necessite un argument";
            error(err);
        }
        w_variable *arg = visitor_compute(args->children[0], variables_t);
        return type(arg);
    }
    if (name == "!input")
    {
        if (args->children.size() != 1)
        {
            std::string err = "la fonction !input necessite un argument";
            error(err);
        }
        w_variable *arg = visitor_compute(args->children[0], variables_t);
        return input(arg, variables_t);
    }
    return nullptr;
}

w_variable *visitor_funcall(std::string name, node *args, std::map<std::string, w_variable *> variables_t)
{

    if (!function_exist(name, functions))
    {
        std::string err = "la fonction " + name + " n'existe pas";
        error(err);
    }
    w_function *func = functions[name];
    args = visitor_separate_listed(args);
    if (visitor_is_inbuild(name))
    {
        return visitor_function_inbuild(name, args, variables_t);
    }
    else
    {
        for (int i = 0; i < args->children.size(); i++)
        {
            std::string arg_n = func->arguments->children[i]->children[0]->value;
            w_variable *arg_v = visitor_compute(args->children[i], variables_t);
            variables_t[arg_n] = arg_v;
        }
        if (name == "!list.en")
        {
            w_variable *res = visitor_visit(func->trunc, variables_t);
            return res;
        }
        w_variable *res = visitor_visit(func->trunc, variables_t);
        return res;
    }
}

w_variable *visitor_use_inbuild_int(int a, int b, std::string opera)
{
    if (opera == "+")
    {
        return int_plus(a, b);
    }
    if (opera == "-")
    {
        return int_minus(a, b);
    }
    if (opera == "*")
    {
        return int_times(a, b);
    }
    if (opera == "/")
    {
        return int_div(a, b);
    }
    if (opera == "<")
    {
        return int_lt(a, b);
    }
    if (opera == ">")
    {
        return int_gt(a, b);
    }
    if (opera == ">=")
    {
        return int_ge(a, b);
    }
    if (opera == "<=")
    {
        return int_le(a, b);
    }
    if (opera == "==")
    {
        return int_equals(a, b);
    }
    if (opera == "%")
    {
        return int_mod(a, b);
    }
    if (opera == "^")
    {
        return int_power(a, b);
    }
    if (opera == "!=")
    {
        return int_ne(a, b);
    }
    std::string err = "operateur inconnu : '" + opera + "'";
    error(err);
    // Throw an error, unknown operator
    return nullptr;
}

w_variable *visitor_use_inbuild_char(std::string a, std::string b, std::string opera)
{
    if (opera == "==")
    {
        return char_equals(a, b);
    }
    if (opera == "!=")
    {
        return char_ne(a, b);
    }
    if (opera == "+")
    {
        return char_plus(a, b);
    }
    std::string err = "operateur inconnu : '" + opera + "'";
    error(err);
    // Throw an error, unknown operator
    return nullptr;
}

w_variable *visitor_use_inbuild(w_variable *a, w_variable *b, std::string opera)
{
    // We check if both value have the same type
    if (a->get_type() != b->get_type())
    {
        w_variable *r = new w_variable();
        int *p = new int(0);
        r->type = 2; // int
        r->content = (void *)p;
        return r;
    }
    if (a->get_type() == "int")
    {
        return visitor_use_inbuild_int(a->convert_to_int(), b->convert_to_int(), opera);
    }
    if (a->get_type() == "char")
    {
        return visitor_use_inbuild_char(a->convert_to_char(), b->convert_to_char(), opera);
    }
    return nullptr;
}

w_variable *visitor_link_operator(w_variable *a, w_variable *b, std::string opera)
{
    std::map<std::string, w_variable *> variables_t;
    w_function *func;
    if (opera == "+")
        func = functions["!" + a->get_type() + ".plus"];
    else if (opera == "-")
        func = functions["!" + a->get_type() + ".minus"];
    else if (opera == "==")
        func = functions["!" + a->get_type() + ".equals"];
    else if (opera == "<")
        func = functions["!" + a->get_type() + ".lt"];
    else if (opera == ">")
        func = functions["!" + a->get_type() + ".gt"];
    else if (opera == "<=")
        func = functions["!" + a->get_type() + ".le"];
    else if (opera == ">=")
        func = functions["!" + a->get_type() + ".ge"];
    else if (opera == "%")
        func = functions["!" + a->get_type() + ".mod"];
    else if (opera == "/")
        func = functions["!" + a->get_type() + ".div"];
    else if (opera == "^")
        func = functions["!" + a->get_type() + ".power"];
    else if (opera == "!=")
        func = functions["!" + a->get_type() + ".ne"];
    else if (opera == "*")
        func = functions["!" + a->get_type() + ".times"];

    if (func->inbuild)
    { // This is an inbuild function
        return visitor_use_inbuild(a, b, opera);
    }
    variables_t["self"] = a;
    variables_t[func->arguments->children[0]->children[0]->value] = b;
    return visitor_visit(func->trunc, variables_t);
}

w_variable *visitor_new_object(std::string name, node *args, std::map<std::string, w_variable *> variables_t)
{
    if (!class_exist(name, classes))
    {
        std::string err = "la classe '"+name+"' n'existe pas";
        error(err);
    }
    w_class_template *temp = classes[name];
    w_object *r = new w_object();
    for (auto attr : temp->trunc->children[1]->children)
    {
        std::string name = attr->children[0]->value;
        node *comp = attr->children[1];
        w_variable *computed = visitor_compute(comp, variables_t);
        r->attribute_attribution(name, computed);
    }
    for (auto meth : temp->trunc->children[2]->children)
    {
        r->methods.push_back(meth->value);
    }
    r->name = temp->trunc->children[0]->value;
    w_variable *var = new w_variable();
    var->type = 3; // an object
    var->content = (void *)r;

    variables_t["self"] = var; // we define by default a varibale named self
    w_function *constructor = functions["!" + name + ".constructeur"];

    visitor_funcall("!" + name + ".constructeur", args, variables_t);

    return var;
}

w_variable *visitor_compute(node *c, std::map<std::string, w_variable *> variables_t)
{
    w_variable *last_value = new w_variable();
    last_value->type = 2;                       // By default, the type is int
    last_value->content = (void *)(new int(0)); // and the value is 0
    for (int i = 0; i < c->children.size(); i++)
    {
        std::string expr = c->children[i]->value;
        if (is_explicit(expr))
        {
            if (is_char(expr))
            {
                last_value = new w_variable();
                last_value->type = 1; // char
                std::string *r = new std::string(del_string(expr));
                last_value->content = (void *)r;
            }
            else if (is_digit(expr))
            {
                last_value = new w_variable();
                last_value->type = 2;                 // int
                int *r = new int(atoi(expr.c_str())); // we create an int pointer from the string
                last_value->content = (void *)r;      // we cast the int pointer as a void pointer
            }
        }
        else if (is_attributed(expr)) // Contains '.'
        {
            if (expr[0] == '!') // Is a funccall
            {
                std::string first_var_n;
                int index = 1;
                while (expr[index] != '.')
                {
                    first_var_n += expr[index];
                    index++;
                }

                if (!variable_exist(first_var_n, variables_t))
                {
                    std::string err = "la variable '" + first_var_n + "' n'existe pas";
                    error(err);
                }
                w_variable *first_var = variables_t[first_var_n];

                if (!first_var->is_object())
                {
                    std::string err = "la variable '" + first_var_n + "' doit être un objet";
                    error(err);
                }
                w_object *last_o = (w_object *)first_var->content;
                w_variable *last_var = first_var;
                std::string patent;
                for (int y = index; y < expr.size(); y++)
                {
                    if (expr[y] == '.')
                    {
                        if (!patent.empty())
                        {
                            if (!last_o->attribute_exist(patent))
                            {
                                std::string err = "'" + last_o->name + "' n'as pas d'attribut '" + patent + "'";
                                error(err);
                            }

                            last_var = last_o->get_attribute(patent);
                            if (!last_var->is_object())
                            {
                                std::string err = "la variable doit être un objet, pas du type " + last_var->get_type();
                                error(err);
                            }
                            last_o = (w_object *)last_var->content;
                            patent.clear();
                        }
                    }
                    else
                        patent += expr[y];
                }

                if (!patent.empty())
                {
                    std::string name = last_var->get_type();
                    std::map<std::string, w_variable *> variables_t_bis = std::map<std::string, w_variable *>(variables_t);
                    variables_t_bis["self"] = last_var;
                    last_value = visitor_funcall("!" + name + "." + patent, c->children[i + 1], variables_t_bis);
                    i++; // we increment by one because they are parenthesis
                }
            }
            else // No funcall
            {
                std::string first_var_n;
                int index = 0;
                while (expr[index] != '.')
                {
                    first_var_n += expr[index];
                    index++;
                }
                if (!variable_exist(first_var_n, variables_t))
                {
                    std::string err = "la variable '" + first_var_n + "' n'existe pas";
                    error(err);
                }
                w_variable *first_var = variables_t[first_var_n];
                if (!first_var->is_object())
                {
                    std::string err = "la variable '" + first_var_n + "' doit être un objet";
                    error(err);
                }
                w_object *last_o = (w_object *)first_var->content;

                std::string patent;
                for (int y = index; y < expr.size(); y++)
                {
                    if (expr[y] == '.')
                    {
                        if (!patent.empty())
                        {
                            if (!last_o->attribute_exist(patent))
                            {
                                std::string err = "'" + last_o->name + " n'as pas d'attribut '" + patent + "'";
                                error(err);
                            }
                            w_variable *last_var = last_o->get_attribute(patent);
                            if (!last_var->is_object())
                            {
                                std::string err = "la variable '" + last_o->name + "." + patent + " doit être un objet";
                                error(err);
                            }
                            last_o = (w_object *)last_var->content;
                            patent.clear();
                        }
                    }
                    else
                        patent += expr[y];
                }
                if (!patent.empty())
                {
                    if (!last_o->attribute_exist(patent))
                    {
                        std::string err = "'" + last_o->name + "' n'as pas d'attribut '" + patent + "'";
                        error(err);
                    }
                    last_value = last_o->get_attribute(patent);
                }
            }
        }
        else if (is_operator(expr))
        {
            // we look up to the next value

            node *p = new node("*");
            node *next = c->children[i + 1];
            while (i + 1 < c->children.size() and !is_operator(c->children[i + 1]->value))
            { // we look forward to either the next operator or the end of the calculation
                i++;
                next = c->children[i];
                p->push_child(next);
            }
            w_variable *next_val = visitor_compute(p, variables_t);
            last_value = visitor_link_operator(last_value, next_val, expr);
        }
        else if (expr[0] == '!')
        { // call a function
            if (c->children[i + 1]->value != "()")
            {
                std::string err = "l'appel d'une fonction doit etre suivie de ses arguments";
                error(err);
            }
            last_value = visitor_funcall(expr, c->children[i + 1], variables_t);
            i++; // we need to increment by one, because of the parenthesis
        }
        else if (variable_exist(expr, variables_t)) // variable_exist(expr, variables_t))
        {
            // we just take the value inside the variables table
            last_value = variables_t[expr];
        }
        else if (i + 1 < c->children.size()) // create a new object
        {                                    // just avoid segfaults ...
            if (c->children[i + 1]->value == "()")
            { // call a new object
                last_value = visitor_new_object(expr, c->children[i + 1], variables_t);
                i++;
            }
        }
        else if (expr == "()")
        {
            node *parenthis = c->children[i];
            last_value = visitor_compute(parenthis, variables_t);
        }
    }
    return last_value;
}

w_variable *visitor_keyword_return(node *trunc, std::map<std::string, w_variable *> variables_t)
{
    if (trunc->children.size() < 1)
    {
        std::string err = "le mot-clé 'renvoie' doit avoir au moins un argument";
        error(err);
    }
    node *arg = trunc->children[0];
    w_variable *result = visitor_compute(arg, variables_t);
    return result;
}

void visitor_funcdef(node *trunc)
{
    w_function *func = new w_function();
    std::string name = "!" + trunc->children[0]->value;
    func->set_arguments(visitor_separate_listed(trunc->children[1]));
    func->set_content(trunc->children[2]);
    functions[name] = func;
}

void visitor_vardef(node *trunc, std::map<std::string, w_variable *> &variables_t)
{
    // We pass the variables table by reference, so we can modify it

    std::string expr = trunc->children[0]->value;
    node *compute = trunc->children[1];

    w_variable *result = visitor_compute(compute, variables_t);
    if (is_attributed(expr)) // contains "."
    {
        std::string first_var_n;
        int index = 0;
        while (expr[index] != '.')
        {
            first_var_n += expr[index];
            index++;
        }
        if (!variable_exist(first_var_n, variables_t))
        {
            std::string err = "la variable " + first_var_n + " n'existe pas";
            error(err);
        }
        w_variable *first_var = variables_t[first_var_n];

        if (!first_var->is_object())
        {
            std::string err = "la variable " + first_var_n + " doit être un objet";
            error(err);
        }
        w_object *last_o = (w_object *)first_var->content;
        w_variable *last_var = first_var;
        std::string patent;
        for (int y = index; y < expr.size(); y++)
        {
            if (expr[y] == '.')
            {
                if (!patent.empty())
                {
                    if (!last_o->attribute_exist(patent))
                    {
                        std::string err = "'" + last_o->name + "' n'as pas d'attribut '" + patent + "'";
                        error(err);
                    }
                    last_var = last_o->get_attribute(patent);
                    if (!last_var->is_object())
                    {
                        std::string err = "la variable " + last_o->name + "." + patent + " doit être un objet";
                        error(err);
                    }
                    last_o = (w_object *)last_var->content;
                    patent.clear();
                }
            }
            else
                patent += expr[y];
        }
        if (!patent.empty())
        {
            if (!last_var->is_object())
            {
                std::string err = "la variable " + patent + " doit être un objet";
                error(err);
            }
            last_o = (w_object *)last_var->content;
            last_o->attribute_attribution(patent, result);
        }
    }
    else
    {
        variables_t[expr] = result;
    }
}

void visitor_classdef(node *trunc)
{
    w_class_template *c = new w_class_template();
    c->trunc = trunc;
    classes[trunc->children[0]->value] = c;
    for (auto f : trunc->children[2]->children)
    {
        // the name of the classe + "." + the name of the function
        f->children[0]->value = trunc->children[0]->value + "." + f->children[0]->value;
        visitor_funcdef(f); // we define the methode as a function
    }
}

void visitor_if_declaration(node *trunc, std::map<std::string, w_variable *> &variables_t)
{
    // We need to pass the variables table by reference, as we can modifie it later on
    // the if declaration :
    // si
    //      condition
    //          *
    //      {}
    //          ...
    node *condition = trunc->children[0];
    w_variable *result = visitor_compute(condition, variables_t);
    if (is_int_true(result))
    {
        visitor_visit_incode(trunc->children[1], variables_t);
    }
}

void visitor_forloop(node *trunc, std::map<std::string, w_variable *> &variables_t)
{
    // the for loop
    // pour
    //      i
    //      bornes
    //          x
    //          y
    //      {}
    //          ...
    std::string var_name = trunc->children[0]->value;
    node *code = trunc->children[2];

    node *conde = new node("*");
    conde->push_child(trunc->children[1]->children[0]);
    w_variable *first_b = visitor_compute(conde, variables_t);
    if (first_b->get_type() != "int")
    {
        std::string err = "les bornes de la boucle doivent être de type int";
        error(err);
    }
    int borne1 = first_b->convert_to_int();

    conde->children.clear();
    conde->push_child(trunc->children[1]->children[1]);
    w_variable *second_b = visitor_compute(conde, variables_t);
    if (second_b->get_type() != "int")
    {
        std::string err = "les bornes de la boucle doivent être de type int";
        error(err);
    }
    int borne2 = second_b->convert_to_int();

    w_variable *var = new w_variable();
    var->type = 2; // int

    int *var_cont = new int();
    for (int index = borne1; index < borne2; index++)
    {
        free(var_cont); // avoid memory leaks
        var_cont = new int(index);
        var->content = (void *)var_cont;
        variables_t[var_name] = var;
        std::string ret = visitor_visit_incode(code, variables_t);
        if (ret == "break")
        {
            return;
        }
    }
}

void visitor_whileloop(node *trunc, std::map<std::string, w_variable *> &variables_t)
{
    // the while loop
    // tant
    //      condition
    //          *
    //      {}
    //          ...
    node *condition = trunc->children[0];
    node *code = trunc->children[1];

    w_variable *cond = visitor_compute(condition, variables_t);
    while (is_int_true(cond))
    {
        if (visitor_visit_incode(code, variables_t) == "break")
            return;
        cond = visitor_compute(condition, variables_t);
    }
}

std::string visitor_visit_incode(node *trunc, std::map<std::string, w_variable *> &variables_t)
{
    // Use for loops and if statements
    std::string to_return;

    for (int i = 0; i < trunc->children.size(); i++)
    {
        node *instruction = trunc->children[i];
        if (instruction->value == "funcdef")
        {
            visitor_funcdef(instruction);
        }
        else if (instruction->value == "vardef")
        {
            visitor_vardef(instruction, variables_t);
        }
        else if (instruction->value == "classdef")
        {
            visitor_classdef(instruction);
        }
        else if (instruction->value == "ifdec")
        {
            visitor_if_declaration(instruction, variables_t);
        }
        else if (instruction->value == "forloop")
        {
            visitor_forloop(instruction, variables_t);
        }
        else if (instruction->value == "whileloop")
        {
            visitor_whileloop(instruction, variables_t);
        }
        else if (instruction->value == "()")
        { // we just computes what is in parenthesis
            visitor_compute(instruction, variables_t);
        }
        else if (instruction->value[0] == '!')
        { // we call the function
            if (trunc->children[i + 1]->value != "()")
            {
                std::string err = "l'appel d'une fonction doit etre suivie de ses arguments";
                error(err);
            }
            node *r = new node("*");
            r->push_child(instruction);
            r->push_child(trunc->children[i + 1]);
            visitor_compute(r, variables_t);
            i++; // we increment by one because of the parenthesis
        }
        // we need to do a function for keywords
        else if (instruction->value == "casse")
            return "break";
    }
    return to_return;
}

w_variable *visitor_visit(node *trunc, std::map<std::string, w_variable *> variables_t)
{
    if (!is_initialized_inbuild)
    {
        visitor_init_inbuild_functions();
    }
    for (auto in : trunc->children)
    {
        if (in->value == "whileloop")
            break;
    }
    w_variable *to_return = new w_variable();
    to_return->type = 2;
    to_return->content = (void *)(new int(1)); // by default, the result is an int ( 0 )

    for (int i = 0; i < trunc->children.size(); i++)
    {
        node *instruction = trunc->children[i];
        if (instruction->value == "funcdef")
        {
            visitor_funcdef(instruction);
        }
        else if (instruction->value == "vardef")
        {
            visitor_vardef(instruction, variables_t);
        }
        else if (instruction->value == "classdef")
        {
            visitor_classdef(instruction);
        }
        else if (instruction->value == "ifdec")
        {
            visitor_if_declaration(instruction, variables_t);
        }
        else if (instruction->value == "forloop")
        {
            visitor_forloop(instruction, variables_t);
        }
        else if (instruction->value == "whileloop")
        {
            visitor_whileloop(instruction, variables_t);
        }
        else if (instruction->value == "()")
        { // we just computes what is in parenthesis
            visitor_compute(instruction, variables_t);
        }
        else if (instruction->value[0] == '!')
        { // we call the function
            if (trunc->children[i + 1]->value != "()")
            {
                std::string err = "l'appel d'une fonction doit etre suivie de ses arguments";
                error(err);
            }
            node *r = new node("*");
            r->push_child(instruction);
            r->push_child(trunc->children[i + 1]);
            visitor_compute(r, variables_t);
            i++; // we increment by one because of the parenthesis
        }
        // we need to do a function for keywords
        else if (instruction->value == "renvoie")
        { // The renvoie keyword
            w_variable *res = visitor_keyword_return(instruction, variables_t);
            return res;
        }
    }
    return to_return;
}