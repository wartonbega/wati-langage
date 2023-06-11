#include <vector>
#include <iostream>
#include <map>
#include <string>

#include <chrono>    
#include <ctime>

#include "../src/include/node.hpp"
#include "../src/include/variables.hpp"
#include "../src/include/visitor.hpp"

#include "../src/include/lexer.hpp"
#include "../src/include/parser.hpp"
#include "../src/include/visitor.hpp"

#include "../src/include/main.hpp"

extern "C" w_variable *et(std::vector<w_variable *> args, variable_table variables_t, std::string reference, int thread_id)
{
    if (args.size() != 2 || args[0]->get_type() != "int" || args[1]->get_type() != "int")
    {
        error("!et : doit avoir deux arguments de type 'int'", reference, thread_id);
    }


    int64_t a = *(int64_t *)(args[0]->content);
    int64_t b = *(int64_t *)(args[1]->content);

    w_variable *r = new w_variable();
    r->type = 2; // int
    r->content = (void *)(new int64_t(a && b));
    return r;
}

extern "C" w_variable* ou(std::vector<w_variable *> args, variable_table variables_t, std::string reference, int thread_id)
{
    if (args.size() != 2 || args[0]->get_type() != "int" || args[1]->get_type() != "int")
    {
        error("!ou : doit avoir deux arguments de type 'int'", reference, thread_id);
    }


    int64_t a = *(int64_t *)(args[0]->content);
    int64_t b = *(int64_t *)(args[1]->content);

    w_variable *r = new w_variable();
    r->type = 2; // int
    
    r->content = (void *)(new int64_t(a || b));
    return r;
}

extern "C" w_variable* pas(std::vector<w_variable *> args, variable_table variables_t, std::string reference, int thread_id)
{
    if (args.size() != 1 || args[0]->get_type() != "int")
    {
        error("!pas : doit avoir un argument de type 'int'", reference, thread_id);
    }

    int64_t a = *(int64_t *)(args[0]->content);

    w_variable *r = new w_variable();
    r->type = 2; // int
    r->content = (void *)(new int64_t(!a));
    return r;
}

extern "C" w_variable* quitte(std::vector<w_variable *> args, variable_table variables_t, std::string reference, int thread_id)
{
    if (args.size() != 1 || args[0]->get_type() != "int")
    {
        error("!quitte : doit avoir un argument de type 'int'", reference, thread_id);
    }

    int64_t a = *(int64_t *)(args[0]->content);
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

extern "C" w_variable* temps(std::vector<w_variable *> args, variable_table &variables_t, std::string reference, int thread_id)
{
    // we get the current time in milliseconds since the epoch
    int64_t ms = duration_cast< milliseconds >(system_clock::now().time_since_epoch()).count();
    return new w_variable(ms);
}

extern "C" w_variable* execute(std::vector<w_variable *> args, variable_table variables_t, std::string reference, int thread_id)
{   
    // execute some wati-code
    if (args.size() != 1 || args[0]->get_type() != "char")
    {
        error("!execute : doit avoir un argument de type 'char'", reference, thread_id);
    }    
    std::string r = args[0]->convert_to_char(); // the code itself

    std::string filename = cut_error_ref(reference)[0];
	std::vector<std::string> ref;
	std::vector<std::string> lexemes = lexer(r, ref, filename);

	node *ast = parser(lexemes, "main", ref, filename + "1:1");

    visitor_visit_incode(ast, &variables_t, 0);
    return new w_variable(0);
}

extern "C" w_variable* avertissement(std::vector<w_variable *> args, variable_table variables_t, std::string reference, int thread_id)
{
    if (args.size() != 1 || args[0]->get_type() != "char")
    {
        error("!execute : doit avoir un argument de type 'char'", reference, thread_id);
    }    
    std::string a = args[0]->convert_to_char();
    warning(a, reference);
    return new w_variable(0);
}

std::vector<w_variable *> arguments_from_list(w_variable* list, variable_table variables_t, std::string reference, int thread_id)
{
    // The list must be orderd like this :
    // (arg1, arg2, ...)
    w_object *o = (w_object *)list->content;
    w_variable *len_var = o->get_attribute("taille");
    std::vector<w_variable *> arguments;
    int64_t len = *(int64_t *)len_var->content;
    

    for (int i = 0; i < len; i++)
    {
        node *arg_bis = new node("*");
        arg_bis->push_child(new node(std::to_string(i)));
        arguments.push_back(visitor_funcall_methode("!list.en", arg_bis, variables_t, &variables_t, list, thread_id));
    }
    return arguments;
}

extern "C" w_variable* essaie(std::vector<w_variable *> args, variable_table variables_t, std::string reference, int thread_id)
{
    /* 
    essaie prend deux arguments (de type fonction)
    Le premier est la fonction qui doit être exécuté dans tous les cas
    Le second est la fonction qui est exécuté en cas d'erreur. Elle doit prendre un argument : l'erreur
    */
    if (args.size() != 3 || args[0]->get_type() != "fonction" || args[1]->get_type() != "list" || args[2]->get_type() != "fonction")
    {
        error("!execute : doit avoir trois arguments de type 'fonction', 'list', et 'fonction'", reference, thread_id);
    }
    std::string f1_name = *(std::string *)args[0]->content;
    w_variable* function_args = args[1];
    std::string f2_name = *(std::string *)args[2]->content;
    try
    {
        std::vector<w_variable *> f_a = arguments_from_list(function_args, variables_t, reference, thread_id);
        w_function *f1 = functions[f1_name];
        variable_table variable_table_bis = prepare_arguments(f_a, f1->arguments, variables_t);
        
        visitor_visit(f1->trunc, variable_table_bis, thread_id);
    }
    catch (w_variable *error)
    {
        std::vector<w_variable *> error_func_arg;
        error_func_arg.push_back(error);
        w_function *f2 = functions[f2_name];
        variable_table variable_table_bis = prepare_arguments(error_func_arg, f2->arguments, variables_t);
        visitor_visit(f2->trunc, variable_table_bis, thread_id);
    }
    return new w_variable(0);
}