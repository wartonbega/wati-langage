#include <iostream>
#include <stack>
#include <dlfcn.h>

#include "include/visitor.hpp"
#include "include/variables.hpp"
#include "include/types.hpp"
#include "include/inbuilds.hpp"
#include "include/main.hpp"
#include "include/lexer.hpp"
#include "include/parser.hpp"
#include "include/trackers.hpp"

std::map<std::string, w_function *> functions;
std::map<std::string, std::string> functions_documentation;
std::map<std::string, w_class_template *> classes;

std::vector<std::string> inbuild_funcs;
std::vector<std::string> inbuild_funcs_documentation;

void *cpp_lib_handler = dlopen("/usr/local/lib/wati/cpp_lib/libs.so", RTLD_LAZY);

bool is_initialized_inbuild = false;

int thread_utilized = 1;

std::vector<std::stack<std::string> *> references;
std::vector<std::string> included;

variable_table prepare_arguments(std::vector<w_variable *> vars, node *trunc, variable_table variables_t)
{
    variable_table var_bis = variable_table(variables_t);
    for (int i = 0; i < trunc->children.size(); i++)
    {
        var_bis.assign(trunc->children[i]->children[0]->value, vars[i], 0);
    }
    return var_bis;
}

std::stack<std::string> *what_reference(int thread_id)
{
    return references[thread_id];
}

std::string ref_pop(int thread_id)
{
    std::string s = references[thread_id]->top();
    references.pop_back();
    return s;
}


bool parentethis_is_listed(node *trunc)
{
    for (auto i : trunc->children)
    {
        if (i->value == ",")
        {
            return true;
        }
    }
    return false;
}

std::vector<std::string> cut_error_ref(std::string ref)
{ //"filename:line:column" => ["filename", "line", "column"]
    int i = 0;
    std::string filename;
    while (ref[i] != ':')
    {
        filename += ref[i];
        i++;
    }
    i++;
    std::string line;
    while (ref[i] != ':')
    {
        line += ref[i];
        i++;
    }
    i++;
    std::string col;
    while (i < ref.size())
    {
        col += ref[i];
        i++;
    }
    std::vector<std::string> r;
    r.push_back(filename);
    r.push_back(line);
    r.push_back(col);
    return r;
}

int make_floor(int a, int b)
{
    if (a < b)
        return b;
    return a;
}

int make_max(int a, int b)
{
    if (a > b)
        return b;
    return a;
}

std::string give_file_error(std::string filename, int line, int column, std::string full_ref)
{
    std::string file_content = open_file(filename.c_str()) + "\n";
    if (file_content == "file_not_found")
        return "";
    std::vector<std::string> lines;
    std::string ret;
    int i = 0;
    while (i < file_content.size())
    {
        if (file_content[i] == '\n')
        {
            lines.push_back(ret);
            ret = "";
        }
        else
        {
            ret += file_content[i];
        }
        i++;
    }

    int min_line = make_floor(line - 3, 0);
    int max_line = make_max(line + 3, lines.size());
    std::string rend = TERMINAL_GREEN + full_ref + "\n";

    rend += TERMINAL_BOLDWHITE;
    for (int i = min_line; i < max_line; i++)
    {
        std::string patent = std::to_string(i + 1) + " | " + lines[i];
        if (i + 1 == line)
        {
            std::string head = "> " + patent + TERMINAL_BOLDWHITE;
            int p_len = ("> " + std::to_string(i + 1) + " | ").size();
            patent = TERMINAL_BOLDRED + head;
            std::string r;
            for (int f = 0; f < column + p_len - 1; f++)
            {
                r += " ";
            }
            r += TERMINAL_BOLDRED + "^" + TERMINAL_BOLDWHITE;
            patent += "\n" + r;
        }
        rend += patent + "\n";
    }
    rend += TERMINAL_RESET;
    return rend;
}

void error(std::string err, std::string ref, int thread_id)
{
    w_variable *erreur = new w_variable();
    erreur->type = T_OBJECT;
    w_object *o = new w_object();
    o->name = "w_erreur";
    erreur->content = (void *)o;

    if (!class_exist("w_erreur", classes))
    {
        std::cout << "La classe w_erreur n'a pas été correctement importée. L'utilisation du wati en dépend" << std::endl;
        exit(1);
    }
    
    std::vector<std::string> c_ref = cut_error_ref(ref);
    std::string filename = c_ref[0];

    std::string ref_error;

    int line;
    int col;
    try // To avoid 'libc++abi: terminating with uncaught exception of type std::invalid_argument: stoi: no conversion'
    {
        line = stoi(c_ref[1]);
        col = stoi(c_ref[2]);
    }
    catch (std::invalid_argument err)
    {
    }

    ref_error += give_file_error(filename, line, col, ref) + "\n\n";

    while (!(what_reference(thread_id))->empty())
    {
        std::string s_ref = ref_pop(thread_id);
        std::string s_fln = cut_error_ref(s_ref)[0];
        int s_lne;
        int s_col;
        try // To avoid error : 'libc++abi: terminating with uncaught exception of type std::invalid_argument: stoi: no conversion'
        {
            s_lne = stoi(cut_error_ref(s_ref)[1]);
            s_col = stoi(cut_error_ref(s_ref)[2]);
        }
        catch (std::invalid_argument err)
        {
        }
        if (!(what_reference(thread_id))->empty())
            (what_reference(thread_id))->pop();
        ref_error += TERMINAL_BOLDCYAN + "\nréférencé depuis :\n" + give_file_error(s_fln, s_lne, s_col, s_ref);
    }
    variable_table vt = variable_table();
    w_variable *w_err = new w_variable(err);
    w_variable *w_type = new w_variable("");
    w_variable *w_ref = new w_variable(ref);
    w_variable *w_appel = new w_variable(ref_error);
    vt.assign("erreur", w_err, thread_id);
    vt.assign("type", w_type, thread_id);
    vt.assign("reference", w_ref, thread_id);
    vt.assign("appel", w_appel, thread_id);
    vt.assign("self", erreur, thread_id);
    w_function *func = functions["!w_erreur.constructeur"];
    visitor_visit(func->trunc, vt, thread_id);
    throw erreur;
}

void warning(std::string err, std::string ref)
{
    std::cout << "\n";
    std::string err_s = TERMINAL_BOLDYELLOW + ref + TERMINAL_BOLDBLACK + " avertissement : " + TERMINAL_BOLDYELLOW + err;
    std::vector<std::string> c_ref = cut_error_ref(ref);
    std::string filename = c_ref[0];

    int line;
    int col;
    try // To avoid 'libc++abi: terminating with uncaught exception of type std::invalid_argument: stoi: no conversion'
    {
        line = stoi(c_ref[1]);
        col = stoi(c_ref[2]);
    }
    catch (std::invalid_argument err)
    {
    }

    std::cout << err_s << std::endl;
    std::cout << give_file_error(filename, line, col, ref) << "\n\n";
}

bool function_exist(std::string name, std::map<std::string, w_function *> funcs)
{
    if (funcs.find(name) != funcs.end() || visitor_is_inbuild(name))
    {
        return true;
    }
    return false;
}

