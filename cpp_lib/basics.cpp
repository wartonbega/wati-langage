#include <vector>
#include <iostream>
#include <map>
#include <string>

#include <chrono>    
#include <ctime>

#include "../src/include/node.hpp"
#include "../src/include/variables.hpp"
#include "../src/include/visitor.hpp"

extern "C" w_variable *et(std::vector<w_variable *> args, variable_table variables_t, std::string reference, int thread_id)
{
    if (args.size() != 2 || args[0]->get_type() != "int" || args[1]->get_type() != "int")
    {
        error("!et : doit avoir deux arguments de type 'int'", reference, thread_id);
    }


    int a = *(int *)(args[0]->content);
    int b = *(int *)(args[1]->content);

    w_variable *r = new w_variable();
    r->type = 2; // int
    r->content = (void *)(new int(a && b));
    return r;
}

extern "C" w_variable* ou(std::vector<w_variable *> args, variable_table variables_t, std::string reference, int thread_id)
{
    if (args.size() != 2 || args[0]->get_type() != "int" || args[1]->get_type() != "int")
    {
        error("!ou : doit avoir deux arguments de type 'int'", reference, thread_id);
    }


    int a = *(int *)(args[0]->content);
    int b = *(int *)(args[1]->content);

    w_variable *r = new w_variable();
    r->type = 2; // int
    
    r->content = (void *)(new int(a || b));
    return r;
}

extern "C" w_variable* pas(std::vector<w_variable *> args, variable_table variables_t, std::string reference, int thread_id)
{
    if (args.size() != 1 || args[0]->get_type() != "int")
    {
        error("!pas : doit avoir un argument de type 'int'", reference, thread_id);
    }

    int a = *(int *)(args[0]->content);

    w_variable *r = new w_variable();
    r->type = 2; // int
    r->content = (void *)(new int(!a));
    return r;
}

extern "C" w_variable* quitte(std::vector<w_variable *> args, variable_table variables_t, std::string reference, int thread_id)
{
    if (args.size() != 1 || args[0]->get_type() != "int")
    {
        error("!quitte : doit avoir un argument de type 'int'", reference, thread_id);
    }

    int a = *(int *)(args[0]->content);
    exit(a);
    return nullptr;
}

extern "C" w_variable* attributs(std::vector<w_variable *> args, variable_table variables_t, std::string reference, int thread_id)
{
    if (args.size() != 1 || !args[0]->is_object())
    {
        error("!attributs : doit avoir un argument de type 'objet'", reference, thread_id);
    }
    w_object *o = (w_object *)args[0]->content;

    w_variable *list = visitor_new_object("list", new node("*"), variables_t, thread_id); // list of attributs

    for (auto var : o->attributes->vars)
    {
        w_variable *var_name = new w_variable(std::get<0>(var));

        variable_table variables_t_bis = variable_table(variables_t);
        variables_t_bis.assign("self", list, thread_id);
        variables_t_bis.assign("content", var_name, thread_id);

        w_function *plus = functions["!list.plus"];
        visitor_visit(plus->trunc, variables_t_bis, thread_id);
    }
    return list;
}

extern "C" w_variable* methodes(std::vector<w_variable *> args, variable_table variables_t, std::string reference, int thread_id)
{
    if (args.size() != 1 || !args[0]->is_object())
    {
        error("!methodes : doit avoir un argument de type 'objet'", reference, thread_id);
    }
    w_object *o = (w_object *)args[0]->content;

    w_variable *list = visitor_new_object("list", new node("*"), variables_t, thread_id); // list of attributs

    std::string base = "!" + o->name;
    for (auto var : functions)
    {
        std::string name = std::get<0>(var);
        if (name.compare(0, base.size(), base) == 0)
        {
            w_variable *var_name = new w_variable(name);

            variable_table variables_t_bis = variable_table(variables_t);
            variables_t_bis.assign("self", list, thread_id);
            variables_t_bis.assign("content", var_name, thread_id);

            w_function *plus = functions["!list.plus"];
            visitor_visit(plus->trunc, variables_t_bis, thread_id);
        }
    }
    return list;
}

extern "C" w_variable* doc(std::vector<w_variable *> args, variable_table variables_t, std::string reference, int thread_id)
{
    if (args.size() != 1 || args[0]->get_type() != "fonction")
    {
        error("!doc : doit avoir un argument de type 'fonction'", reference, thread_id);
    }
    std::string name = *(std::string *)args[0]->content;
    std::string documentation = name + " : ";

    if (visitor_is_inbuild(name))
    {
        for (int i = 0; i < inbuild_funcs.size(); i ++)
        {
            if (inbuild_funcs[i] == name)
            {
                documentation = inbuild_funcs_documentation[i];
            }
        }
    }
    else
    {
        if (function_exist(name, functions))
        {
            if (functions_documentation.find(name) != functions_documentation.end())
            {
                documentation += functions_documentation[name];
            }
            else 
            {
                documentation += "la fonction " +name + " n'as pas de documentation";
            }
        }
        else 
        {
            std::string err = "la fonction '" + name + "' n'existe pas";
            error(err, reference, thread_id);
        }
    }
    if (documentation.empty())
    {
        documentation += "la fonction " +name + " n'as pas de documentation"; 
    }

    return new w_variable(documentation);
}

extern "C" w_variable* dirs(std::vector<w_variable *> args, variable_table variables_t, std::string reference, int thread_id)
{
    w_variable *list = visitor_new_object("list", new node("*"), variables_t, thread_id); // list of attributs

    for (auto var : variables_t.vars)
    {
        std::string name = std::get<0>(var);
        w_variable *var_name = new w_variable(name);

        variable_table variables_t_bis = variable_table(variables_t);
        variables_t_bis.assign("self", list, thread_id);
        variables_t_bis.assign("content", var_name, thread_id);

        w_function *plus = functions["!list.plus"];
        visitor_visit(plus->trunc, variables_t_bis, thread_id);
    }
    return list;
}

using namespace std::chrono;

extern "C" w_variable* temps(std::vector<w_variable *> args, variable_table variables_t, std::string reference, int thread_id)
{
    // we get the current time in milliseconds since the epoch
    int64_t ms = duration_cast< milliseconds >(system_clock::now().time_since_epoch()).count();
    return new w_variable(ms);
}