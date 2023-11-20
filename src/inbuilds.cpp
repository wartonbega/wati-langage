
#include <iostream>
#include <stdio.h>
#include "include/parser.hpp"
#include "include/node.hpp"
#include "include/lexer.hpp"
#include "include/inbuilds.hpp"
#include "include/visitor.hpp"
#include "include/trackers.hpp"

// Inbuild functions for values char and int
// This are not objects so inbuilds functions to multiply, add ect... are not defined
// in a class but here.

// definitions of the arrow keys
#define KEY_UP 72
#define KEY_DOWN 80
#define KEY_LEFT 75
#define KEY_RIGHT 77

std::string s_delete_index(int index, std::string expr)
{
    std::string r;
    for (int i = 0; i < expr.size(); i++)
    {
        if (index != i)
            r += expr[i];
    }
    return r;
}

w_variable *char_plus(std::string a, std::string b)
{ // returns a w_variable as a char
    w_variable *r = new w_variable();
    std::string *result = new std::string(a + b);
    r->type = 1;                 // we set the type to char (1)
    r->content = (void *)result; // we cast the old string to a void pointer
    // we are sure to get the good result, as we set the type to 1 (char)
    return r;
}

w_variable *char_minus(std::string a, std::string b)
{
    w_variable *r = new w_variable();
    int index = -1;
    for (int i = 0; i < a.size(); i++)
    {
        std::string c;
        c += a[i];
        if (c == b)
            index = i;
    }
    if (index != -1)
        a = s_delete_index(index, a);

    std::string *result = new std::string(a);

    r->type = 1;                 // we set the type to char (1)
    r->content = (void *)result; // we cast the old string to a void pointer

    return r;
}

w_variable *char_equals(std::string a, std::string b)
{ // returns a variable as a bool (int)
    w_variable *r = new w_variable(a == b);
    return r;
}

w_variable *char_ne(std::string a, std::string b)
{ // returns a variable as a bool (int)
    w_variable *r = new w_variable(a != b);
    return r;
}

w_variable *int_plus(int64_t a, int64_t b)
{ // returns a variable as an int

    w_variable *r = new w_variable(a + b);
    return r;
}

w_variable *int_minus(int64_t a, int64_t b)
{ // returns a variable as an int
    w_variable *r = new w_variable(a - b);
    return r;
}

w_variable *int_times(int64_t a, int64_t b)
{ // returns a variable as a int
    w_variable *r = new w_variable(a * b);
    return r;
}

w_variable *int_div(int64_t a, int64_t b)
{ // returns a variable as a int
    w_variable *r = new w_variable(a / b);
    return r;
}

w_variable *int_equals(int64_t a, int64_t b)
{ // returns a variable as a int
    w_variable *r = new w_variable(a == b);
    return r;
}

w_variable *int_lt(int64_t a, int64_t b)
{ // returns a variable as a int
    w_variable *r = new w_variable(a < b);
    return r;
}

w_variable *int_gt(int64_t a, int64_t b)
{ // returns a variable as a int
    w_variable *r = new w_variable(a > b);
    return r;
}

w_variable *int_ge(int64_t a, int64_t b)
{ // returns a variable as a int
    w_variable *r = new w_variable(a >= b);
    return r;
}

w_variable *int_le(int64_t a, int64_t b)
{ // returns a variable as a int
    w_variable *r = new w_variable(a <= b);
    return r;
}

w_variable *int_mod(int64_t a, int64_t b)
{ // returns a variable as a int
    w_variable *r = new w_variable(a % b);
    return r;
}

w_variable *int_power(int64_t a, int64_t b)
{ // returns a variable as a int
    w_variable *r = new w_variable(pow(a, b));
    return r;
}

w_variable *int_ne(int64_t a, int64_t b)
{ // returns a variable as a int
    w_variable *r = new w_variable(a != b);
    return r;
}

w_variable *int_ou(int64_t a, int64_t b)
{ // returns a variable as a int
    w_variable *r = new w_variable(a and b);
    return r;
}

w_variable *int_et(int64_t a, int64_t b)
{ // returns a variable as a int
    w_variable *r = new w_variable(a or b);
    return r;
}

std::string convert_to_string(w_variable *content, variable_table variables_t, int thread_id)
{
    std::string type = content->get_type();
    if (type == "char")
    {
        return content->convert_to_char();
    }
    else if (type == "fonction")
    {
        return "<fonction " + *(std::string *)(content->content) + ">";
    }
    else if (type == "int")
    {
        return std::to_string(content->convert_to_int());
    }
    else if (type == "traqueur")
    {
        tracker *t = (tracker *)(content->content);
        std::string s = t->en_string(variables_t, thread_id);
        return s;
    }
    else
    { // it is therefore an object
        w_object *r = (w_object *)content->content;
        std::string name = "!" + r->name + ".en_string";
        node *args = new node("*");
        variables_t.assign("self", content, thread_id);
        return convert_to_string(visitor_funcall(name, args, variables_t, &variables_t, thread_id), variables_t, thread_id);
    }
}