bool class_exist(std::string name, std::map<std::string, w_class_template *> classes)
{
    if (classes.find(name) != classes.end() || visitor_is_inbuild(name))
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

    w_function *char_minus = new w_function();
    char_minus->inbuild = true;
    functions["!char.minus"] = char_minus; // -

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


    // we defines all the inbuild functions and include theirs documentation
    inbuild_funcs.push_back("!print");
    inbuild_funcs_documentation.push_back("Imprime sur la sortie standard les arguments. !print(var1, var2, ...) -> 0");
    inbuild_funcs.push_back("!type");
    inbuild_funcs_documentation.push_back("Renvoie le type de la variable en 'char'. !type( var ) -> 'char'");
    inbuild_funcs.push_back("!system");
    inbuild_funcs_documentation.push_back("Execute la commande. !system( var(char) ) -> 0");
    inbuild_funcs.push_back("!input");
    inbuild_funcs_documentation.push_back("Attend une entrée de l'utilisateur de taille 1. !input( var(char) ) -> char");
    inbuild_funcs.push_back("!char");
    inbuild_funcs_documentation.push_back("Convertis la variable en type 'char'. !char( var ) -> char");
    inbuild_funcs.push_back("!c_len");
    inbuild_funcs_documentation.push_back("Renvoie la longueure d'une variable 'char'. !c_len( var(char) ) -> int");
    inbuild_funcs.push_back("!erreur");
    inbuild_funcs_documentation.push_back("Lance l'erreur. !erreur( var(char) ) -> int");
    inbuild_funcs.push_back("!sortie");
    inbuild_funcs_documentation.push_back("Finis le programme avec le code indiqué. !sortie( var(int) ) -> int");
    inbuild_funcs.push_back("!c_en");
    inbuild_funcs_documentation.push_back("Accède au caractère X d'une variable 'char'. !c_en( var(char) ) -> char");
    
    std::string fs = open_file("/usr/local/lib/wati/cpp_lib/functions");
    if (fs == "file_not_found")
    { // very, very bad
        std::string err = "interne : ne trouve pas '/usr/local/lib/wati/cpp_lib/functions' pour les fonctions près-fabriquée";
        error(err, what_reference(0)->top(), 0);
    }
    std::string patent;
    bool fill = true;
    std::string documentation;
    for (auto i : fs)
    {
        if (i == '\n')
        {
            inbuild_funcs.push_back(patent);
            inbuild_funcs_documentation.push_back(documentation);
            patent = "";
            documentation = "";
            fill = 1;
        }
        else if (std::string(1, i) == " " && fill)
        {
            fill = false;
        }
        else if (fill)
        {
            patent += i;
        }
        else if (!fill)
        {
            documentation += i;
        }
    }
    if (! patent.empty())
    {
        inbuild_funcs.push_back(patent);
        inbuild_funcs_documentation.push_back(documentation);
    }
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
    for (auto i : inbuild_funcs)
    {
        if (i == name)
        {
            return true;
        }
    }
    return false;
}

w_variable *visitor_function_inbuild(std::string name, node *args, variable_table variables_t, int thread_id)
{
    if (name == "!print")
    {
        for (auto arg_t : args->children)
        {
            w_variable *arg = visitor_compute(arg_t, &variables_t, thread_id);
            print(arg, variables_t.vars);
        }
        w_variable *r = new w_variable(0);

        return r;
    }
    if (name == "!type")
    {
        if (args->children.size() != 1)
        {
            std::string err = "la fonction !type necessite un argument";
            error(err, ref_pop(thread_id), thread_id);
        }
        w_variable *arg = visitor_compute(args->children[0], &variables_t, thread_id);
        return type(arg);
    }
    if (name == "!input")
    {
        if (args->children.size() != 1)
        {
            std::string err = "la fonction !input necessite un argument";
            error(err, ref_pop(thread_id), thread_id);
        }
        w_variable *arg = visitor_compute(args->children[0], &variables_t, thread_id);
        return input(arg, variables_t.vars);
    }
    if (name == "!system")
    {
        if (args->children.size() != 1)
        {
            std::string err = "la fonction !system necessite un argument";
            error(err, ref_pop(thread_id), thread_id);
        }
        w_variable *arg = visitor_compute(args->children[0], &variables_t, thread_id);
        return w_system(arg, variables_t.vars);
    }
    if (name == "!char")
    {
        if (args->children.size() != 1)
        {
            std::string err = "la fonction !char necessite un argument";
            error(err, ref_pop(thread_id), thread_id);
        }
        w_variable *arg = visitor_compute(args->children[0], &variables_t, thread_id);
        return w_char(arg, variables_t.vars, thread_id);
    }
    if (name == "!c_en")
    {
        if (args->children.size() != 2)
        {
            std::string err = "la fonction !c_en necessite un argument";
            error(err, ref_pop(thread_id), thread_id);
        }
        std::vector<w_variable *> arg_c;
        for (auto arg_t : args->children)
        {
            arg_c.push_back(visitor_compute(arg_t, &variables_t, thread_id));
        }
        if (arg_c[0]->get_type() != "char")
        {
            std::string err = "le premier argument de !c_en doit etre de type char";
            error(err, ref_pop(thread_id), thread_id);
        }

        if (arg_c[1]->get_type() != "int")
        {
            std::string err = "le deuxième argument de !c_en doit etre de type int";
            error(err, ref_pop(thread_id), thread_id);
        }
        return w_en(arg_c, variables_t.vars);
    }
    if (name == "!c_len")
    {
        if (args->children.size() != 1)
        {
            std::string err = "la fonction !c_len necessite un argument";
            error(err, ref_pop(thread_id), thread_id);
        }
        w_variable *arg = visitor_compute(args->children[0], &variables_t, thread_id);
        if (arg->get_type() != "char")
        {
            std::string err = "l'argument de !c_len doit etre de type char";
            error(err, ref_pop(thread_id), thread_id);
        }
        return c_len(arg);
    }
    if (name == "!erreur")
    {
        if (args->children.size() != 1)
        {
            std::string err = "la fonction !erreur necessite un argument";
            error(err, ref_pop(thread_id), thread_id);
        }
        w_variable *arg = visitor_compute(args->children[0], &variables_t, thread_id);
        w_error(arg, variables_t.vars, thread_id);
    }
    if (name == "!sortie")
    {
        if (args->children.size() != 1)
        {
            std::string err = "la fonction !sortie necessite un argument";
            error(err, ref_pop(thread_id), thread_id);
        }
        w_variable *arg = visitor_compute(args->children[0], &variables_t, thread_id);
        if (arg->get_type() != "int")
        {
            std::string err = "l'argument de !sortie doit etre de type int";
            error(err, ref_pop(thread_id), thread_id);
        }
        w_exit(arg);
    }
    
    // that means we are looking for a shared header
    std::string real_name = remove_function_call_prefix(name);
    w_variable * (*fonction)(std::vector<w_variable *>, variable_table, std::string, int);
    fonction = (w_variable* (*) (std::vector<w_variable *>, variable_table, std::string, int))dlsym(cpp_lib_handler, real_name.c_str());
    std::vector<w_variable *> args_c;

    for (auto arg_t : args->children)
    {
        w_variable *arg = visitor_compute(arg_t, &variables_t, thread_id);
        args_c.push_back(arg);
    }

    if (fonction == NULL)
    {
        std::string err = "ne peux pas charger la fonction '" + name + "', elle n'existe pas dans les fichiers compilés";
        error(err, what_reference(thread_id)->top(), thread_id);
    }

    return fonction(args_c, variables_t, what_reference(thread_id)->top(), thread_id);
}

