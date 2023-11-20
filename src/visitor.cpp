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
#include "include/computations.hpp"

std::map<std::string, w_function *> functions;
std::map<std::string, std::string> functions_documentation;
std::map<std::string, w_class_template *> classes;

std::vector<std::string> inbuild_funcs;
std::vector<std::string> inbuild_funcs_documentation;

void *cpp_lib_handler = dlopen("/usr/local/lib/wati1.2/cpp_lib/libs.so", RTLD_LAZY);

bool is_initialized_inbuild = false;

int thread_utilized = 1;

std::vector<std::stack<std::string> *> references;
std::vector<std::string> included;

variable_table prepare_arguments(std::vector<w_variable *> vars, node *trunc, variable_table variables_t)
{ // Prépare les arguments dans la table de variable
    variable_table var_bis = variable_table();
    for (int i = 0; i < vars.size(); i++)
    {
        var_bis.assign(trunc->children[i]->children[0]->children[0]->value, vars[i], 0);
    }
    return var_bis;
}

std::stack<std::string> *what_reference(int thread_id)
{
    return references[thread_id];
}

std::string ref_pop(int thread_id)
{
    if (references[thread_id]->size() != 0)
    {
        std::string s = references[thread_id]->top();
        references[thread_id]->pop();
        return s;
    }
    return "";
}

