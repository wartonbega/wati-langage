#include <iostream>
#include <stack>

#include "include/visitor.hpp"
#include "include/variables.hpp"
#include "include/types.hpp"
#include "include/inbuilds.hpp"
#include "include/main.hpp"
#include "include/lexer.hpp"
#include "include/parser.hpp"

std::map<std::string, w_function *> functions;
std::map<std::string, w_class_template *> classes;
bool is_initialized_inbuild = false;
std::stack<std::string> references;

std::vector<std::string> included;

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

void error(std::string err, std::string ref)
{
    std::cout << "\n";
    std::string err_s = TERMINAL_BOLDRED + ref + TERMINAL_BOLDBLACK + " erreur : " + TERMINAL_BOLDRED + err;
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

    while (!references.empty())
    {
        std::string s_ref = references.top();
        std::string s_fln = cut_error_ref(s_ref)[0];
        int s_lne;
        int s_col;
        try // To avoid 'libc++abi: terminating with uncaught exception of type std::invalid_argument: stoi: no conversion'
        {
            s_lne = stoi(cut_error_ref(s_ref)[1]);
            s_col = stoi(cut_error_ref(s_ref)[2]);
        }
        catch (std::invalid_argument err)
        {
        }

        references.pop();
        std::cout << TERMINAL_BOLDCYAN + "\nréférencé depuis :\n"
                  << give_file_error(s_fln, s_lne, s_col, s_ref);
    }
    exit(1);
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
    if (name == "!print" || name == "!type" || name == "!system" || name == "!input" || name == "!char" || name == "!c_len" || name == "!erreur" || name == "!sortie" || name == "!c_en" || name == "!temps")
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
        r->content = (void *)(new int(0));

        return r;
    }
    if (name == "!type")
    {
        if (args->children.size() != 1)
        {
            std::string err = "la fonction !type necessite un argument";
            error(err, references.top());
        }
        w_variable *arg = visitor_compute(args->children[0], variables_t);
        return type(arg);
    }
    if (name == "!input")
    {
        if (args->children.size() != 1)
        {
            std::string err = "la fonction !input necessite un argument";
            error(err, references.top());
        }
        w_variable *arg = visitor_compute(args->children[0], variables_t);
        return input(arg, variables_t);
    }
    if (name == "!system")
    {
        if (args->children.size() != 1)
        {
            std::string err = "la fonction !system necessite un argument";
            error(err, references.top());
        }
        w_variable *arg = visitor_compute(args->children[0], variables_t);
        return w_system(arg, variables_t);
    }
    if (name == "!char")
    {
        if (args->children.size() != 1)
        {
            std::string err = "la fonction !char necessite un argument";
            error(err, references.top());
        }
        w_variable *arg = visitor_compute(args->children[0], variables_t);
        return w_char(arg, variables_t);
    }
    if (name == "!c_en")
    {
        if (args->children.size() != 2)
        {
            std::string err = "la fonction !c_en necessite un argument";
            error(err, references.top());
        }
        std::vector<w_variable *> arg_c;
        for (auto arg_t : args->children)
        {
            arg_c.push_back(visitor_compute(arg_t, variables_t));
        }
        if (arg_c[0]->get_type() != "char")
        {
            std::string err = "le premier argument de !c_en doit etre de type char";
            error(err, references.top());
        }

        if (arg_c[1]->get_type() != "int")
        {
            std::string err = "le deuxième argument de !c_en doit etre de type int";
            error(err, references.top());
        }
        return w_en(arg_c, variables_t);
    }
    if (name == "!c_len")
    {
        if (args->children.size() != 1)
        {
            std::string err = "la fonction !c_len necessite un argument";
            error(err, references.top());
        }
        w_variable *arg = visitor_compute(args->children[0], variables_t);
        if (arg->get_type() != "char")
        {
            std::string err = "l'argument de !c_len doit etre de type char";
            error(err, references.top());
        }
        return c_len(arg);
    }
    if (name == "!erreur")
    {
        if (args->children.size() != 1)
        {
            std::string err = "la fonction !erreur necessite un argument";
            error(err, references.top());
        }
        w_variable *arg = visitor_compute(args->children[0], variables_t);
        w_error(arg, variables_t);
    }
    if (name == "!sortie")
    {
        if (args->children.size() != 1)
        {
            std::string err = "la fonction !sortie necessite un argument";
            error(err, references.top());
        }
        w_variable *arg = visitor_compute(args->children[0], variables_t);
        if (arg->get_type() != "int")
        {
            std::string err = "l'argument de !sortie doit etre de type int";
            error(err, references.top());
        }
        w_exit(arg);
    }
    if (name == "!temps")
    {
        // No args
        if (args->children.size() != 0)
        {
            std::string err = "la fonction !temps n'as pas d'argument";
            error(err, references.top());
        }
        return w_time();
    }
    return nullptr;
}

