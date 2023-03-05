#include <math.h>
#include <vector>
#include <iostream>
#include <map>
#include <string>

#include "../src/include/node.hpp"
#include "../src/include/variables.hpp"
#include "../src/include/visitor.hpp"

#include "./float_usefull.hpp"

extern "C" w_variable *wcos(std::vector<w_variable *> args, variable_table variables_t, std::string reference, int thread_id)
{
    if (args.size() != 1 || args[0]->get_type() != "float")
    {
        error("!wcos : doit avoir un argument de type 'float'", reference, thread_id);
    }
    w_variable *arg = args[0];

    w_object *X = (w_object *)(arg->content);
    float e = create_float(X);
    
    w_object *self_b = new w_object(*(w_object *)(arg->content));

    float res = cos(e);

    int64_t tp = give_ten_power(res);
    w_variable *ret = new w_variable(res * pow(10, tp));
    w_variable *ntp = new w_variable(-tp);
    self_b->attribute_attribution("content", ret);
    self_b->attribute_attribution("ten_pow", ntp);


    w_variable *r = new w_variable();
    r->type = T_OBJECT; // object
    r->content = self_b;
    return visitor_new_object("float", ((new node("()"))->push_child((new node("*"))->set_prevalue(r))), variables_t, thread_id);
}

extern "C" w_variable *wsin(std::vector<w_variable *> args, variable_table variables_t, std::string reference, int thread_id)
{
    if (args.size() != 1 || args[0]->get_type() != "float")
    {
        error("!wsin : doit avoir un argument de type 'float'", reference, thread_id);
    }
    w_variable *arg = args[0];

    w_object *X = (w_object *)(arg->content);
    float e = create_float(X);
    
    w_object *self_b = new w_object(*(w_object *)(arg->content));

    float res = sin(e);

    int64_t tp = give_ten_power(res);
    w_variable *ret = new w_variable(res * pow(10, tp));
    w_variable *ntp = new w_variable(-tp);
    self_b->attribute_attribution("content", ret);
    self_b->attribute_attribution("ten_pow", ntp);


    w_variable *r = new w_variable();
    r->type = T_OBJECT; // object
    r->content = self_b;
    return visitor_new_object("float", ((new node("()"))->push_child((new node("*"))->set_prevalue(r))), variables_t, thread_id);
}