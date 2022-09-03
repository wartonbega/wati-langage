
#include <string>
#include <map>
#include <vector>
#include <math.h>
#include "variables.hpp"
#include "node.hpp"

#ifndef INBUILD_H
#define INBUILD_H

w_variable *char_plus(std::string a, std::string b);
w_variable *char_equals(std::string a, std::string b);
w_variable *char_ne(std::string a, std::string b);
w_variable *char_minus(std::string a, std::string b);

w_variable *int_plus(int64_t a, int64_t b);
w_variable *int_minus(int64_t a, int64_t b);
w_variable *int_times(int64_t a, int64_t b);
w_variable *int_div(int64_t a, int64_t b);
w_variable *int_equals(int64_t a, int64_t b);
w_variable *int_lt(int64_t a, int64_t b);
w_variable *int_gt(int64_t a, int64_t b);
w_variable *int_ge(int64_t a, int64_t b);
w_variable *int_le(int64_t a, int64_t b);
w_variable *int_mod(int64_t a, int64_t b);
w_variable *int_power(int64_t a, int64_t b);
w_variable *int_ne(int64_t a, int64_t b);

std::string convert_to_string(w_variable *content, std::map<std::string, w_variable *> variables_t, int thread_id);
void print(w_variable *content, std::map<std::string, w_variable *> variables_t);
w_variable *type(w_variable *content);
w_variable *input(w_variable *content, std::map<std::string, w_variable *> variables_t);
w_variable *w_system(w_variable *content, std::map<std::string, w_variable *> variables_t);
w_variable *w_char(w_variable *content, std::map<std::string, w_variable *> variables_t, int thread_id);
void w_error(w_variable *content, std::map<std::string, w_variable *> variables_t, int thread_id);
void w_exit(w_variable *content);
w_variable *c_len(w_variable *content);
w_variable *w_en(std::vector<w_variable *>content, std::map<std::string, w_variable *> variables_t);
w_variable *w_time();
#endif