std::string remove_function_call_prefix(std::string name)
{
    std::string r = "";
    for (int i = 1; i < name.size(); i ++)
    {
        r += name[i];
    }
    return r;
}

w_variable *visitor_funcall(std::string name, node *args, std::map<std::string, w_variable *> variables_t)
{
    w_function *func;
    std::string var_name = remove_function_call_prefix(name);
    if (variable_exist(var_name, variables_t))
    {
        w_variable *func_var = variables_t[var_name];
        if (func_var->get_type() != "fonction")
        {
            std::string err = "le type '" + func_var->get_type() + "' ne peut pas être appelé";
            error(err, references.top());
        }

        name = *(std::string *)(func_var->content);
        if (!function_exist(name, functions))
        {
            std::string err = "la fonction " + name + " n'existe pas";
            error(err, references.top());
        }
    }
    func = functions[name];
    args = visitor_separate_listed(args);
    if (visitor_is_inbuild(name))
    {
        return visitor_function_inbuild(name, args, variables_t);
    }
    else
    {
        if (func->arguments->children.size() != args->children.size())
        {
            std::string err = "la fonction " + name + " requiers " + std::to_string(func->arguments->children.size()) + " argument(s), " + std::to_string(args->children.size()) + " argument(s) as(ont) été indiqué(s)";
            error(err, references.top());
        }
        for (int i = 0; i < args->children.size(); i++)
        {
            std::string arg_n = func->arguments->children[i]->children[0]->value;
            w_variable *arg_v = visitor_compute(args->children[i], variables_t);
            variables_t[arg_n] = arg_v;
        }
        w_variable *res = visitor_visit(func->trunc, variables_t);
        return res;
    }
}

