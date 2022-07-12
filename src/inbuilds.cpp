#include <curses.h>
#include <iostream>
#include <stdio.h>
#include "include/inbuilds.hpp"
#include "include/visitor.hpp"

// Inbuild functions for values char and int
// This are not objects so inbuilds functions to multiply, add ect... are not defined 
// in a class but here.

//

std::string s_delete_index(int index, std::string expr)
{
    std::string r;
    for (int  i = 0; i < expr.size(); i++)
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
    r->type = 1; // we set the type to char (1)
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

    r->type = 1; // we set the type to char (1)
    r->content = (void *)result; // we cast the old string to a void pointer

    return r;
}

w_variable *char_equals(std::string a, std::string b)
{ // returns a variable as a bool (int)
    w_variable *r = new w_variable();
    int *result = new int(a == b);
    r->type = 2; // we set the type to int (2)
    r->content = (void *)result; // we cast the value int to a void pointer
    return r;
}

w_variable *char_ne(std::string a, std::string b)
{ // returns a variable as a bool (int)
    w_variable *r = new w_variable();
    int *result = new int(a != b);
    r->type = 2; // we set the type to int (2)
    r->content = (void *)result; // we cast the value int to a void pointer
    return r;
}

w_variable *int_plus(int a, int b)
{ // returns a variable as an int

    w_variable *r = new w_variable();
    int *result = new int(a + b);
    r->type = 2; // we set the type to int (2)
    r->content = (void *)result; // we cast the value int to a void pointer
    return r;
}

w_variable *int_minus(int a, int b)
{ // returns a variable as an int
    w_variable *r = new w_variable();
    int *result = new int(a - b);
    r->type = 2; // we set the type to int (2)
    r->content = (void *)result; // we cast the value int to a void pointer
    return r;
}

w_variable *int_times(int a, int b)
{ // returns a variable as a int 
    w_variable *r = new w_variable();
    int *result = new int(a * b);
    r->type = 2; // we set the type to int (2)
    r->content = (void *)result; // we cast the value int to a void pointer
    return r;
}

w_variable *int_div(int a, int b)
{ // returns a variable as a int 
    w_variable *r = new w_variable();
    int *result = new int(a / b);
    r->type = 2; // we set the type to int (2)
    r->content = (void *)result; // we cast the value int to a void pointer
    return r;
}

w_variable *int_equals(int a, int b)
{ // returns a variable as a int 
    w_variable *r = new w_variable();
    int *result = new int(a == b);
    r->type = 2; // we set the type to int (2)
    r->content = (void *)result; // we cast the value int to a void pointer
    return r;
}

w_variable *int_lt(int a, int b)
{ // returns a variable as a int 
    w_variable *r = new w_variable();
    int *result = new int(a < b);
    r->type = 2; // we set the type to int (2)
    r->content = (void *)result; // we cast the value int to a void pointer
    return r;
}

w_variable *int_gt(int a, int b)
{ // returns a variable as a int 
    w_variable *r = new w_variable();
    int *result = new int(a > b);
    r->type = 2; // we set the type to int (2)
    r->content = (void *)result; // we cast the value int to a void pointer
    return r;
}

w_variable *int_ge(int a, int b)
{ // returns a variable as a int 
    w_variable *r = new w_variable();
    int *result = new int(a >= b);
    r->type = 2; // we set the type to int (2)
    r->content = (void *)result; // we cast the value int to a void pointer
    return r;
}

w_variable *int_le(int a, int b)
{ // returns a variable as a int 
    w_variable *r = new w_variable();
    int *result = new int(a <= b);
    r->type = 2; // we set the type to int (2)
    r->content = (void *)result; // we cast the value int to a void pointer
    return r;
}

w_variable *int_mod(int a, int b)
{ // returns a variable as a int 
    w_variable *r = new w_variable();
    int *result = new int(a % b);
    r->type = 2; // we set the type to int (2)
    r->content = (void *)result; // we cast the value int to a void pointer
    return r;
}

w_variable *int_power(int a, int b)
{ // returns a variable as a int 
    w_variable *r = new w_variable();
    int *result = new int(pow(a, b));
    r->type = 2; // we set the type to int (2)
    r->content = (void *)result; // we cast the value int to a void pointer
    return r;
}

