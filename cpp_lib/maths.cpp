#include <math.h>
#include <vector>
#include <iostream>
#include <map>
#include <string>

#include "../src/include/node.hpp"
#include "../src/include/variables.hpp"
#include "../src/include/visitor.hpp"


extern "C" w_variable *wcos(std::vector<w_variable *> args, std::map<std::string, w_variable *> variables_t, std::string reference, int thread_id)
{
    if (args.size() != 1 || args[0]->get_type() != "int")
    {
        error("!cos : doit avoir un argument de type 'int'", reference, thread_id);
    }

    int e = *(int *)(args[0]->content);
    float a = cos(e) * 100;

    w_variable *r = new w_variable();
    r->type = 2; // int
    r->content = (void *)(new int(a));
    return r;
}

extern "C" w_variable *wsin(std::vector<w_variable *> args, std::map<std::string, w_variable *> variables_t, std::string reference, int thread_id)
{
    if (args.size() != 1 || args[0]->get_type() != "int")
    {
        error("!sin : doit avoir un argument de type 'int'", reference, thread_id);
    }

    int e = *(int *)(args[0]->content);
    float a = sinf(e) * 100; 

    w_variable *r = new w_variable();
    r->type = 2; // int
    r->content = (void *)(new int(a));
    return r;
}