w_variable *visitor_funcall_methode(std::string name, node *args, std::map<std::string, w_variable *> variables_t, w_variable *self)
{

    if (!function_exist(name, functions))
    {
        std::string err = "la fonction " + name + " n'existe pas";
        error(err, references.top());
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
        variables_t["self"] = self;
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
    error(err, references.top());
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
    if (opera == "-")
    {
        return char_minus(a, b);
    }
    std::string err = "operateur inconnu : '" + opera + "'";
    error(err, references.top());
    // Throw an error, unknown operator
    return nullptr;
}

w_variable *visitor_use_inbuild(w_variable *a, w_variable *b, std::string opera)
{
    // We check if both value have the same type
    if (a->get_type() != b->get_type())
    {
        if (opera == "!=")
        {
            w_variable *r = new w_variable();
            int *p = new int(1);
            r->type = 2; // int
            r->content = (void *)p;
            return r;   
        }
        else
        {
            w_variable *r = new w_variable();
            int *p = new int(0);
            r->type = 2; // int
            r->content = (void *)p;
            return r;
        }
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
    {
        std::string f_name = "!" + a->get_type() + ".plus";
        if (!function_exist(f_name, functions))
        {
            std::string err = "il n'existe pas de fonction " + f_name;
            error(err, references.top());
        }
        func = functions[f_name];
    }
    else if (opera == "-")
    {
        std::string f_name = "!" + a->get_type() + ".minus";
        if (!function_exist(f_name, functions))
        {
            std::string err = "il n'existe pas de fonction " + f_name;
            error(err, references.top());
        }
        func = functions[f_name];
    }
    else if (opera == "==")
    {
        std::string f_name = "!" + a->get_type() + ".equals";
        if (!function_exist(f_name, functions))
        {
            std::string err = "il n'existe pas de fonction " + f_name;
            error(err, references.top());
        }
        func = functions[f_name];
    }
    else if (opera == "<")
    {
        std::string f_name = "!" + a->get_type() + ".lt";
        if (!function_exist(f_name, functions))
        {
            std::string err = "il n'existe pas de fonction " + f_name;
            error(err, references.top());
        }
        func = functions[f_name];
    }
    else if (opera == ">")
    {
        std::string f_name = "!" + a->get_type() + ".gt";
        if (!function_exist(f_name, functions))
        {
            std::string err = "il n'existe pas de fonction " + f_name;
            error(err, references.top());
        }
        func = functions[f_name];
    }
    else if (opera == "<=")
    {
        std::string f_name = "!" + a->get_type() + ".le";
        if (!function_exist(f_name, functions))
        {
            std::string err = "il n'existe pas de fonction " + f_name;
            error(err, references.top());
        }
        func = functions[f_name];
    }
    else if (opera == ">=")
    {
        std::string f_name = "!" + a->get_type() + ".ge";
        if (!function_exist(f_name, functions))
        {
            std::string err = "il n'existe pas de fonction " + f_name;
            error(err, references.top());
        }
        func = functions[f_name];
    }
    else if (opera == "%")
    {
        std::string f_name = "!" + a->get_type() + ".mod";
        if (!function_exist(f_name, functions))
        {
            std::string err = "il n'existe pas de fonction " + f_name;
            error(err, references.top());
        }
        func = functions[f_name];
    }
    else if (opera == "/")
    {
        std::string f_name = "!" + a->get_type() + ".div";
        if (!function_exist(f_name, functions))
        {
            std::string err = "il n'existe pas de fonction " + f_name;
            error(err, references.top());
        }
        func = functions[f_name];
    }
    else if (opera == "^")
    {
        std::string f_name = "!" + a->get_type() + ".power";
        if (!function_exist(f_name, functions))
        {
            std::string err = "il n'existe pas de fonction " + f_name;
            error(err, references.top());
        }
        func = functions[f_name];
    }
    else if (opera == "!=")
    {
        std::string f_name = "!" + a->get_type() + ".ne";
        if (!function_exist(f_name, functions))
        {
            std::string err = "il n'existe pas de fonction " + f_name;
            error(err, references.top());
        }
        func = functions[f_name];
    }
    else if (opera == "*")
    {
        std::string f_name = "!" + a->get_type() + ".times";
        if (!function_exist(f_name, functions))
        {
            std::string err = "il n'existe pas de fonction " + f_name;
            error(err, references.top());
        }
        func = functions[f_name];
    }

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
        std::string err = "la classe '" + name + "' n'existe pas";
        error(err, references.top());
    }
    w_class_template *temp = classes[name];
    w_object *r = new w_object();
    for (auto attr : temp->trunc->children[1]->children)
    { // attibutes
        std::string name = attr->children[0]->value;
        node *comp = attr->children[1];
        w_variable *computed = visitor_compute(comp, variables_t);
        r->attribute_attribution(name, computed);
    }
    for (auto meth : temp->trunc->children[2]->children)
    { // methods
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

w_variable *generate_function_variable(std::string name)
{
    w_variable *r = new w_variable();
    r->type = 0; // this is a function
    r->content = (void *)(new std::string(name)); // the content is just the name of the variable
    return r;
}

w_variable *visitor_compute(node *c, std::map<std::string, w_variable *> variables_t)
{
    w_variable *last_value = new w_variable();
    last_value->type = 2;                       // By default, the type is int
    last_value->content = (void *)(new int(0)); // and the value is 0
    for (int i = 0; i < c->children.size(); i++)
    {
        references.push(c->children[i]->reference);
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
                    error(err, c->children[i]->reference);
                }
                w_variable *first_var = variables_t[first_var_n];

                if (!first_var->is_object())
                {
                    std::string err = "la variable '" + first_var_n + "' doit être un objet";
                    error(err, c->children[i]->reference);
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
                                error(err, c->children[i]->reference);
                            }

                            last_var = last_o->get_attribute(patent);
                            if (!last_var->is_object())
                            {
                                std::string err = "la variable doit être un objet, pas du type " + last_var->get_type();
                                error(err, c->children[i]->reference);
                            }
                            last_o = (w_object *)last_var->content;
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
                    last_value = generate_function_variable(f_name);
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
                    std::map<std::string, w_variable *> variables_t_bis = std::map<std::string, w_variable *>(variables_t);
                    last_value = visitor_funcall_methode(f_name, c->children[i + 1], variables_t_bis, last_var);
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
                    error(err, c->children[i]->reference);
                }
                w_variable *first_var = variables_t[first_var_n];
                if (!first_var->is_object())
                {
                    std::string err = "la variable '" + first_var_n + "' doit être un objet";
                    error(err, c->children[i]->reference);
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
                                error(err, c->children[i]->reference);
                            }
                            w_variable *last_var = last_o->get_attribute(patent);
                            if (!last_var->is_object())
                            {
                                std::string err = "la variable '" + last_o->name + "." + patent + " doit être un objet";
                                error(err, c->children[i]->reference);
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
                        error(err, c->children[i]->reference);
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
            if (i + 1 >= c->children.size() || c->children[i + 1]->value != "()") // does not call
            {
                last_value = generate_function_variable(c->children[i]->value);
                // std::string err = "l'appel d'une fonction doit etre suivie de ses arguments";
                // error(err, c->children[i]->reference);
            }
            else
            {
                last_value = visitor_funcall(expr, c->children[i + 1], variables_t);
                i++; // we need to increment by one, because of the parenthesis
            }
        }
        else if (variable_exist(expr, variables_t)) // variable_exist(expr, variables_t))
        {
            // we just take the value inside the variables table
            last_value = variables_t[expr];
        }
        else if (i + 1 < c->children.size() && c->children[i + 1]->value == "()") // create a new object
        {                                    // just avoid segfaults ...
            // call a new object
            last_value = visitor_new_object(expr, c->children[i + 1], variables_t);
            i++;
        }
        else if (expr == "()")
        {
            if (parentethis_is_listed(c->children[i]))
            { // then we try to create a new list classe
                if (!class_exist("list", classes))
                {
                    std::string err = "la classe 'list' n'existe pas, on ne peux donc pas initialiser de nouvelles listes à partir de parenthèses.\nPeut être avez-vous oublié d'inclue la librairie standard (std.wati)";
                    error(err, c->children[i]->reference);
                }

                w_variable *r = visitor_new_object("list", new node("()"), variables_t);
                std::map<std::string, w_variable*> variables_t_bis = std::map<std::string, w_variable*>(variables_t);
                variables_t_bis["self"] = r;
                
                std::string funcname = "!list.plus"; // the function to add elements to a list

                node *elements = visitor_separate_listed(c->children[i]); // it will be used as arguments
                
                for (auto parts : elements->children)
                {   
                    visitor_funcall(funcname, parts, variables_t_bis); 
                }
                last_value = r; // we finally push the list
            }
            else
            { // just compute what is in the parentethis
                node *parenthis = c->children[i];
                last_value = visitor_compute(parenthis, variables_t);
            }
        }
        else if (expr == "[]")
        {
            node *brack = c->children[i];
            w_variable *b_value = visitor_compute(brack, variables_t);

            std::string name = "!" + last_value->get_type() + ".index";

            if (!function_exist(name, functions))
            {
                std::string err = "la fonction '" + name + "' n'existe pas";
                error(err, c->children[i]->reference);
            }
            w_function *f = functions[name];

            std::map<std::string, w_variable *> variables_t_bis = std::map<std::string, w_variable *>(variables_t);
            variables_t_bis["self"] = last_value;
            variables_t_bis[f->arguments->children[0]->children[0]->value] = b_value;
            last_value = visitor_visit(f->trunc, variables_t_bis);
        }
        else
        {
            std::string err = "expression ou variable inconnue '" + expr + "'";
            error(err, c->children[i]->reference);
        }
        references.pop();
    }
    return last_value;
}

w_variable *visitor_keyword_return(node *trunc, std::map<std::string, w_variable *> variables_t)
{
    references.push(trunc->reference);
    if (trunc->children.size() < 1)
    {
        std::string err = "le mot-clé 'renvoie' doit avoir au moins un argument";
        error(err, trunc->reference);
    }
    node *arg = trunc->children[0];
    w_variable *result = visitor_compute(arg, variables_t);
    references.pop();
    return result;
}

void visitor_keyword_free(node *trunc, std::map<std::string, w_variable *> variables_t)
{
    references.push(trunc->reference);
    if (trunc->children.size() < 1)
    {
        std::string err = "le mot-clé 'libere' doit avoir au moins un argument";
        error(err, trunc->reference);
    }
    node *arg = trunc->children[0];
    w_variable *result = visitor_compute(arg, variables_t);
    if (!result->is_object())
    {
        std::string err = "ne peux pas libérer un non-object";
        error(err, trunc->reference);
    }
    for (auto i : variables_t)
    {
        std::string key = std::get<0>(i);
        if (std::get<1>(i) == result)
        {
            variables_t.erase(key);
            break;
        }
    }
    w_object *t = (w_object *)result->content;

    std::string name = "!" + t->name + ".destructeur"; // we call the desctructor
    if (function_exist(name, functions))
        visitor_funcall_methode(name, new node("*"), variables_t, result);

    delete result;
    references.pop();
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

void visitor_keyword_include(node *trunc, std::map<std::string, w_variable *> variables_t)
{
    references.push(trunc->reference);
    if (trunc->children.size() < 1)
    {
        std::string err = "le mot-clé 'inclue' doit avoir au moins un argument";
        error(err, trunc->reference);
    }
    node *arg = trunc->children[0];
    w_variable *result = visitor_compute(arg, variables_t);
    if (result->get_type() != "char")
    {
        std::string err = "le mot-clé 'inclue' doit avoir un argument de type 'char'";
        error(err, trunc->children[0]->reference);
    }

    std::string filename = result->convert_to_char();
    std::string dir = base_dir;

    std::string filename2 = base_dir + filename;
    base_dir += separate_base_dir(filename);

    if (!visitor_is_included(filename2))
    {
        included.push_back(filename2);
        std::string r = open_file(filename2.c_str());
        if (r == "file_not_found")
        {
            std::string err = "fichier introuvable '" + filename2 + "'";
            error(err, trunc->children[0]->reference);
        }
        std::vector<std::string> ref;
        std::vector<std::string> lexemes = lexer(r, ref, filename2);

        node *ast = parser(lexemes, filename2, ref, filename2 + "1:1");
        visitor_visit(ast, variables_t);
    }
    base_dir = dir;

    references.pop();
}

void visitor_funcdef(node *trunc)
{
    references.push(trunc->reference);
    w_function *func = new w_function();
    std::string name = "!" + trunc->children[0]->value;
    func->set_arguments(visitor_separate_listed(trunc->children[1]));
    func->set_content(trunc->children[2]);
    functions[name] = func;
    references.pop();
}

void visitor_vardef(node *trunc, std::map<std::string, w_variable *> &variables_t)
{
    references.push(trunc->reference);
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
            error(err, trunc->children[0]->reference);
        }
        w_variable *first_var = variables_t[first_var_n];

        if (!first_var->is_object())
        {
            std::string err = "la variable " + first_var_n + " doit être un objet";
            error(err, trunc->children[0]->reference);
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
                        error(err, trunc->children[0]->reference);
                    }
                    last_var = last_o->get_attribute(patent);
                    if (!last_var->is_object())
                    {
                        std::string err = "la variable " + last_o->name + "." + patent + " doit être un objet";
                        error(err, trunc->children[0]->reference);
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
                error(err, trunc->children[0]->reference);
            }
            last_o = (w_object *)last_var->content;
            last_o->attribute_attribution(patent, result);
        }
    }
    else
    {
        variables_t[expr] = result;
    }
    references.pop();
}