std::string beautify_for_print(w_variable *content, variable_table variables_t)
{
    std::string full = "";
    std::string r = convert_to_string(content, variables_t, 0);
    int i = 0;
    while (i < r.size())
    {
        char t = r[i];
        if (t == '\\')
        {
            if (r[i + 1] == 'b')
            {
                full += '\b';
                i++;
            }
            if (r[i + 1] == 't')
            {
                full += "\t";
                i++;
            }
            else
                full += t;
        }
        else
        {
            full += t;
        }
        i++;
    }
    return full;
}

void print(w_variable *content, variable_table variables_t)
{ // !print (content)
    // ==> 0
    std::string to_print = beautify_for_print(content, variables_t);
    std::cout << to_print;
    std::cout << std::flush;
}

w_variable *type(w_variable *content)
{
    std::string type = content->get_type();
    w_variable *res = new w_variable(type);
    return res;
}

w_variable *input(w_variable *content, variable_table variables_t)
{ // we use c input to get one caracter by one
    print(content, variables_t);
    char chr;
    chr = getchar();

    std::string *res = new std::string();
    if (chr == 127)
    {
        *res += "\b";
        putchar('\b');
        putchar('\b');
    }
    else if (chr == 10)
        *res += "\n";
    else if (chr == 27)
        *res += "";
    else if (chr == 28)
        *res += "";
    else if (chr == KEY_LEFT)
        *res += "";
    else if (chr == KEY_RIGHT)
        *res += "";
    else
        *res += chr;

    w_variable *var = new w_variable();
    var->type = 1; // Char
    var->content = (void *)res;
    return var;
}

w_variable *w_system(w_variable *content, variable_table variables_t)
{
    if (content->get_type() != "char")
    {
        // [TODO]: Errors
    }
    std::string to_exec = *(std::string *)content->content;
    int *r = new int(system(to_exec.c_str()));
    w_variable *result = new w_variable();
    result->type = 2; // int
    result->content = (void *)r;
    return result;
}

w_variable *w_char(w_variable *content, variable_table variables_t, int thread_id)
{
    std::string type = content->get_type();
    if (type == "char")
    {
        return content;
    }
    else if (type == "fonction")
    {
        std::string *s = new std::string("<fonction " + *(std::string *)(content->content) + ">");
        w_variable *r = new w_variable();
        r->type = T_CHAR;
        r->content = (void *)s;
        return r;
    }
    else if (type == "int")
    {
        std::string *s = new std::string(std::to_string(content->convert_to_int()));
        w_variable *r = new w_variable();
        r->type = T_CHAR;
        r->content = (void *)s;
        return r;
    }
    else if (type == "traqueur")
    {
        std::string *s = new std::string(convert_to_string(content, variables_t, thread_id));
        w_variable *r = new w_variable();
        r->type = T_CHAR;
        r->content = (void *)s;
        return r;
    }
    else
    { // it is therefore an object
        std::string *s = new std::string(convert_to_string(content, variables_t, thread_id));
        w_variable *r = new w_variable();
        r->type = T_CHAR;
        r->content = (void *)s;
        return r;
    }
}

w_variable *c_len(w_variable *content)
{
    int64_t *s = new int64_t(content->convert_to_char().size());
    w_variable *r = new w_variable();
    r->type = T_INT;
    r->content = (void *)s;
    return r;
}

void w_error(w_variable *content, variable_table variables_t, int thread_id)
{
    std::string r = convert_to_string(content, variables_t, thread_id);
    error(r, what_reference(0)->top(), thread_id);
}

void w_exit(w_variable *content)
{
    int n = content->convert_to_int();
    exit(n);
}

w_variable *w_en(std::vector<w_variable *> content, variable_table variables_t)
{
    std::string p = convert_to_string(content[0], variables_t, 0);
    int64_t i = content[1]->convert_to_int();

    std::string u;
    u += p[i];
    std::string *s = new std::string(u);
    w_variable *r = new w_variable();
    r->type = 1;
    r->content = (void *)s;
    return r;
}

w_variable *w_time()
{
    // return the time
    time_t t = time(NULL);         // get the time
    struct tm *tm = localtime(&t); // we get the time
    time_t u = tm->tm_gmtoff;
    int64_t *r = new int64_t(t);           // we convert the time to seconds
    w_variable *result = new w_variable(); // we create a new variable
    result->type = 2;                      // int
    result->content = (void *)r;           // we cast the value int to a void pointer
    return result;                         // we return the variable
}

w_variable *execute(w_variable *arg, variable_table *variables_t, std::string reference, int thread_id)
{
    // execute some wati-code
    if (arg->get_type() != "char")
    {
        error("!execute : doit avoir un argument de type 'char'", reference, thread_id);
    }
    std::string r = arg->convert_to_char(); // the code itself

    std::string filename = cut_error_ref(reference)[0];
    std::vector<std::string> ref;
    std::vector<std::string> lexemes = lexer(r, ref, filename);

    node *ast = parser(lexemes, "main", ref, filename + "1:1");

    // on renvoie ce que renvoie le programme
    std::pair<std::string, w_variable *> to_ret = visitor_visit_incode(ast, variables_t, 0);
    if (to_ret.second != nullptr)
        return to_ret.second;
    return new w_variable("none");
}