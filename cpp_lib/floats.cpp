#include <vector>
#include <iostream>
#include <map>
#include <string>
#include "math.h"

#include <chrono>    
#include <ctime>

#include "../src/include/node.hpp"
#include "../src/include/variables.hpp"
#include "../src/include/visitor.hpp"

#include "./float_usefull.hpp"

extern "C" w_variable *float_times(std::vector<w_variable *> args, variable_table variables_t, std::string reference, int thread_id)
{   // 2 args : self, and X
    w_variable *self = args[0];
    w_variable *arg = args[1];
    if (arg->get_type() != "float" && arg->get_type() != "int")
    {
        std::string err = "la fonction !float.times n'accepte que un argument du type 'int' ou 'float'";
        error(err, reference, thread_id);
    }

    w_object *self_b = (w_object *)(self->content);
    
    float res = create_float(self_b);

    if (arg->get_type() == "float")
    {
        w_object *X = (w_object *)(arg->content);
        float other = create_float(X);
        res = res * other;
        int64_t tp = give_ten_power(res);
        w_variable *ret = new w_variable(res * pow(10, tp));
        w_variable *ntp = new w_variable(-tp);
        self_b->attribute_attribution("content", ret);
        self_b->attribute_attribution("ten_pow", ntp);
    }
    else 
    {
        int other = int(arg->convert_to_int());
        res = res * other;
        int64_t tp = give_ten_power(res);
        w_variable *ret = new w_variable(res * pow(10, tp));
        w_variable *ntp = new w_variable(-tp);
        self_b->attribute_attribution("content", ret);
        self_b->attribute_attribution("ten_pow", ntp);
    }

    return self;
}

extern "C" w_variable *float_div(std::vector<w_variable *> args, variable_table variables_t, std::string reference, int thread_id)
{   // 2 args : self, and X
        w_variable *self = args[0];
    w_variable *arg = args[1];
    if (arg->get_type() != "float" && arg->get_type() != "int")
    {
        std::string err = "la fonction !float.times n'accepte que un argument du type 'int' ou 'float'";
        error(err, reference, thread_id);
    }

    w_object *self_b = (w_object *)(self->content);
    
    float res = create_float(self_b);

    if (arg->get_type() == "float")
    {
        w_object *X = (w_object *)(arg->content);
        float other = create_float(X);
        res = res / other;
        int64_t tp = give_ten_power(res);
        w_variable *ret = new w_variable(res * pow(10, tp));
        w_variable *ntp = new w_variable(-tp);
        self_b->attribute_attribution("content", ret);
        self_b->attribute_attribution("ten_pow", ntp);
    }
    else 
    {
        int other = int(arg->convert_to_int());
        res = res / other;
        int64_t tp = give_ten_power(res);
        w_variable *ret = new w_variable(res * pow(10, tp));
        w_variable *ntp = new w_variable(-tp);
        self_b->attribute_attribution("content", ret);
        self_b->attribute_attribution("ten_pow", ntp);
    }

    return self;
}

extern "C" w_variable *float_plus(std::vector<w_variable *> args, variable_table variables_t, std::string reference, int thread_id)
{   // 2 args : self, and X
        w_variable *self = args[0];
    w_variable *arg = args[1];
    if (arg->get_type() != "float" && arg->get_type() != "int")
    {
        std::string err = "la fonction !float.times n'accepte que un argument du type 'int' ou 'float'";
        error(err, reference, thread_id);
    }

    w_object *self_b = (w_object *)(self->content);
    
    float res = create_float(self_b);

    if (arg->get_type() == "float")
    {
        w_object *X = (w_object *)(arg->content);
        float other = create_float(X);
        res = res + other;
        int64_t tp = give_ten_power(res);
        w_variable *ret = new w_variable(res * pow(10, tp));
        w_variable *ntp = new w_variable(-tp);
        self_b->attribute_attribution("content", ret);
        self_b->attribute_attribution("ten_pow", ntp);
    }
    else 
    {
        int other = int(arg->convert_to_int());
        res = res + other;
        int64_t tp = give_ten_power(res);
        w_variable *ret = new w_variable(res * pow(10, tp));
        w_variable *ntp = new w_variable(-tp);
        self_b->attribute_attribution("content", ret);
        self_b->attribute_attribution("ten_pow", ntp);
    }

    return self;
}