void visitor_classdef(node *trunc)
{
    references.push(trunc->reference);
    w_class_template *c = new w_class_template();
    c->trunc = trunc;
    classes[trunc->children[0]->value] = c;
    for (auto f : trunc->children[2]->children)
    {
        // the name of the classe + "." + the name of the function
        f->children[0]->value = trunc->children[0]->value + "." + f->children[0]->value;
        visitor_funcdef(f); // we define the methode as a function
    }
    references.pop();
}

std::tuple<std::string, w_variable *> visitor_if_declaration(node *trunc, std::map<std::string, w_variable *> &variables_t)
{
    references.push(trunc->reference);
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
        std::tuple<std::string, w_variable *> ret = visitor_visit_incode(trunc->children[1], variables_t);
        if (std::get<0>(ret) == "return")
        {
            references.pop();
            return ret;
        }
        if (std::get<0>(ret) == "continue")
        {
            references.pop();
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
            w_variable *cond = visitor_compute(elif->children[0], variables_t);
            if (is_int_true(cond))
            {
                e = true;
                std::tuple<std::string, w_variable *> ret = visitor_visit_incode(elif->children[1], variables_t);
                if (std::get<0>(ret) == "return")
                {
                    references.pop();
                    return ret;
                }
                if (std::get<0>(ret) == "continue")
                {
                    references.pop();
                    return ret;
                }
                break;
            }
        }
        if (!e) // meaning, there were no elif used
        { // we call the else
            node *else_ = trunc->children[3];
            if (else_->children.size() != 0)
            {
                else_ = else_->children[0];
                std::tuple<std::string, w_variable *> ret = visitor_visit_incode(else_->children[0], variables_t);
                if (std::get<0>(ret) == "return")
                {
                    references.pop();
                    return ret;
                }
                if (std::get<0>(ret) == "continue")
                {
                    references.pop();
                    return ret;
                }
            }
        }
    }
    references.pop();
    return std::tuple<std::string, w_variable *>{"", nullptr};
}

