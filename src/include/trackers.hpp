
#include "node.hpp"
#include "variables.hpp"

#include <map>
#include <vector>
#include <string>

#ifndef TRACKERS_H  
#define TRACKERS_H


struct tracker 
{
    variable_table *variable_t;
    std::string name_to_track;

    tracker(std::string, variable_table *);
    w_variable *value(int);
};


#endif