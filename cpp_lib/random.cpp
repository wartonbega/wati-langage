#include <vector>
#include <iostream>
#include <map>
#include <string>
#include <cstdlib>
#include <time.h>

#include "../src/include/node.hpp"
#include "../src/include/variables.hpp"
#include "../src/include/visitor.hpp"
#include "../src/include/main.hpp"
#include "../src/include/parser.hpp"
#include "../src/include/inbuilds.hpp"

extern "C" w_variable *init_random(std::vector<w_variable *> args, variable_table variables_t, std::string reference, int thread_id)
{
    srand(time(0));
    return nullptr;
}

extern "C" w_variable *w_random(std::vector<w_variable *> args, variable_table variables_t, std::string reference, int thread_id)
{
    // Renvoie un nombre random entre lb et ur
    if (args.size() != 2 || args[0]->get_type() != "int" || args[1]->get_type() != "int")
        error("!random : necessite deux arguments de type int", reference, thread_id);
    
    int64_t lb = *(int64_t *)args[0]->content;
    int64_t ub = *(int64_t *)args[1]->content;

    int64_t r = int64_t((rand() % (ub - lb + 1)) + lb);
    
    return new w_variable(r);
}