std::tuple<std::string, w_variable *> visitor_forloop(node *trunc, std::map<std::string, w_variable *> &variables_t)
{
    references.push(trunc->reference);
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
        error(err, conde->children[0]->reference);
    }
    int borne1 = first_b->convert_to_int();

    conde->children.clear();
    conde->push_child(trunc->children[1]->children[1]);
    w_variable *second_b = visitor_compute(conde, variables_t);
    if (second_b->get_type() != "int")
    {
        std::string err = "les bornes de la boucle doivent être de type int";
        error(err, conde->children[0]->reference);
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
        std::tuple<std::string, w_variable *> ret = visitor_visit_incode(code, variables_t);
        if (std::get<0>(ret) == "break")
        {
            references.pop();
            return std::tuple<std::string, w_variable *>{"", nullptr};
        };
        if (std::get<0>(ret) == "return")
        {
            references.pop();
            return ret;
        }
        if (std::get<0>(ret) == "continue")
        {
            continue;
        }
    }
    references.pop();
    return std::tuple<std::string, w_variable *>{"", nullptr};
}

std::tuple<std::string, w_variable *> visitor_whileloop(node *trunc, std::map<std::string, w_variable *> &variables_t)
{
    references.push(trunc->reference);
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
        std::tuple<std::string, w_variable *> ret = visitor_visit_incode(code, variables_t);
        if (std::get<0>(ret) == "break")
        {
            references.pop();
            return std::tuple<std::string, w_variable *>{"", nullptr};
        }
        if (std::get<0>(ret) == "return")
        {
            references.pop();
            return ret;
        }
        if (std::get<0>(ret) == "continue")
            continue;
        cond = visitor_compute(condition, variables_t);
    }
    references.pop();
    return std::tuple<std::string, w_variable *>{"", nullptr};
}