std::string remove_function_call_prefix(std::string name)
{
    std::string r = "";
    for (int i = 1; i < name.size(); i++)
    {
        r += name[i];
    }
    return r;
}

w_variable *visitor_funcall(std::string name, node *args, variable_table variables_t, variable_table *variables_t_parent, int thread_id)
{
    // La table de variable parente sert à calculer les arguements, pour entre autre
    // que les pointeurs soient référencés au bon endroit
    w_function *func;
    std::string var_name = remove_function_call_prefix(name);
    if (variables_t.exist(var_name))
    {
        w_variable *func_var = variables_t.get(var_name);
        if (func_var->get_type() != "fonction")
        {
            std::string err = "le type '" + func_var->get_type() + "' ne peut pas être appelé";
            error(err, ref_pop(thread_id), thread_id);
        }

        name = *(std::string *)(func_var->content);
        if (!function_exist(name, functions))
        {
            std::string err = "la fonction " + name + " n'existe pas";
            error(err, ref_pop(thread_id), thread_id);
        }
    }
    if (!function_exist(name, functions))
    {
        std::string err = "la fonction '" + name + "' n'existe pas";
        error(err, what_reference(thread_id)->top(), thread_id);
    }
    func = functions[name];
    args = visitor_separate_listed(args);
    if (visitor_is_inbuild(name))
    {
        return visitor_function_inbuild(name, args, variables_t, thread_id);
    }
    else
    {
        if (func->arguments->children.size() != args->children.size())
        {
            std::string err = "la fonction " + name + " requiers " + std::to_string(func->arguments->children.size()) + " argument(s), " + std::to_string(args->children.size()) + " argument(s) as(ont) été indiqué(s)";
            error(err, ref_pop(thread_id), thread_id);
        }
        for (int i = 0; i < args->children.size(); i++)
        {
            std::string arg_n = func->arguments->children[i]->children[0]->value;
            w_variable *arg_v = visitor_compute(args->children[i], variables_t_parent, thread_id);
            // variables_t[arg_n] = arg_v; we use the variable asignement
            variables_t.assign(arg_n, arg_v, thread_id);
        }
        w_variable *res = visitor_visit(func->trunc, variables_t, thread_id);
        return res;
    }
}

w_variable *visitor_funcall_methode(std::string name, node *args, variable_table variables_t, variable_table *variables_t_parent, w_variable *self, int thread_id)
{

    if (!function_exist(name, functions))
    {
        std::string err = "la fonction " + name + " n'existe pas";
        error(err, ref_pop(thread_id), thread_id);
    }
    w_function *func = functions[name];
    args = visitor_separate_listed(args);
    if (visitor_is_inbuild(name))
    {
        return visitor_function_inbuild(name, args, variables_t, thread_id);
    }
    else
    {
        for (int i = 0; i < args->children.size(); i++)
        {
            std::string arg_n = func->arguments->children[i]->children[0]->value;
            w_variable *arg_v = visitor_compute(args->children[i], variables_t_parent, thread_id);
            variables_t.assign(arg_n, arg_v, thread_id);
            // variables_t[arg_n] = arg_v;
        }
        variables_t.assign("self", self, thread_id);
        // variables_t["self"] = self;
        w_variable *res = visitor_visit(func->trunc, variables_t, thread_id);
        return res;
    }
}

w_variable *visitor_use_inbuild_int(int64_t a, int64_t b, std::string opera, int thread_id)
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
    error(err, ref_pop(thread_id), thread_id);
    // Throw an error, unknown operator
    return nullptr;
}

w_variable *visitor_use_inbuild_char(std::string a, std::string b, std::string opera, int thread_id)
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
    if (opera == "-")
    {
        return char_minus(a, b);
    }
    std::string err = "operateur inconnu : '" + opera + "'";
    error(err, ref_pop(thread_id), thread_id);
    // Throw an error, unknown operator
    return nullptr;
}

w_variable *visitor_use_inbuild(w_variable *a, w_variable *b, std::string opera, int thread_id)
{
    // We check if both value have the same type
    if (a->get_type() != b->get_type())
    {
        if (opera == "!=")
        {
            w_variable *r = new w_variable(1);
            return r;
        }
        else
        {
            w_variable *r = new w_variable(0);
            return r;
        }
    }
    if (a->get_type() == "int")
    {
        return visitor_use_inbuild_int(a->convert_to_int(), b->convert_to_int(), opera, thread_id);
    }
    if (a->get_type() == "char")
    {
        return visitor_use_inbuild_char(a->convert_to_char(), b->convert_to_char(), opera, thread_id);
    }
    return nullptr;
}

w_variable *visitor_link_operator(w_variable *a, w_variable *b, std::string opera, int thread_id)
{
    variable_table *variables_t = new variable_table();
    w_function *func;
    if (opera == "+")
    {
        std::string f_name = "!" + a->get_type() + ".plus";
        if (!function_exist(f_name, functions))
        {
            std::string err = "il n'existe pas de fonction " + f_name;
            error(err, ref_pop(thread_id), thread_id);
        }
        func = functions[f_name];
    }
    else if (opera == "-")
    {
        std::string f_name = "!" + a->get_type() + ".minus";
        if (!function_exist(f_name, functions))
        {
            std::string err = "il n'existe pas de fonction " + f_name;
            error(err, ref_pop(thread_id), thread_id);
        }
        func = functions[f_name];
    }
    else if (opera == "==")
    {
        std::string f_name = "!" + a->get_type() + ".equals";
        if (!function_exist(f_name, functions))
        {
            std::string err = "il n'existe pas de fonction " + f_name;
            error(err, ref_pop(thread_id), thread_id);
        }
        func = functions[f_name];
    }
    else if (opera == "<")
    {
        std::string f_name = "!" + a->get_type() + ".lt";
        if (!function_exist(f_name, functions))
        {
            std::string err = "il n'existe pas de fonction " + f_name;
            error(err, ref_pop(thread_id), thread_id);
        }
        func = functions[f_name];
    }
    else if (opera == ">")
    {
        std::string f_name = "!" + a->get_type() + ".gt";
        if (!function_exist(f_name, functions))
        {
            std::string err = "il n'existe pas de fonction " + f_name;
            error(err, ref_pop(thread_id), thread_id);
        }
        func = functions[f_name];
    }
    else if (opera == "<=")
    {
        std::string f_name = "!" + a->get_type() + ".le";
        if (!function_exist(f_name, functions))
        {
            std::string err = "il n'existe pas de fonction " + f_name;
            error(err, ref_pop(thread_id), thread_id);
        }
        func = functions[f_name];
    }
    else if (opera == ">=")
    {
        std::string f_name = "!" + a->get_type() + ".ge";
        if (!function_exist(f_name, functions))
        {
            std::string err = "il n'existe pas de fonction " + f_name;
            error(err, ref_pop(thread_id), thread_id);
        }
        func = functions[f_name];
    }
    else if (opera == "%")
    {
        std::string f_name = "!" + a->get_type() + ".mod";
        if (!function_exist(f_name, functions))
        {
            std::string err = "il n'existe pas de fonction " + f_name;
            error(err, ref_pop(thread_id), thread_id);
        }
        func = functions[f_name];
    }
    else if (opera == "/")
    {
        std::string f_name = "!" + a->get_type() + ".div";
        if (!function_exist(f_name, functions))
        {
            std::string err = "il n'existe pas de fonction " + f_name;
            error(err, ref_pop(thread_id), thread_id);
        }
        func = functions[f_name];
    }
    else if (opera == "^")
    {
        std::string f_name = "!" + a->get_type() + ".power";
        if (!function_exist(f_name, functions))
        {
            std::string err = "il n'existe pas de fonction " + f_name;
            error(err, ref_pop(thread_id), thread_id);
        }
        func = functions[f_name];
    }
    else if (opera == "!=")
    {
        std::string f_name = "!" + a->get_type() + ".ne";
        if (!function_exist(f_name, functions))
        {
            std::string err = "il n'existe pas de fonction " + f_name;
            error(err, ref_pop(thread_id), thread_id);
        }
        func = functions[f_name];
    }
    else if (opera == "*")
    {
        std::string f_name = "!" + a->get_type() + ".times";
        if (!function_exist(f_name, functions))
        {
            std::string err = "il n'existe pas de fonction " + f_name;
            error(err, ref_pop(thread_id), thread_id);
        }
        func = functions[f_name];
    }

    if (func->inbuild)
    { // This is an inbuild function
        return visitor_use_inbuild(a, b, opera, thread_id);
    }

    variables_t->assign("self", a, thread_id);
    variables_t->assign(func->arguments->children[0]->children[0]->value, b, thread_id);

    return visitor_visit(func->trunc, *variables_t, thread_id);
}