bool parentethis_is_listed(node *trunc)
{ // Renvoie True si il y a des virgules (au moins une)
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
        warning("La classe w_erreur n'a pas été correctement importée. L'utilisation du wati en dépend. Erreur original : '" + err + "'", ref);
        std::cout.flush();
        throw "Bad import error";
    }

    std::vector<std::string> c_ref = cut_error_ref(ref);
    std::string filename = c_ref[0];

    std::string ref_error;

    int line;
    int col;
    int adder;
    if (c_ref[0] == main_filename)
        adder = 1;
    else
        adder = 0;
    try // To avoid 'libc++abi: terminating with uncaught exception of type std::invalid_argument: stoi: no conversion'
    {
        line = stoi(c_ref[1]) - adder;
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
        int s_adder;

        if (cut_error_ref(s_ref)[0] == main_filename)
            s_adder = 1;
        else
            s_adder = 0;

        try // To avoid error : 'libc++abi: terminating with uncaught exception of type std::invalid_argument: stoi: no conversion'
        {
            s_lne = stoi(cut_error_ref(s_ref)[1]) - s_adder;
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
    inbuild_funcs.push_back("!execute");
    inbuild_funcs_documentation.push_back("Execute l'argument comme du wati. !execute( arg(char) ) -> (?)");
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

    std::string fs = open_file("/usr/local/lib/wati1.2/cpp_lib/functions");
    if (fs == "file_not_found")
    { // very, very bad
        std::string err = "Interne : ne trouve pas '/usr/local/lib/wati1.2/cpp_lib/functions' pour les fonctions près-compilées";
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
    if (!patent.empty())
    {
        inbuild_funcs.push_back(patent);
        inbuild_funcs_documentation.push_back(documentation);
    }
}

std::vector<w_variable *> visitor_separate_listed_computed(node *parent, variable_table *variables_t, int thread_id)
{ // On prend les arguments, et on les transforme en une liste de variables qui sont après attribuables en arguemt
    std::vector<w_variable *> l;
    w_variable *comp = nullptr;
    for (auto n : parent->children)
    {
        if (n->value == ",")
        {
            l.push_back(comp);
            comp = nullptr;
        }
        else if (!(n->value == COMP_KEYWORD && n->children.size() == 0))
        {
            comp = visitor_compute(n, variables_t, thread_id);
        }
    }
    if (comp != nullptr)
        l.push_back(comp);
    return l;
}

node *visitor_separate_listed(node *parent)
{ // parent = ()
    node *total = new node("()");
    node *a = new node(COMP_KEYWORD);
    for (auto n : parent->children)
    {
        if (n->value == ",")
        {
            total->push_child(a);
            a = new node(COMP_KEYWORD);
        }
        else if (!(n->value == COMP_KEYWORD && n->children.size() == 0))
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

w_variable *visitor_function_inbuild(std::string name, std::vector<w_variable *> args, variable_table *variables_t, int thread_id)
{
    if (name == "!print")
    {
        for (auto arg_t : args)
        {
            print(arg_t, *variables_t);
        }
        w_variable *r = new w_variable(0);

        return r;
    }
    if (name == "!execute")
    {
        if (args.size() != 1)
        {
            std::string err = "la fonction !execute necessite un argument";
            error(err, ref_pop(thread_id), thread_id);
        }
        return execute(args[0], variables_t, ref_pop(thread_id), thread_id);
    }
    if (name == "!type")
    {
        if (args.size() != 1)
        {
            std::string err = "la fonction !type necessite un argument";
            error(err, ref_pop(thread_id), thread_id);
        }
        return type(args[0]);
    }
    if (name == "!input")
    {
        if (args.size() != 1)
        {
            std::string err = "la fonction !input necessite un argument";
            error(err, ref_pop(thread_id), thread_id);
        }
        return input(args[0], *variables_t);
    }
    if (name == "!system")
    {
        if (args.size() != 1)
        {
            std::string err = "la fonction !system necessite un argument";
            error(err, ref_pop(thread_id), thread_id);
        }
        return w_system(args[0], *variables_t);
    }
    if (name == "!char")
    {
        if (args.size() != 1)
        {
            std::string err = "la fonction !char necessite un argument";
            error(err, ref_pop(thread_id), thread_id);
        }

        return w_char(args[0], *variables_t, thread_id);
    }
    if (name == "!c_en")
    {
        if (args.size() != 2)
        {
            std::string err = "la fonction !c_en necessite deux argument";
            error(err, ref_pop(thread_id), thread_id);
        }
        if (args[0]->get_type() != "char")
        {
            std::string err = "le premier argument de !c_en doit etre de type char";
            error(err, ref_pop(thread_id), thread_id);
        }
        if (args[1]->get_type() != "int")
        {
            std::string err = "le deuxième argument de !c_en doit etre de type int";
            error(err, ref_pop(thread_id), thread_id);
        }
        return w_en(args, *variables_t);
    }
    if (name == "!c_len")
    {
        if (args.size() != 1)
        {
            std::string err = "la fonction !c_len necessite un argument";
            error(err, ref_pop(thread_id), thread_id);
        }
        if (args[0]->get_type() != "char")
        {
            std::string err = "l'argument de !c_len doit etre de type char";
            error(err, ref_pop(thread_id), thread_id);
        }
        return c_len(args[0]);
    }
    if (name == "!erreur")
    {
        if (args.size() != 1)
        {
            std::string err = "la fonction !erreur necessite un argument";
            error(err, ref_pop(thread_id), thread_id);
        }
        w_error(args[0], *variables_t, thread_id);
    }
    if (name == "!sortie")
    {
        if (args.size() != 1)
        {
            std::string err = "la fonction !sortie necessite un argument";
            error(err, ref_pop(thread_id), thread_id);
        }
        if (args[0]->get_type() != "int")
        {
            std::string err = "l'argument de !sortie doit etre de type int";
            error(err, ref_pop(thread_id), thread_id);
        }
        w_exit(args[0]);
    }

    // that means we are looking for a shared header
    std::string real_name = remove_prefix(name);
    w_variable *(*fonction)(std::vector<w_variable *>, variable_table, std::string, int);
    fonction = (w_variable * (*)(std::vector<w_variable *>, variable_table, std::string, int)) dlsym(cpp_lib_handler, real_name.c_str());

    variable_table local_var_table = variable_table();
    if (fonction == NULL)
    {
        std::string err = "ne peut pas charger la fonction '" + name + "', elle n'existe pas dans les fichiers compilés";
        error(err, what_reference(thread_id)->top(), thread_id);
    }

    return fonction(args, local_var_table, what_reference(thread_id)->top(), thread_id);
}

std::string remove_prefix(std::string name)
{
    std::string r = "";
    for (int i = 1; i < name.size(); i++)
    {
        r += name[i];
    }
    return r;
}

w_variable *visitor_prepare_variable_for_argument(w_variable *var, variable_table variables_t, int thread_id, std::string original_f_name)
{
    // renvoie le bon argument, à savoir, si la variable est appelée en arguement, et si oui, si elle se modifie
    std::string type = var->get_type();
    std::string f_name = "!" + type + ".argument";
    if (!function_exist(f_name, functions))
        return var;

    w_function *func = functions[f_name];
    variables_t.assign_no_copy("self", var, thread_id);

    w_variable *f = new w_variable(original_f_name);
    f->type = T_FONCTION;
    variables_t.assign_no_copy(func->get_arg_name(0), f, thread_id);

    w_variable *n = visitor_visit(func->trunc, variables_t, thread_id);
    if (n != var) // on vérifie que la variable a changé, et si non on essaie de supprimer l'inutile
        var->check_garbage();

    return n;
}

w_variable *visitor_prepare_variable_for_copy(w_variable *var, variable_table variables_t, int thread_id)
{
    std::string type = var->get_type();
    std::string f_name = "!" + type + ".copie";
    if (!function_exist(f_name, functions))
        return var;

    w_function *func = functions[f_name];
    variables_t.assign_no_copy("self", var, thread_id);

    w_variable *n = visitor_visit(func->trunc, variables_t, thread_id);
    if (n != var)
        var->check_garbage();
    return n;
}

w_variable *visitor_funcall(std::string name, node *args, variable_table variables_t, variable_table *variables_t_parent, int thread_id)
{
    // La table de variable parente sert à calculer les arguements, pour entre autre
    // que les pointeurs soient référencés au bon endroit
    w_function *func;
    std::string var_name = remove_prefix(name);
    std::map<std::string, w_function *> funcs = functions;
    if (!function_exist(std::string(name), funcs))
    {
        std::string err = "la fonction '" + name + "' n'existe pas";
        error(err, what_reference(thread_id)->top(), thread_id);
    }

    func = functions[name];
    std::vector<w_variable *> args_c = visitor_separate_listed_computed(args, variables_t_parent, thread_id);

    if (visitor_is_inbuild(name))
    {
        return visitor_function_inbuild(name, args_c, variables_t_parent, thread_id);
    }
    else
    {
        if (func->arguments->children.size() != args_c.size())
        {
            std::string err = "la fonction " + name + " requiers " + std::to_string(func->arguments->children.size()) + " argument(s), " + std::to_string(args_c.size()) + " argument(s) as(ont) été indiqué(s)";
            error(err, ref_pop(thread_id), thread_id);
        }
        for (int i = 0; i < args_c.size(); i++)
        {
            std::string arg_n = func->get_arg_name(i);
            args_c[i] = visitor_prepare_variable_for_argument(args_c[i], variables_t, thread_id, name);
            variables_t.assign_no_copy(arg_n, args_c[i], thread_id);
        }
        w_variable *res = visitor_visit(func->trunc, variables_t, thread_id);

        if (res != nullptr && res->use != 0)
            res->use--;

        for (auto i : args_c)
        {
            // i->use --; //[JSP]
            i->check_garbage();
        }

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
    std::vector<w_variable *> args_c = visitor_separate_listed_computed(args, variables_t_parent, thread_id);
    if (visitor_is_inbuild(name))
    {
        return visitor_function_inbuild(name, args_c, variables_t_parent, thread_id);
    }
    else
    {
        for (int i = 0; i < args_c.size(); i++)
        {
            std::string arg_n = func->get_arg_name(i);
            args_c[i] = visitor_prepare_variable_for_argument(args_c[i], variables_t, thread_id, name);
            variables_t.assign_no_copy(arg_n, args_c[i], thread_id);
        }
        variables_t.assign("self", self, thread_id);
        w_variable *res = visitor_visit(func->trunc, variables_t, thread_id);

        if (res != nullptr)
            res->use--;

        for (auto i : args_c)
        {
            // i->use --; // [JSP]
            i->check_garbage();
        }
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
    /*if (opera == "et")
    {
        return int_et(a, b);
    }
    if (opera == "ou")
    {
        return int_ou(a, b);
    }*/

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
        else if (opera == "==")
        {
            w_variable *r = new w_variable(0);
            return r;
        }
        else
            error("Ne peut pas utiliser l'opérateur '" + opera + "' avec les types '" + a->get_type() + "' et '" + b->get_type() + "'", ref_pop(thread_id), thread_id);
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
    w_function *func;
    std::string f_name;
    if (opera == "+")
    {
        f_name = "!" + a->get_type() + ".plus";
        if (!function_exist(f_name, functions))
        {
            std::string err = "il n'existe pas de fonction " + f_name;
            error(err, ref_pop(thread_id), thread_id);
        }
        func = functions[f_name];
    }
    else if (opera == "-")
    {
        f_name = "!" + a->get_type() + ".minus";
        if (!function_exist(f_name, functions))
        {
            std::string err = "il n'existe pas de fonction " + f_name;
            error(err, ref_pop(thread_id), thread_id);
        }
        func = functions[f_name];
    }
    else if (opera == "==")
    {
        f_name = "!" + a->get_type() + ".equals";
        if (!function_exist(f_name, functions))
        {
            std::string err = "il n'existe pas de fonction " + f_name;
            error(err, ref_pop(thread_id), thread_id);
        }
        func = functions[f_name];
    }
    else if (opera == "<")
    {
        f_name = "!" + a->get_type() + ".lt";
        if (!function_exist(f_name, functions))
        {
            std::string err = "il n'existe pas de fonction " + f_name;
            error(err, ref_pop(thread_id), thread_id);
        }
        func = functions[f_name];
    }
    else if (opera == ">")
    {
        f_name = "!" + a->get_type() + ".gt";
        if (!function_exist(f_name, functions))
        {
            std::string err = "il n'existe pas de fonction " + f_name;
            error(err, ref_pop(thread_id), thread_id);
        }
        func = functions[f_name];
    }
    else if (opera == "<=")
    {
        f_name = "!" + a->get_type() + ".le";
        if (!function_exist(f_name, functions))
        {
            std::string err = "il n'existe pas de fonction " + f_name;
            error(err, ref_pop(thread_id), thread_id);
        }
        func = functions[f_name];
    }
    else if (opera == ">=")
    {
        f_name = "!" + a->get_type() + ".ge";
        if (!function_exist(f_name, functions))
        {
            std::string err = "il n'existe pas de fonction " + f_name;
            error(err, ref_pop(thread_id), thread_id);
        }
        func = functions[f_name];
    }
    else if (opera == "%")
    {
        f_name = "!" + a->get_type() + ".mod";
        if (!function_exist(f_name, functions))
        {
            std::string err = "il n'existe pas de fonction " + f_name;
            error(err, ref_pop(thread_id), thread_id);
        }
        func = functions[f_name];
    }
    else if (opera == "/")
    {
        f_name = "!" + a->get_type() + ".div";
        if (!function_exist(f_name, functions))
        {
            std::string err = "il n'existe pas de fonction " + f_name;
            error(err, ref_pop(thread_id), thread_id);
        }
        func = functions[f_name];
    }
    else if (opera == "^")
    {
        f_name = "!" + a->get_type() + ".power";
        if (!function_exist(f_name, functions))
        {
            std::string err = "il n'existe pas de fonction " + f_name;
            error(err, ref_pop(thread_id), thread_id);
        }
        func = functions[f_name];
    }
    else if (opera == "!=")
    {
        f_name = "!" + a->get_type() + ".ne";
        if (!function_exist(f_name, functions))
        {
            std::string err = "il n'existe pas de fonction " + f_name;
            error(err, ref_pop(thread_id), thread_id);
        }
        func = functions[f_name];
    }
    else if (opera == "*")
    {
        f_name = "!" + a->get_type() + ".times";
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
    w_variable *r;
    {
        variable_table v = variable_table();
        a = visitor_prepare_variable_for_argument(a, v, thread_id, f_name);
        v.assign("self", a, thread_id);
        b = visitor_prepare_variable_for_argument(b, v, thread_id, f_name);
        v.assign(func->arguments->children[0]->children[0]->children[0]->value, b, thread_id);
        r = visitor_visit(func->trunc, v, thread_id);
    }
    r->use--;
    return r;
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
    { // attibuts de base
        visitor_vardef(attr, r->attributes, thread_id);
    }
    r->attributes->assign("iterable", new w_variable(0), thread_id);
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
    r->type = T_FONCTION;                         // this is a function
    r->content = (void *)(new std::string(name)); // the content is just the name of the variable
    return r;
}

w_variable *visitor_keyword_return(node *trunc, variable_table variables_t, int thread_id)
{
    if (trunc->children.size() < 1)
    {
        std::string err = "le mot-clé 'renvoie' doit avoir au moins un argument";
        error(err, trunc->reference, thread_id);
    }
    node *arg = trunc->children[0];
    w_variable *result = visitor_compute(arg, &variables_t, thread_id);

    result->use++;
    return result;
}

void visitor_keyword_free(node *trunc, variable_table *variables_t, int thread_id)
{
    if (trunc->children.size() < 1)
    {
        std::string err = "le mot-clé 'libere' doit avoir au moins un argument";
        error(err, trunc->reference, thread_id);
    }
    node *arg = trunc->children[0]->children[0];

    std::string varName;
    w_variable *self;
    variable_table *t = variables_t;

    w_variable *b_value = get_name_for_vardef(arg, variables_t, varName, self, thread_id); // vas être l'index si on a une indexation

    if (varName[0] == '!') // Dans ce cas, c'est une assignation dans un itérable "!type.modifie"
    {
        varName = "!" + self->get_type() + ".pop"; // On modifie legerement la fonction car on ne veux pas modifier
                                                   // la liste mais bien supprimer un élément
        if (!function_exist(varName, functions))
            error("La fonction '" + varName + "' n'existe pas pour la destruction de la variable de variable", trunc->reference, thread_id);

        w_function *f = functions[varName]; // arguments : (index, valeur)

        // On récupère le nom de l'argument de la fonction !type.pop(arg)
        std::string n1 = f->arguments->children[0]->children[0]->children[0]->value;
        w_variable *a_detruire;

        {
            variable_table bis = variable_table();
            bis.assign(n1, b_value, thread_id);
            bis.assign("self", self, thread_id);
            a_detruire = visitor_visit(f->trunc, bis, thread_id);
        }
        // Maintenant que on a récupéré la variable est qu'elle est sensée ne plus être dans la liste
        // on doit détruire la valeur
        a_detruire->use--;
        bool check = a_detruire->check_garbage();
        // if (!check)
        //     error("La variable qui est entrain d'être détruite n'a pas été correctement enlevé des tables de variables (usage = " + std::to_string(a_detruire->use) + ")", trunc->reference, thread_id);
    }
    else
    {
        if (!t->exist(varName))
            error("La variable '" + varName + "' n'existe pas", trunc->reference, thread_id);

        auto var = t->get(varName);

        if (var->is_object())
        {
            w_variable *r;
            if (function_exist("!" + var->get_type() + ".destructeur", functions))
            {
                w_function *f = functions["!" + var->get_type() + ".destructeur"];
                variable_table local_table = variable_table();
                local_table.assign_no_copy("self", var, thread_id);
                r = visitor_visit(f->trunc, local_table, thread_id);
                // r->use --;
            }
        }
        t->delete_var(varName);
    }
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
        node *arg = new node(COMP_KEYWORD);
        arg->push_child((new node("0"))->set_prevalue(new w_variable(0)));
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
            node *arg_bis = new node(COMP_KEYWORD);
            arg_bis->push_child(((new node(std::to_string(i))))->set_prevalue(new w_variable(i)));
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
        w_variable *potentiel_destru = visitor_visit(func->trunc, variables_bis, thread_id);
        potentiel_destru->use--;
        potentiel_destru->check_garbage(); // [JSP]
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
    what_reference(thread_id)->push(trunc->reference);
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
            filename2 = "/usr/local/lib/wati1.2/lib/" + filename;
            if (visitor_is_included(filename2))
            { // meaning we already included it
                ref_pop(thread_id);
                return;
            }
            r = open_file(filename2.c_str());
        }
        if (r == "file_not_found")
        { // it clearly doesn't exists
            std::string err = "fichier introuvable '" + filename2 + "'";
            warning(err, trunc->children[0]->reference);
            error(err, trunc->children[0]->reference, thread_id);
        }

        included.push_back(filename2);
        std::vector<std::string> ref;
        std::vector<std::string> lexemes = lexer(r, ref, filename2);

        node *ast = parser(lexemes, filename2, ref, filename2 + "1:1");
        if (show_tree)
        {
            std::cout << ast->to_string(0) << std::endl;
        }
        visitor_visit_incode(ast, variables_t, thread_id);
    }
    base_dir = dir;
    what_reference(thread_id)->pop();
}

void visitor_funcdef(node *trunc)
{
    what_reference(0)->push(trunc->reference);
    w_function *func = new w_function();
    std::string name = "!" + trunc->children[0]->value;
    // Qu'il y ai des arguments ou non, le parser aura mis un COMP_KEYWORD dans la parenthèse pour éviter le plantage
    func->set_arguments(visitor_separate_listed(trunc->children[1]));
    if (is_char(trunc->children[2]->children[0]->children[0]->value))
    { // it is the description of the function, we can store it and remove it from the trunc
        // we remove the '"'
        std::string description = del_string(trunc->children[2]->children[0]->children[0]->value);
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
    std::string var_name;
    w_variable *self = nullptr;

    variable_table *v = variables_t;
    w_variable *b_value = get_name_for_vardef(trunc->children[0], v, var_name, self, thread_id); // vas être l'index si on a une indexation

    node *compute = trunc->children[1];
    bool tracker_assignation = false;
    w_variable *result = visitor_compute(compute, variables_t, thread_id);
    check_value_integrity(result, trunc->reference, thread_id); // Nécessaire pour vérifier que la valeur est 'valide'
    result = visitor_prepare_variable_for_copy(result, variable_table(), thread_id);
    if (var_name[0] == '!') // Dans ce cas, c'est une assignation dans un itérable "!type.modifie"
    {
        if (!function_exist(var_name, functions))
            error("La fonction '" + var_name + "' n'existe pas pour cette définition de variable", trunc->reference, thread_id);

        w_function *f = functions[var_name]; // arguments : (index, valeur)

        // On récupère le nom des arguments de la fonction
        std::string n1 = f->arguments->children[0]->children[0]->children[0]->value;
        std::string n2 = f->arguments->children[1]->children[0]->children[0]->value;

        variable_table bis = variable_table();
        bis.assign(n1, b_value, thread_id);
        bis.assign(n2, result, thread_id);
        bis.assign("self", self, thread_id);
        w_variable *potentiel_destru = visitor_visit(f->trunc, bis, thread_id);
        if (potentiel_destru != nullptr)
        {
            potentiel_destru->use--;
            potentiel_destru->check_garbage(); // [JSP]}
        }
    }
    else
        v->assign(var_name, result, thread_id);

    // return result;
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
    node *condition = trunc->children[0]->children[0];
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
            w_variable *cond = visitor_compute(elif->children[0]->children[0], variables_t, thread_id);
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
    //      COMP_KEYWORD
    //          ...
    //      {}
    //          ...
    std::string var_name = trunc->children[0]->value;
    node *code = trunc->children[2];
    w_variable *iter = std::get<1>(visitor_visit_incode(trunc->children[1], variables_t, thread_id));

    if (!function_exist("!initialise_boucle", functions))
        error("La fonction !initialise_boucle n'existe pas. La bibliothèque standard n'a pas été correctement importée", trunc->reference, thread_id);

    w_function *init_boucle = functions["!initialise_boucle"];
    variable_table v = variable_table();
    v.assign_no_copy(init_boucle->arguments->children[0]->children[0]->children[0]->value, iter, thread_id); // on ne veux pas que l'itérable soit copié
    visitor_visit(init_boucle->trunc, v, thread_id);

    w_function *suivant = functions["!" + iter->get_type() + ".suivant"];
    w_function *a_la_fin = functions["!" + iter->get_type() + ".a_la_fin"];
    w_function *debut = functions["!" + iter->get_type() + ".debut"];
    v.assign_no_copy("self", iter, thread_id);

    w_variable *var;

    while(!is_int_true(visitor_visit(a_la_fin->trunc, v, thread_id)))
    {
        var = visitor_visit(suivant->trunc, v, thread_id);
        variables_t->assign(var_name, var, thread_id);
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
        {
            continue;
        }
    }
    if (!function_exist("!termine_boucle", functions))
        error("La fonction !termine_boucle n'existe pas. La bibliothèque standard n'a pas été correctement importée", trunc->reference, thread_id);
    w_function *term_boucle = functions["!termine_boucle"];
    visitor_visit(term_boucle->trunc, v, thread_id);
    iter->check_garbage();
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

    w_variable *cond = std::get<1>(visitor_visit_incode(condition, variables_t, thread_id));
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
        cond->check_garbage(); // On a surement plus besoin de cette variable à ce stade
        cond = std::get<1>(visitor_visit_incode(condition, variables_t, thread_id));
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
        what_reference(thread_id)->push(instruction->reference);
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
                ref_pop(thread_id);
                return ret;
            }
            if (std::get<0>(ret) == "continue")
            {
                ref_pop(thread_id);
                return ret;
            }
            if (std::get<0>(ret) == "break")
            {
                ref_pop(thread_id);
                return ret;
            }
        }
        else if (instruction->value == "forloop")
        {
            std::tuple<std::string, w_variable *> ret = visitor_forloop(instruction, variables_t, thread_id);
            if (std::get<0>(ret) == "return")
            {
                ref_pop(thread_id);
                return ret;
            }
        }
        else if (instruction->value == "whileloop")
        {
            std::tuple<std::string, w_variable *> ret = visitor_whileloop(instruction, variables_t, thread_id);
            if (std::get<0>(ret) == "return")
            {
                ref_pop(thread_id);
                return ret;
            }
        }
        else if (instruction->value == "renvoie")
        { // The renvoie keyword
            w_variable *res = visitor_keyword_return(instruction, *variables_t, thread_id);
            // res->use ++; [JSP]
            ref_pop(thread_id);
            return std::tuple<std::string, w_variable *>{"return", res};
        }
        else if (instruction->value == "libere")
        { // The libere keyword
            visitor_keyword_free(instruction, variables_t, thread_id);
        }
        else if (instruction->value == "casse")
        {
            ref_pop(thread_id);
            return std::tuple<std::string, w_variable *>{"break", nullptr};
        }
        else if (instruction->value == "continue") // just continue the loop for the next iteration
        {
            ref_pop(thread_id);
            return std::tuple<std::string, w_variable *>{"continue", nullptr};
        }
        else if (instruction->value == "inclue")
        { // The renvoie keyword
            visitor_keyword_include(instruction, variables_t, thread_id);
        }
        else if (instruction->value == "libere")
        { // The renvoie keyword
            visitor_keyword_free(instruction, variables_t, thread_id);
        }
        else
        {
            to_return = std::tuple<std::string, w_variable *>{"", visitor_compute(instruction, variables_t, thread_id)};
        }
        ref_pop(thread_id);
    }
    return to_return;
}

w_variable *visitor_visit(node *trunc, variable_table variables_t, int thread_id)
{
    int thread_number = 0;
    std::thread thread_in_scope[100];
    if (!is_initialized_inbuild)
    {
        visitor_init_inbuild_functions();
    }
    w_variable *to_return = nullptr;

    for (int i = 0; i < trunc->children.size(); i++)
    {
        node *instruction = trunc->children[i];
        what_reference(thread_id)->push(instruction->reference);
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
                ref_pop(thread_id);
                std::get<1>(ret)->use++;
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
                ref_pop(thread_id);
                std::get<1>(ret)->use++;
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
                ref_pop(thread_id);
                std::get<1>(ret)->use++;
                return std::get<1>(ret);
            }
        }
        // we need to do a function for keywords
        else if (instruction->value == "renvoie")
        {                                                                                  // The libere keyword
            w_variable *res = visitor_keyword_return(instruction, variables_t, thread_id); // Ici : res->use ++;
            for (int p = 0; p < sizeof(thread_in_scope) / sizeof(thread_in_scope[0]); p++)
            {
                if (thread_in_scope[p].joinable())
                    thread_in_scope[p].join();
            }
            ref_pop(thread_id);
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
            thread_in_scope[thread_number] = std::thread(visitor_keyword_tache, instruction, variables_t, thread_utilized);
            thread_utilized += 1;
            thread_number++;
        }
        else
        {
            to_return = visitor_compute(instruction, &variables_t, thread_id);
            if (to_return != nullptr)
                to_return->use ++;
        }
        ref_pop(thread_id);
    }

    for (int p = 0; p < thread_number; p++)
    {
        if (thread_in_scope[p].joinable())
        {
            thread_in_scope[p].join();
        }
        else
            thread_in_scope[p].detach();
    }
    return to_return;
}