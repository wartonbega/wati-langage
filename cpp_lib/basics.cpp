#include <vector>
#include <iostream>
#include <map>
#include <string>

#include "../src/include/node.hpp"
#include "../src/include/variables.hpp"
#include "../src/include/visitor.hpp"


extern "C" w_variable *et(std::vector<w_variable *> args, std::map<std::string, w_variable *> variables_t, std::string reference, int thread_id)
{
    if (args.size() != 2 || args[0]->get_type() != "int" || args[1]->get_type() != "int")
    {
        error("!et : doit avoir deux arguments de type 'int'", reference, thread_id);
    }


    int a = *(int *)(args[0]->content);
    int b = *(int *)(args[1]->content);

    w_variable *r = new w_variable();
    r->type = 2; // int
    r->content = (void *)(new int(a && b));
    return r;
}

extern "C" w_variable* ou(std::vector<w_variable *> args, std::map<std::string, w_variable *> variables_t, std::string reference, int thread_id)
{
    if (args.size() != 2 || args[0]->get_type() != "int" || args[1]->get_type() != "int")
    {
        error("!ou : doit avoir deux arguments de type 'int'", reference, thread_id);
    }


    int a = *(int *)(args[0]->content);
    int b = *(int *)(args[1]->content);

    w_variable *r = new w_variable();
    r->type = 2; // int
    
    r->content = (void *)(new int(a || b));
    return r;
}

extern "C" w_variable* pas(std::vector<w_variable *> args, std::map<std::string, w_variable *> variables_t, std::string reference, int thread_id)
{
    if (args.size() != 1 || args[0]->get_type() != "int")
    {
        error("!pas : doit avoir un argument de type 'int'", reference, thread_id);
    }

    int a = *(int *)(args[0]->content);

    w_variable *r = new w_variable();
    r->type = 2; // int
    r->content = (void *)(new int(!a));
    return r;
}
