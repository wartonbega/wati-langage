#include <curses.h>
#include <iostream>
#include <stdio.h>
#include "include/inbuilds.hpp"
#include "include/visitor.hpp"

// Inbuild functions for values char and int
// This are not objects so inbuilds functions to multiply, add ect... are not defined 
// in a class but here.

//

w_variable *char_plus(std::string a, std::string b)
{ // returns a w_variable as a char
    w_variable *r = new w_variable();
    std::string *result = new std::string(a + b);
    r->type = 1; // we set the type to char (1)
    r->content = (void *)result; // we cast the old string to a void pointer
    // we are sure to get the good result, as we set the type to 1 (char)
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
    else if (type == "int")
    {
        return std::to_string(content->convert_to_int());
    }
    else
    { // it is therefor an object
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
    std::cout << convert_to_string(content, variables_t);
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
    scanf("%c",&chr);
    std::string *res = new std::string();
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