extern "C" w_variable *float_minus(std::vector<w_variable *> args, variable_table variables_t, std::string reference, int thread_id)
{   // 2 args : self, and X
    w_variable *self = args[0];
    w_variable *arg = args[1];
    if (arg->get_type() != "float" && arg->get_type() != "int")
    {
        std::string err = "la fonction !float.minus n'accepte que un argument du type 'int' ou 'float'";
        error(err, reference, thread_id);
    }

    w_object *self_b = (w_object *)(self->content);
    
    float res = create_float(self_b);

    if (arg->get_type() == "float")
    {
        w_object *X = (w_object *)(arg->content);
        float other = create_float(X);
        res = res - other;
        int64_t tp = give_ten_power(res);
        w_variable *ret = new w_variable(res * pow(10, tp));
        w_variable *ntp = new w_variable(-tp);
        self_b->attribute_attribution("content", ret);
        self_b->attribute_attribution("ten_pow", ntp);
    }
    else 
    {
        int other = int(arg->convert_to_int());
        res = res - other;
        int64_t tp = give_ten_power(res);
        w_variable *ret = new w_variable(res * pow(10, tp));
        w_variable *ntp = new w_variable(-tp);
        self_b->attribute_attribution("content", ret);
        self_b->attribute_attribution("ten_pow", ntp);
    }

    return self;
}

extern "C" w_variable *float_mod(std::vector<w_variable *> args, variable_table variables_t, std::string reference, int thread_id)
{   // 2 args : self, and X
    w_variable *self = args[0];
    w_variable *arg = args[1];
    if (arg->get_type() != "float" && arg->get_type() != "int")
    {
        std::string err = "la fonction !float.mod n'accepte que un argument du type 'int' ou 'float'";
        error(err, reference, thread_id);
    }

    
    w_object *self_b = (w_object *)(self->content);
    
    float res = create_float(self_b);

    if (arg->get_type() == "float")
    {
        w_object *X = (w_object *)(arg->content);
        float other = create_float(X);
        res = fmod(res, other);
        int64_t tp = give_ten_power(res);
        w_variable *ret = new w_variable(res * pow(10, tp));
        w_variable *ntp = new w_variable(-tp);
        self_b->attribute_attribution("content", ret);
        self_b->attribute_attribution("ten_pow", ntp);
    }
    else 
    {
        int other = int(arg->convert_to_int());
        res = fmod(res, other);
        int64_t tp = give_ten_power(res);
        w_variable *ret = new w_variable(res * pow(10, tp));
        w_variable *ntp = new w_variable(-tp);
        self_b->attribute_attribution("content", ret);
        self_b->attribute_attribution("ten_pow", ntp);
    }

    return self;
}

extern "C" w_variable *float_power(std::vector<w_variable *> args, variable_table variables_t, std::string reference, int thread_id)
{   // 2 args : self, and X
    w_variable *self = args[0];
    w_variable *arg = args[1];
    if (arg->get_type() != "float" && arg->get_type() != "int")
    {
        std::string err = "la fonction !float.power n'accepte que un argument du type 'int' ou 'float'";
        error(err, reference, thread_id);
    }

    
    w_object *self_b = (w_object *)(self->content);
    
    float res = create_float(self_b);

    if (arg->get_type() == "float")
    {
        w_object *X = (w_object *)(arg->content);
        float other = create_float(X);
        res = pow(res, other);
        int64_t tp = give_ten_power(res);
        w_variable *ret = new w_variable(res * pow(10, tp));
        w_variable *ntp = new w_variable(-tp);
        self_b->attribute_attribution("content", ret);
        self_b->attribute_attribution("ten_pow", ntp);
    }
    else 
    {
        int other = int(arg->convert_to_int());
        res = pow(res, other);
        int64_t tp = give_ten_power(res);
        w_variable *ret = new w_variable(res * pow(10, tp));
        w_variable *ntp = new w_variable(-tp);
        self_b->attribute_attribution("content", ret);
        self_b->attribute_attribution("ten_pow", ntp);
    }

    return self;
}

extern "C" w_variable *w_sqrt(std::vector<w_variable *> args, variable_table variables_t, std::string reference, int thread_id)
{   // 2 args : self, and X
    w_variable *fl = args[0];
    if (fl->get_type() != "float")
    {
        std::string err = "la fonction !sqrt n'accepte que un argument du type 'int' ou 'float'";
        error(err, reference, thread_id);
    }

    w_object *self_b = (w_object *)(fl->content);

    float res = create_float(self_b);

    if (res < 0) 
    {
        w_variable *ret = new w_variable(0);
        w_variable *ntp = new w_variable(1);
        self_b->attribute_attribution("content", ret);
        self_b->attribute_attribution("ten_pow", ntp);
        w_variable *self = new w_variable();
        self->type = T_OBJECT;
        self->content = (void *)self_b;
        return self;
    }

    res = sqrt(res); // Here is the computation
    int64_t tp = give_ten_power(res);
    w_variable *ret = new w_variable(res * pow(10, tp));
    w_variable *ntp = new w_variable(-tp);
    self_b->attribute_attribution("content", ret);
    self_b->attribute_attribution("ten_pow", ntp);

    return fl;
}