w_variable *visitor_new_object(std::string name, node *args, variable_table variables_t, int thread_id)
{
    if (!class_exist(name, classes))
    {
        std::string err = "la classe '" + name + "' n'existe pas";
        error(err, ref_pop(thread_id), thread_id);
    }
    w_class_template *temp = classes[name];
    w_object *r = new w_object();
    for (auto attr : temp->trunc->children[1]->children)
    { // attibutes
        std::string name = attr->children[0]->value;
        node *comp = attr->children[1];
        w_variable *computed = visitor_compute(comp, &variables_t, thread_id);
        r->attribute_attribution(name, computed);
    }
    for (auto meth : temp->trunc->children[2]->children)
    { // methods
        r->methods.push_back(meth->value);
    }
    r->name = temp->trunc->children[0]->value;
    w_variable *var = new w_variable();
    var->type = T_OBJECT; // an object
    var->content = (void *)r;

    visitor_funcall_methode("!" + name + ".constructeur", args, variables_t, &variables_t, var, thread_id);
    return var;
}

w_variable *generate_function_variable(std::string name, int thread_id)
{
    if (!function_exist(name, functions))
    {
        std::string err = "la fonction " + name + " n'existe pas";
        error(err, what_reference(thread_id)->top(), thread_id);
    }
    w_variable *r = new w_variable();
    r->type = T_FONCTION;                                  // this is a function
    r->content = (void *)(new std::string(name)); // the content is just the name of the variable
    return r;
}