std::tuple<std::string, w_variable *> visitor_visit_incode(node *trunc, std::map<std::string, w_variable *> &variables_t)
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
            visitor_vardef(instruction, variables_t);
        }
        else if (instruction->value == "classdef")
        {
            visitor_classdef(instruction);
        }
        else if (instruction->value == "ifdec")
        {
            std::tuple<std::string, w_variable *> ret = visitor_if_declaration(instruction, variables_t);
            if (std::get<0>(ret) == "return")
            {
                return ret;
            }
            if (std::get<0>(ret) == "continue")
            {
                return ret;
            }
        }
        else if (instruction->value == "forloop")
        {
            std::tuple<std::string, w_variable *> ret = visitor_forloop(instruction, variables_t);
            if (std::get<0>(ret) == "return")
            {
                return ret;
            }
        }
        else if (instruction->value == "whileloop")
        {
            std::tuple<std::string, w_variable *> ret = visitor_whileloop(instruction, variables_t);
            if (std::get<0>(ret) == "return")
            {
                return ret;
            }
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
                error(err, instruction->reference);
            }
            node *r = new node("*");
            r->push_child(instruction);
            r->push_child(trunc->children[i + 1]);
            visitor_compute(r, variables_t);
            i++; // we increment by one because of the parenthesis
        }
        else if (instruction->value == "renvoie")
        { // The renvoie keyword
            w_variable *res = visitor_keyword_return(instruction, variables_t);
            return std::tuple<std::string, w_variable *>{"return", res};
        }
        else if (instruction->value == "libere")
        { // The libere keyword
            visitor_keyword_free(instruction, variables_t);
        }
        else if (instruction->value == "casse")
        {
            return std::tuple<std::string, w_variable *>{"break", nullptr};
        }
        else if (instruction->value == "continue") // just continue the loop for the next iteration
        {
            return std::tuple<std::string, w_variable *>{"continue", nullptr};
        }
        else if (instruction->value != ";")
        {
            std::string err = "instruction inconnue '" + instruction->value + "'. \nAttention, l'instruction est peut-être inconnue dans le contexte (boucle, appel de fonction ect...). \nSi ça fait partie d'un calcule, il faut utiliser des parenthèses.";
            error(err, instruction->reference);
        }
    }
    return to_return;
}