w_variable *int_ne(int a, int b)
{ // returns a variable as a int 
    w_variable *r = new w_variable();
    int *result = new int(a != b);
    r->type = 2; // we set the type to int (2)
    r->content = (void *)result; // we cast the value int to a void pointer
    return r;
}

std::string convert_to_string(w_variable *content, std::map<std::string, w_variable *> variables_t)
{
    std::string type = content->get_type();
    if (type == "char")
    {
        return content->convert_to_char();
    }
    else if (type == "fonction")
    {
        return "<fonction "+ *(std::string *)(content->content) +">";
    }
    else if (type == "int")
    {
        return std::to_string(content->convert_to_int());
    }
    else
    { // it is therefore an object
        w_object *r = (w_object *)content->content;
        std::string name = "!" + r->name + ".en_string";
        variables_t["self"] = content;
        node *args = new node("*");
        return convert_to_string(visitor_funcall(name, args, variables_t), variables_t);
    }
}

void print(w_variable *content, std::map<std::string, w_variable *> variables_t)
{ // !print (content)
    // ==> 0
    std::string r = convert_to_string(content, variables_t);
    int i = 0;
    while (i < r.size())
    {
        char t = r[i];
        if (t == '\\')
        {
            if (r[i+1] == 'b')
            {
                putchar('\b');
                i++;
            }
            else 
                putchar(t);
        }
        else 
        {
            putchar(t);
        }
        i ++;
    }
}

w_variable *type(w_variable *content)
{
    std::string *type = new std::string(content->get_type());
    w_variable *res = new w_variable();
    res->type = 1; // It is a char
    res->content = (void *)type;
    return res;
}

w_variable *input(w_variable *content, std::map<std::string, w_variable *> variables_t)
{ // we use c input to get one caracter by one
    print(content, variables_t);
    char chr;
    chr = getchar();
    
    std::string *res = new std::string();
    
    if (chr == 127)
    {
        *res += "\\b";
        putchar('\b');
        putchar('\b');
    }
    else
        *res += chr;
    
    w_variable *var = new w_variable();
    var->type = 1; // Char
    var->content = (void *)res;
    return var;
}

w_variable *w_system(w_variable *content, std::map<std::string, w_variable *> variables_t)
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

w_variable *w_char(w_variable *content, std::map<std::string, w_variable *> variables_t)
{
    std::string type = content->get_type();
    if (type == "char")
    {
        return content;
    }
    else if (type == "fonction")
    {
        std::string *s = new std::string("<fonction "+ *(std::string *)(content->content) +">");
        w_variable *r = new w_variable();
        r->type = 1;
        r->content = (void *)s;
        return r;
    }
    else if (type == "int")
    {
        std::string *s = new std::string(std::to_string(content->convert_to_int()));
        w_variable *r = new w_variable();
        r->type = 1;
        r->content = (void *)s;
        return r;
    }
    else
    { // it is therefore an object
        w_object *r = (w_object *)content->content;
        std::string name = "!" + r->name + ".en_string";
        variables_t["self"] = content;
        node *args = new node("*");
        return w_char(visitor_funcall(name, args, variables_t), variables_t);
    }
}

w_variable *c_len(w_variable *content)
{
    int *s = new int(content->convert_to_char().size());
    w_variable *r = new w_variable();
    r->type = 2;
    r->content = (void *)s;
    return r;
}

void w_error(w_variable *content, std::map<std::string, w_variable *> variables_t)
{
    std::string r = convert_to_string(content, variables_t);
    error(r, references.top());
}

void w_exit(w_variable *content) 
{
    int n = content->convert_to_int();
    exit(n);
}

w_variable *w_en(std::vector<w_variable *>content, std::map<std::string, w_variable *> variables_t)
{
    std::string p = convert_to_string(content[0], variables_t);
    int i = content[1]->convert_to_int();

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
    time_t t = time(NULL); // get the time
    struct tm *tm = localtime(&t); // we get the time
    time_t u = tm->tm_gmtoff;
    int *r = new int(t); // we convert the time to seconds
    w_variable *result = new w_variable(); // we create a new variable
    result->type = 2; // int
    result->content = (void *)r; // we cast the value int to a void pointer
    return result; // we return the variable
}