w_variable *visitor_compute(node *c, variable_table *variables_t, int thread_id)
{
    w_variable *last_value = new w_variable(0);
    bool unused = true;
    for (int i = 0; i < c->children.size(); i++)
    {
        (what_reference(thread_id))->push(c->children[i]->reference);
        std::string expr = c->children[i]->value;
        if (c->children[i]->pre_value != nullptr)
        {
            last_value = c->children[i]->pre_value;
            unused = false;
        }
        else if (is_explicit(expr))
        {
            if (is_char(expr))
            {
                last_value = new w_variable(del_string(expr));
            }
            else if (is_digit(expr))
            {
                
                last_value = new w_variable(atoi(expr.c_str()));
            }
            unused = false;
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
                w_variable *first_var;

                if (first_var_n == "")
                {
                    
                    first_var = last_value;
                }
                else
                {
                    if (!variables_t->exist(first_var_n))
                    {
                        std::string err = "la variable '" + first_var_n + "' n'existe pas";
                        error(err, c->children[i]->reference, thread_id);
                    }
                    first_var = variables_t->get(first_var_n);
                }

                if (!first_var->is_object())
                {
                    std::string err = "la variable '" + first_var_n + "' (" + *(std::string *)(w_char(first_var, variables_t->vars, thread_id)->content) + ") doit être un objet";
                    error(err, c->children[i]->reference, thread_id);
                }

                w_object *last_o;
                if (first_var->content != NULL)
                {
                    last_o = (w_object *)first_var->content;
                }
                else
                {
                    std::string err = "tentative d'accès à une partie de la mémoire interdite";
                    error(err, c->children[i]->reference, thread_id);
                }

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
                                error(err, c->children[i]->reference, thread_id);
                            }

                            last_var = last_o->get_attribute(patent);
                            if (!last_var->is_object())
                            {
                                std::string err = "la variable doit être un objet, pas du type " + last_var->get_type();
                                error(err, c->children[i]->reference, thread_id);
                            }
                            if (last_var->content != NULL)
                                last_o = (w_object *)last_var->content;
                            else
                            {
                                std::string err = "tentative d'accès à une partie de la mémoire interdite";
                                error(err, c->children[i]->reference, thread_id);
                            }
                            patent.clear();
                        }
                    }
                    else
                        patent += expr[y];
                }

                if (i + 1 >= c->children.size())
                {
                    std::string name = last_var->get_type();
                    std::string f_name = "!" + name + "." + patent;
                    
                    last_value = generate_function_variable(f_name, thread_id);
                    unused = false;
                    // we generate a function variable
                }
                else if (!patent.empty())
                {
                    std::string name = last_var->get_type();
                    std::string f_name = "!" + name + "." + patent;
                    if (!function_exist(f_name, functions))
                    {
                        if (last_o->attribute_exist(patent))
                        {
                            w_variable *f_var = last_o->get_attribute(patent);
                            if (f_var->get_type() == "fonction")
                            {
                                f_name = *(std::string *)(f_var->content);
                            }
                        }
                    }
                    variable_table variables_t_bis = variable_table(*variables_t);
                    
                    last_value = visitor_funcall_methode(f_name, c->children[i + 1], variables_t_bis, variables_t, last_var, thread_id);
                    unused = false;
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
                w_variable *first_var;
                if (first_var_n == "")
                {
                    first_var = last_value;
                }
                else
                {
                    if (!variables_t->exist(first_var_n))
                    {
                        std::string err = "la variable '" + first_var_n + "' n'existe pas";
                        error(err, c->children[i]->reference, thread_id);
                    }
                    first_var = variables_t->get(first_var_n);
                }   
                if (!first_var->is_object())
                {
                    std::string err = "la variable '" + first_var_n + "' (" + *(std::string *)(w_char(first_var, variables_t->vars, thread_id)->content) + ") doit être un objet";
                    error(err, c->children[i]->reference, thread_id);
                }

                w_object *last_o;
                if (first_var->content != NULL)
                    last_o = (w_object *)first_var->content;
                else
                {
                    std::string err = "tentative d'accès à une partie de la mémoire interdite";
                    error(err, c->children[i]->reference, thread_id);
                }

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
                                error(err, c->children[i]->reference, thread_id);
                            }
                            w_variable *last_var = last_o->get_attribute(patent);
                            if (!last_var->is_object())
                            {
                                std::string err = "la variable '" + last_o->name + "." + patent + " doit être un objet";
                                error(err, c->children[i]->reference, thread_id);
                            }
                            if (last_var->content != NULL)
                                last_o = (w_object *)last_var->content;
                            else
                            {
                                std::string err = "tentative d'accès à une partie de la mémoire interdite";
                                error(err, c->children[i]->reference, thread_id);
                            }
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
                        error(err, c->children[i]->reference, thread_id);
                    }
                    
                    last_value = last_o->get_attribute(patent);
                    unused = false;
                }
            }
        }
        else if (expr == "&")
        { // we create a tracker
            w_variable *track = new w_variable();
            track->type = T_TRACKER;
            if (i + 1 >= c->children.size())
            {
                std::string err = "l'operateur '&' (creation de traqueur) ne peux être utilisé que devant une variable";
                error(err, c->children[i]->reference, thread_id);
            }
            std::string e = c->children[i + 1]->value;
            i ++; // we increment by one 
            tracker *t = new tracker(e, variables_t);

            track->content = (void *)t;
            
            last_value = track;
            unused = false;
        }
        else if (expr == "*" && unused)
        { // this is the opposit of & : we get the value of the tracker
            node *p = new node("*");
            node *next = c->children[i + 1];
            while (i + 1 < c->children.size() and !is_operator(c->children[i + 1]->value))
            { // we look forward to either the next operator or the end of the calculation
                i++;
                next = c->children[i];
                p->push_child(next);
            }
            w_variable *val = visitor_compute(p, variables_t, thread_id);
            if (val->get_type() != "traqueur")
            {
                std::string err = "ne peux pas récupérer le contenu du traquage du type '" + val->get_type() + "'";
                error(err, c->children[i]->reference, thread_id);
            }
            tracker *t = (tracker *)val->content;
            
            last_value = t->value(thread_id);
            unused = false;
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
            w_variable *next_val = visitor_compute(p, variables_t, thread_id);
            last_value = visitor_link_operator(last_value, next_val, expr, thread_id);
            unused = false;
        }
        else if (expr[0] == '!')
        {// call a function
            if (i + 1 >= c->children.size() || c->children[i + 1]->value != "()") // does not call
            {
                
                last_value = generate_function_variable(c->children[i]->value, thread_id);
                unused = false;
            }
            else
            {
                
                last_value = visitor_funcall(expr, c->children[i + 1], *variables_t, variables_t, thread_id);
                unused = false;
                i++; // we need to increment by one, because of the parenthesis
            }
        }
        else if (variables_t->exist(expr)) // variable_exist(expr, variables_t))
        {
            // we just take the value inside the variables table
            
            last_value = variables_t->get(expr);
            unused = false;
        }
        else if (i + 1 < c->children.size() && c->children[i + 1]->value == "()") // create a new object
        {                                                                         // just avoid segfaults ...
            // call a new object
            
            last_value = visitor_new_object(expr, c->children[i + 1], *variables_t, thread_id);
            unused = false;
            i++;
        }
        else if (expr == "()")
        {
            if (parentethis_is_listed(c->children[i]))
            { // then we try to create a new list classe
                if (!class_exist("list", classes))
                {
                    std::string err = "la classe 'list' n'existe pas, on ne peux donc pas initialiser de nouvelles listes à partir de parenthèses.\nPeut être avez-vous oublié d'inclue la librairie standard (std.wati)";
                    error(err, c->children[i]->reference, thread_id);
                }

                w_variable *r = visitor_new_object("list", new node("()"), *variables_t, thread_id);

                std::string funcname = "!list.plus"; // the function to add elements to a list

                node *elements = visitor_separate_listed(c->children[i]); // it will be used as arguments

                for (auto parts : elements->children)
                {
                    visitor_funcall_methode(funcname, parts, *variables_t, variables_t, r, thread_id);
                }
                
                last_value = r; // we finally put the list as the last value
                unused = false;
            }
            else
            { // just compute what is in the parentethis
                node *parenthis = c->children[i];
                
                last_value = visitor_compute(parenthis, variables_t, thread_id);
                unused = false;
            }
        }
        else if (expr == "[]")
        {
            node *brack = c->children[i];
            w_variable *b_value = visitor_compute(brack, variables_t, thread_id);

            std::string name = "!" + last_value->get_type() + ".index";

            if (!function_exist(name, functions))
            {
                std::string err = "la fonction '" + name + "' n'existe pas";
                error(err, c->children[i]->reference, thread_id);
            }
            w_function *f = functions[name];

            variable_table variables_t_bis = variable_table(*variables_t);
            variables_t_bis.assign("self", last_value, thread_id);
            variables_t_bis.assign(f->arguments->children[0]->children[0]->value, b_value, thread_id);
            
            last_value = visitor_visit(f->trunc, variables_t_bis, thread_id);
            unused = false;
        }
        else
        {
            std::string err = "expression ou variable inconnue '" + expr + "'";
            error(err, c->children[i]->reference, thread_id);
        }
        if (!(what_reference(thread_id))->empty())
            (what_reference(thread_id))->pop();
    }
    return last_value;
}

w_variable *visitor_keyword_return(node *trunc, variable_table variables_t, int thread_id)
{
    (what_reference(thread_id))->push(trunc->reference);
    if (trunc->children.size() < 1)
    {
        std::string err = "le mot-clé 'renvoie' doit avoir au moins un argument";
        error(err, trunc->reference, thread_id);
    }
    node *arg = trunc->children[0];
    w_variable *result = visitor_compute(arg, &variables_t, thread_id);
    if (!(what_reference(thread_id))->empty())
        (what_reference(thread_id))->pop();
    return result;
}

void visitor_keyword_free(node *trunc, variable_table *variables_t, int thread_id)
{
    (what_reference(thread_id))->push(trunc->reference);
    if (trunc->children.size() < 1)
    {
        std::string err = "le mot-clé 'libere' doit avoir au moins un argument";
        error(err, trunc->reference, thread_id);
    }
    node *arg = trunc->children[0];
    w_variable *result = visitor_compute(arg, variables_t, thread_id);
    if (result->is_object())
    {
        for (auto i : variables_t->vars)
        {
            std::string key = std::get<0>(i);
            if (std::get<1>(i) == result)
            {
                variables_t->vars.erase(key);
                break;
            }
        }
        w_object *t;
        if (result->content != NULL)
            t = (w_object *)result->content;
        else
        {
            std::string err = "tentative d'accès à une partie de la mémoire interdite";
            error(err, trunc->reference, thread_id);
        }

        std::string name = "!" + t->name + ".destructeur"; // we call the desctructor
        if (function_exist(name, functions))
            visitor_funcall_methode(name, new node("*"), *variables_t, variables_t, result, thread_id);
    }

    std::map<std::string, w_variable *>::iterator it;
    for (it = variables_t->vars.begin(); it != variables_t->vars.end(); ++it)
    {
        if (it->second == result)
        {
            variables_t->vars.erase(it->first);
        }
    }

    delete result; // seems to cause problems with the multithreading
    if (!(what_reference(thread_id))->empty())
        (what_reference(thread_id))->pop();
}

