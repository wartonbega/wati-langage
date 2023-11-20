#include <map>
#include <string>
#include <tuple>
#include <stack>
#include <future>
#include <thread>
#include "node.hpp"
#include "variables.hpp"

#ifndef COMPUTATIONS_HPP
#define COMPUTATIONS_HPP

bool is_binary_operator(node *);
bool is_unary_operator(node *);
void check_value_integrity(w_variable *value, std::string ref, int thread_id);
w_variable *get_name_for_vardef(node *, variable_table *&, std::string &, w_variable *&, int);
w_variable *call_object(std::string, node *, variable_table *, int);
w_variable *treate_multiple_values(node *c, w_variable *last, variable_table *varibales_t, int thread_id, w_variable *&self, std::string &last_name, bool);
w_variable *create_list(node *, variable_table *, w_variable *, int);
w_variable *use_tracker_def(node *, variable_table *, int);
w_variable *get_tracker_val(node *, variable_table *, int, std::string &, variable_table *&, w_variable *&, w_variable *&);
w_variable *visitor_link_unary_operator(node *op, variable_table *variables_t, int thread_id);
w_variable *visitor_link_logic_operator(node *op, variable_table *variables_t, int thread_id);
w_variable *visitor_compute_recursive(node *, variable_table *, w_variable *, int, bool, std::string &, std::string &);
w_variable *visitor_compute_recursive(node *, variable_table *, w_variable *, int);
w_variable *visitor_compute(node *, variable_table *, int);

#endif