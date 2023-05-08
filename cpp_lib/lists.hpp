#include <vector>
#include <string>

#include "../src/include/node.hpp"
#include "../src/include/variables.hpp"
#include "../src/include/visitor.hpp"


#ifndef LIST_HPP
#define LIST_HPP

w_variable *create_list(std::vector<int64_t>, variable_table, int);
w_variable *create_list(std::vector<std::string>, variable_table, int);

#endif