void visitor_keyword_tache(node *trunc, variable_table variables_t, int thread_id)
{
    (what_reference(thread_id))->push(trunc->reference);
    if (trunc->children.size() < 1)
    {
        std::string err = "le mot-clé 'tache' doit avoir au moins un argument";
        error(err, trunc->reference, thread_id);
    }
    node *arg = trunc->children[0];
    w_variable *result = visitor_compute(arg, &variables_t, thread_id);

    if (result->get_type() != "fonction" && result->get_type() != "list")
    {
        std::string err = "l'argument attendu doit être du type 'fonction' ou 'list' (importable depuis la librairie standard), pas du type '" + result->get_type() + "'";
        error(err, trunc->reference, thread_id);
    }

    if (result->get_type() == "fonction")
    {
        std::string name = *(std::string *)result->content;
        visitor_funcall(name, new node("()"), variables_t, &variables_t, thread_id);
    }
    else if (result->get_type() == "list")
    {
        // The list must be orderd like this :
        // (!funcname, arg1, arg2, ...)
        node *arg = new node("*");
        arg->push_child(new node("0"));
        w_variable *potential_function = visitor_funcall_methode("!list.en", arg, variables_t, &variables_t, result, thread_id);

        w_object *o;
        if (result->content != NULL)
            o = (w_object *)(result->content);
        else
        {
            std::string err = "tentative d'accès à une partie de la mémoire interdite";
            error(err, trunc->reference, thread_id);
        }

        w_variable *len_var = o->get_attribute("taille");
        std::vector<w_variable *> arguments;
        int64_t len = *(int64_t *)len_var->content;

        for (int i = 1; i < len; i++)
        {
            node *arg_bis = new node("*");
            arg_bis->push_child(new node(std::to_string(i)));
            arguments.push_back(visitor_funcall_methode("!list.en", arg_bis, variables_t, &variables_t, result, thread_id));
            free((void *)arg_bis);
        }

        if (potential_function->get_type() != "fonction")
        {
            std::string err = "le premier élément de la liste doit être la fonction";
            error(err, trunc->reference, thread_id);
        }

        std::string name = *(std::string *)potential_function->content;
        if (!function_exist(name, functions))
        {
            std::string err = "la fonction '" + name + "' n'existe pas";
            error(err, trunc->reference, thread_id);
        }

        w_function *func = functions[name];
        variable_table variables_bis = prepare_arguments(arguments, func->arguments, variables_t);
        visitor_visit(func->trunc, variables_bis, thread_id);
    }

    if (!(what_reference(thread_id))->empty())
        (what_reference(thread_id))->pop();
}

bool visitor_is_included(std::string libname)
{
    for (auto i : included)
    {
        if (i == libname)
            return true;
    }
    return false;
}

void visitor_keyword_include(node *trunc, variable_table *variables_t, int thread_id)
{
    if (trunc->children.size() < 1)
    {
        std::string err = "le mot-clé 'inclue' doit avoir au moins un argument";
        error(err, trunc->reference, thread_id);
    }
    node *arg = trunc->children[0];
    w_variable *result = visitor_compute(arg, variables_t, thread_id);
    if (result->get_type() != "char")
    {
        std::string err = "le mot-clé 'inclue' doit avoir un argument de type 'char'";
        error(err, trunc->children[0]->reference, thread_id);
    }

    std::string filename = result->convert_to_char();
    std::string dir = base_dir;

    std::string filename2 = base_dir + filename;
    base_dir += separate_base_dir(filename);

    if (!visitor_is_included(filename2))
    {
        std::string r = open_file(filename2.c_str());
        if (r == "file_not_found") 
        { // if we don't find the file, then it is maybe in the lib dir
            filename2 = "/usr/local/lib/wati/lib/" + filename;
            if (visitor_is_included(filename2))
            { // meaning we already included it
                return;
            }
            r = open_file(filename2.c_str());
        }
        if (r == "file_not_found")
        { // it clearly doesn't exists
            std::string err = "fichier introuvable '" + filename2 + "'";
            error(err, trunc->children[0]->reference, thread_id);
        }

        included.push_back(filename2);
        std::vector<std::string> ref;
        std::vector<std::string> lexemes = lexer(r, ref, filename2);

        node *ast = parser(lexemes, filename2, ref, filename2 + "1:1");
        visitor_visit_incode(ast, variables_t, thread_id);
    }
    base_dir = dir;
}

void visitor_funcdef(node *trunc)
{
    what_reference(0)->push(trunc->reference);
    w_function *func = new w_function();
    std::string name = "!" + trunc->children[0]->value;
    func->set_arguments(visitor_separate_listed(trunc->children[1]));
    if (is_char(trunc->children[2]->children[0]->value))
    { // it is the description of the function, we can store it and remove it from the trunc
        // we remove the '"' 
        std::string description = del_string(trunc->children[2]->children[0]->value);
        trunc->children[2]->children.erase(trunc->children[2]->children.begin()); // we delete the first value of the function code
        functions_documentation[name] = description;
    }
    func->set_content(trunc->children[2]);
    functions[name] = func;
    if (!what_reference(0)->empty())
        what_reference(0)->pop();
}

void visitor_vardef(node *trunc, variable_table *variables_t, int thread_id)
{
    // We pass the variables table by reference, so we can modify it

    std::string expr = trunc->children[0]->value;
    node *compute = trunc->children[1];
    bool tracker_assignation = false;
    if (expr == "*")
    {
        if (trunc->children.size() != 3)
        {
            std::string err = "'*' ne peux pas être appliqué seul";
            error(err, trunc->children[0]->value, thread_id);
        }
        expr = trunc->children[1]->value;
        compute = trunc->children[2];
        tracker_assignation = true;
    }

    w_variable *result = visitor_compute(compute, variables_t, thread_id);
    if (is_attributed(expr)) // contains "."
    {
        std::string first_var_n;
        int index = 0;
        while (expr[index] != '.')
        {
            first_var_n += expr[index];
            index++;
        }
        if (!variables_t->exist(first_var_n))
        {
            std::string err = "la variable " + first_var_n + " n'existe pas";
            error(err, trunc->children[0]->reference, thread_id);
        }
        w_variable *first_var = variables_t->get(first_var_n);

        if (!first_var->is_object())
        {
            std::string err = "la variable " + first_var_n + " doit être un objet";
            error(err, trunc->children[0]->reference, thread_id);
        }
        w_object *last_o;
        if (first_var->content != NULL)
            last_o = (w_object *)first_var->content;
        else
        {
            std::string err = "tentative d'accès à une partie de la mémoire interdite";
            error(err, trunc->children[0]->reference, thread_id);
        }

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
                        error(err, trunc->children[0]->reference, thread_id);
                    }
                    last_var = last_o->get_attribute(patent);
                    if (!last_var->is_object())
                    {
                        std::string err = "la variable " + last_o->name + "." + patent + " doit être un objet";
                        error(err, trunc->children[0]->reference, thread_id);
                    }
                    if (last_var->content != NULL)
                        last_o = (w_object *)last_var->content;
                    else
                    {
                        std::string err = "tentative d'accès à une partie de la mémoire interdite";
                        error(err, trunc->children[0]->reference, thread_id);
                    }
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
                error(err, trunc->children[0]->reference, thread_id);
            }
            last_o = (w_object *)last_var->content;
            if (tracker_assignation)
            {
                if (!last_o->attribute_exist(patent))
                {
                    std::string err = "la variable '" + patent + "' n'existe pas";
                    error(err, trunc->children[1]->reference, thread_id);
                }

                w_variable *tra = last_o->get_attribute(patent);
                if (tra->get_type() != "traqueur")
                {
                    std::string err = "doit être du type 'traqueur', pas '" + tra->get_type() + "'";
                    error(err, trunc->children[1]->reference, thread_id);
                }
                tracker *t = (tracker *)tra->content;
                t->set_value(result, thread_id);
            }
            else
                last_o->attribute_attribution(patent, result);
        }
        else 
        {
            std::string err = "pas de suite donnée au '.'";
            error(err, trunc->reference, thread_id);
        }
    }
    else
    {
        if (tracker_assignation)
        {
            if (!variables_t->exist(expr))
            {
                std::string err = "la variable '" + expr + "' n'existe pas";
                error(err, trunc->children[1]->reference, thread_id);
            }
            w_variable *tra = variables_t->get(expr);
            if (tra->get_type() != "traqueur")
            {
                std::string err = "doit être du type 'traqueur', pas '" + tra->get_type() + "'";
                error(err, trunc->children[1]->reference, thread_id);
            }
            tracker *t = (tracker *)tra->content;
            t->set_value(result, thread_id);
        }
        else
            variables_t->assign(expr, result, thread_id);
    }
}