w_variable *visitor_visit(node *trunc, std::map<std::string, w_variable *> variables_t)
{
    if (!is_initialized_inbuild)
    {
        visitor_init_inbuild_functions();
    }
    w_variable *to_return = new w_variable();
    to_return->type = 2;
    to_return->content = (void *)(new int(0)); // by default, the result is an int ( 0 )

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
            std::tuple<std::string, w_variable *> ret = visitor_if_declaration(instruction, variables_t);
            if (std::get<0>(ret) == "return")
            {
                return std::get<1>(ret);
                references.pop();
            }
        }
        else if (instruction->value == "forloop")
        {
            std::tuple<std::string, w_variable *> ret = visitor_forloop(instruction, variables_t);
            if (std::get<0>(ret) == "return")
            {
                return std::get<1>(ret);
                references.pop();
            }
        }
        else if (instruction->value == "whileloop")
        {
            std::tuple<std::string, w_variable *> ret = visitor_whileloop(instruction, variables_t);
            if (std::get<0>(ret) == "return")
            {
                return std::get<1>(ret);
                references.pop();
            }
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
                error(err, instruction->reference);
            }
            node *r = new node("*");
            r->push_child(instruction);
            r->push_child(trunc->children[i + 1]);
            visitor_compute(r, variables_t);
            i++; // we increment by one because of the parenthesis
        }
        // we need to do a function for keywords
        else if (instruction->value == "renvoie")
        { // The libere keyword
            w_variable *res = visitor_keyword_return(instruction, variables_t);
            return res;
        }
        else if (instruction->value == "libere")
        { // The renvoie keyword
            visitor_keyword_free(instruction, variables_t);
        }
        else if (instruction->value == "inclue")
        { // The renvoie keyword
            visitor_keyword_include(instruction, variables_t);
        }
        else if (instruction->value != ";")
        {
            std::string err = "instruction inconnue '" + instruction->value + "'. \nAttention, l'instruction est peut-être inconnue dans le contexte (boucle, appel de fonction ect...). \nSi ça fait partie d'un calcule, il faut utiliser des parenthèses.";
            error(err, instruction->reference);
        }
    }
    return to_return;
}