void visitor_classdef(node *trunc)
{
    what_reference(0)->push(trunc->reference);
    w_class_template *c = new w_class_template();
    c->trunc = trunc;
    classes[trunc->children[0]->value] = c;
    for (auto f : trunc->children[2]->children)
    {
        // the name of the classe + "." + the name of the function
        f->children[0]->value = trunc->children[0]->value + "." + f->children[0]->value;
        visitor_funcdef(f); // we define the methode as a function
    }
    if (!what_reference(0)->empty())
        what_reference(0)->pop();
}

std::tuple<std::string, w_variable *> visitor_if_declaration(node *trunc, variable_table *variables_t, int thread_id)
{
    // We need to pass the variables table by reference, as we can modifie it later on
    // the if declaration :
    // si
    //      condition
    //          *
    //      {}
    //          ...
    node *condition = trunc->children[0];
    w_variable *result = visitor_compute(condition, variables_t, thread_id);

    if (is_int_true(result))
    {
        std::tuple<std::string, w_variable *> ret = visitor_visit_incode(trunc->children[1], variables_t, thread_id);
        if (std::get<0>(ret) == "return")
        {
            return ret;
        }
        if (std::get<0>(ret) == "continue")
        {
            return ret;
        }
        if (std::get<0>(ret) == "break")
        {
            return ret;
        }
    }
    else
    {
        bool e = false;
        node *elseifs = trunc->children[2];
        for (int i = 0; i < elseifs->children.size(); i++)
        {
            node *elif = elseifs->children[i];
            w_variable *cond = visitor_compute(elif->children[0], variables_t, thread_id);
            if (is_int_true(cond))
            {
                e = true;
                std::tuple<std::string, w_variable *> ret = visitor_visit_incode(elif->children[1], variables_t, thread_id);
                if (std::get<0>(ret) == "return")
                {
                    return ret;
                }
                if (std::get<0>(ret) == "continue")
                {
                    return ret;
                }
                if (std::get<0>(ret) == "break")
                {
                    return ret;
                }
                break;
            }
        }
        if (!e) // meaning, there were no elif used
        {       // we call the else
            node *else_ = trunc->children[3];
            if (else_->children.size() != 0)
            {
                else_ = else_->children[0];
                std::tuple<std::string, w_variable *> ret = visitor_visit_incode(else_->children[0], variables_t, thread_id);
                if (std::get<0>(ret) == "return")
                {
                    return ret;
                }
                if (std::get<0>(ret) == "continue")
                {
                    return ret;
                }
                if (std::get<0>(ret) == "break")
                {
                    return ret;
                }
            }
        }
    }
    return std::tuple<std::string, w_variable *>{"", nullptr};
}

std::tuple<std::string, w_variable *> visitor_forloop(node *trunc, variable_table *variables_t, int thread_id)
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
    w_variable *first_b = visitor_compute(conde, variables_t, thread_id);
    if (first_b->get_type() != "int")
    {
        std::string err = "les bornes de la boucle doivent être de type int";
        error(err, conde->children[0]->reference, thread_id);
    }
    int borne1 = first_b->convert_to_int();

    conde->children.clear();
    conde->push_child(trunc->children[1]->children[1]);
    w_variable *second_b = visitor_compute(conde, variables_t, thread_id);
    if (second_b->get_type() != "int")
    {
        std::string err = "les bornes de la boucle doivent être de type int";
        error(err, conde->children[0]->reference, thread_id);
    }
    int borne2 = second_b->convert_to_int();

    w_variable *var;

    int *var_cont = new int();
    for (int index = borne1; index < borne2; index++)
    {
        // free(var_cont); // avoid memory leaks
        var = new w_variable(index);
        variables_t->assign(var_name, var, thread_id);
        // variables_t[var_name] = var;
        std::tuple<std::string, w_variable *> ret = visitor_visit_incode(code, variables_t, thread_id);
        if (std::get<0>(ret) == "break")
        {
            return std::tuple<std::string, w_variable *>{"", nullptr};
        };
        if (std::get<0>(ret) == "return")
        {
            return ret;
        }
        if (std::get<0>(ret) == "continue")
        {
            continue;
        }
    }
    return std::tuple<std::string, w_variable *>{"", nullptr};
}

std::tuple<std::string, w_variable *> visitor_whileloop(node *trunc, variable_table *variables_t, int thread_id)
{
    // the while loop
    // tant
    //      condition
    //          *
    //      {}
    //          ...
    node *condition = trunc->children[0];
    node *code = trunc->children[1];

    w_variable *cond = visitor_compute(condition, variables_t, thread_id);
    while (is_int_true(cond))
    {
        std::tuple<std::string, w_variable *> ret = visitor_visit_incode(code, variables_t, thread_id);
        if (std::get<0>(ret) == "break")
        {
            return std::tuple<std::string, w_variable *>{"", nullptr};
        }
        if (std::get<0>(ret) == "return")
        {
            return ret;
        }
        if (std::get<0>(ret) == "continue")
            continue;
        cond = visitor_compute(condition, variables_t, thread_id);
    }
    return std::tuple<std::string, w_variable *>{"", nullptr};
}

std::tuple<std::string, w_variable *> visitor_visit_incode(node *trunc, variable_table *variables_t, int thread_id)
{
    // Use for loops and if statements
    std::tuple<std::string, w_variable *> to_return = {"", nullptr};

    for (int i = 0; i < trunc->children.size(); i++)
    {
        node *instruction = trunc->children[i];
        if (instruction->value == "funcdef")
        {
            visitor_funcdef(instruction);
        }
        else if (instruction->value == "vardef")
        {
            visitor_vardef(instruction, variables_t, thread_id);
        }
        else if (instruction->value == "classdef")
        {
            visitor_classdef(instruction);
        }
        else if (instruction->value == "ifdec")
        {
            std::tuple<std::string, w_variable *> ret = visitor_if_declaration(instruction, variables_t, thread_id);
            if (std::get<0>(ret) == "return")
            {
                return ret;
            }
            if (std::get<0>(ret) == "continue")
            {
                return ret;
            }
            if (std::get<0>(ret) == "break")
            {
                return ret;
            }
        }
        else if (instruction->value == "forloop")
        {
            std::tuple<std::string, w_variable *> ret = visitor_forloop(instruction, variables_t, thread_id);
            if (std::get<0>(ret) == "return")
            {
                return ret;
            }
        }
        else if (instruction->value == "whileloop")
        {
            std::tuple<std::string, w_variable *> ret = visitor_whileloop(instruction, variables_t, thread_id);
            if (std::get<0>(ret) == "return")
            {
                return ret;
            }
        }
        else if (instruction->value == "()")
        { // we just computes what is in parenthesis
            visitor_compute(instruction, variables_t, thread_id);
        }
        else if (instruction->value[0] == '!')
        { // we call the function

            if (i + 1 < trunc->children.size() && trunc->children[i + 1]->value != "()")
            {
                std::string err = "l'appel d'une fonction doit etre suivie de ses arguments";
                error(err, instruction->reference, thread_id);
            }
            node *r = new node("*");
            r->push_child(instruction);
            r->push_child(trunc->children[i + 1]);
            visitor_compute(r, variables_t, thread_id);
            i++; // we increment by one because of the parenthesis
        }
        else if (instruction->value == "renvoie")
        { // The renvoie keyword
            w_variable *res = visitor_keyword_return(instruction, *variables_t, thread_id);
            return std::tuple<std::string, w_variable *>{"return", res};
        }
        else if (instruction->value == "libere")
        { // The libere keyword
            visitor_keyword_free(instruction, variables_t, thread_id);
        }
        else if (instruction->value == "casse")
        {
            return std::tuple<std::string, w_variable *>{"break", nullptr};
        }
        else if (instruction->value == "continue") // just continue the loop for the next iteration
        {
            return std::tuple<std::string, w_variable *>{"continue", nullptr};
        }
        else if (instruction->value == "inclue")
        { // The renvoie keyword
            visitor_keyword_include(instruction, variables_t, thread_id);
        }
        else if (instruction->value != ";")
        {
            std::string err = "instruction inconnue '" + instruction->value + "'. \nAttention, l'instruction est peut-être inconnue dans le contexte (boucle, appel de fonction ect...). \nSi ça fait partie d'un calcule, il faut utiliser des parenthèses.";
            error(err, instruction->reference, thread_id);
        }
    }
    return to_return;
}

w_variable *visitor_visit(node *trunc, variable_table variables_t, int thread_id)
{
    //variables_t.use(); // Incrémente de 1 les variables qui sont crées pour le garbage collector

    int thread_number = 0;
    std::thread thread_in_scope[100];
    if (!is_initialized_inbuild)
    {
        visitor_init_inbuild_functions();
    }
    w_variable *to_return = new w_variable(0);

    for (int i = 0; i < trunc->children.size(); i++)
    {
        node *instruction = trunc->children[i];
        if (instruction->value == "funcdef")
        {
            visitor_funcdef(instruction);
        }
        else if (instruction->value == "vardef")
        {
            visitor_vardef(instruction, &variables_t, thread_id);
        }
        else if (instruction->value == "classdef")
        {
            visitor_classdef(instruction);
        }
        else if (instruction->value == "ifdec")
        {
            std::tuple<std::string, w_variable *> ret = visitor_if_declaration(instruction, &variables_t, thread_id);
            if (std::get<0>(ret) == "return")
            {
                for (int p = 0; p < 4; p++)
                {
                    if (thread_in_scope[p].joinable())
                        thread_in_scope[p].join();
                }
                return std::get<1>(ret);
            }
        }
        else if (instruction->value == "forloop")
        {
            std::tuple<std::string, w_variable *> ret = visitor_forloop(instruction, &variables_t, thread_id);
            if (std::get<0>(ret) == "return")
            {
                for (int p = 0; p < 4; p++)
                {
                    if (thread_in_scope[p].joinable())
                        thread_in_scope[p].join();
                }
                return std::get<1>(ret);
            }
        }
        else if (instruction->value == "whileloop")
        {
            std::tuple<std::string, w_variable *> ret = visitor_whileloop(instruction, &variables_t, thread_id);
            if (std::get<0>(ret) == "return")
            {

                for (int p = 0; p < 4; p++)
                {
                    if (thread_in_scope[p].joinable())
                        thread_in_scope[p].join();
                }
                return std::get<1>(ret);
            }
        }
        else if (instruction->value == "()")
        { // we just computes what is in parenthesis
            visitor_compute(instruction, &variables_t, thread_id);
        }
        else if (instruction->value[0] == '!')
        { // we call the function
            if (i + 1 < trunc->children.size() && trunc->children[i + 1]->value != "()")
            {
                std::string err = "l'appel d'une fonction doit etre suivie de ses arguments";
                error(err, instruction->reference, thread_id);
            }
            node *r = new node("*");
            r->push_child(instruction);
            r->push_child(trunc->children[i + 1]);
            visitor_compute(r, &variables_t, thread_id);
            i++; // we increment by one because of the parenthesis
        }
        // we need to do a function for keywords
        else if (instruction->value == "renvoie")
        { // The libere keyword
            w_variable *res = visitor_keyword_return(instruction, variables_t, thread_id);
            for (int p = 0; p < sizeof(thread_in_scope)/sizeof(thread_in_scope[0]); p++)
            {
                if (thread_in_scope[p].joinable())
                    thread_in_scope[p].join();
            }
            return res;
        }
        else if (instruction->value == "libere")
        { // The renvoie keyword
            visitor_keyword_free(instruction, &variables_t, thread_id);
        }
        else if (instruction->value == "inclue")
        { // The renvoie keyword
            visitor_keyword_include(instruction, &variables_t, thread_id);
        }
        else if (instruction->value == "tache")
        { // The tache keyword
            references.push_back(new std::stack<std::string>);
            thread_in_scope[thread_number++] = std::thread(visitor_keyword_tache, instruction, variables_t, thread_utilized);
            thread_utilized += 1;
        }
        else if (instruction->value != ";")
        {
            std::string err = "instruction inconnue '" + instruction->value + "'. \nAttention, l'instruction est peut-être inconnue dans le contexte (boucle, appel de fonction ect...). \nSi ça fait partie d'un calcule, il faut utiliser des parenthèses.";
            error(err, instruction->reference, thread_id);
        }
    }
    for (int p = 0; p < thread_number; p++)
    {
        if (thread_in_scope[p].joinable())
        {
            thread_in_scope[p].join();
        }
    }

    // On supprime les variables supplémentaires
    //variables_t.unuse();
    //variables_t.garbage_collector();
    
    return to_return;
}