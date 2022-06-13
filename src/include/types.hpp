#include <string>
#include "variables.hpp"

#ifndef TYPES_H
#define TYPES_H

bool is_char(std::string);
bool is_digit(std::string);
bool is_explicit(std::string);

bool is_operator(std::string);
std::string del_string(std::string expr);
bool is_keyword(std::string expr);
bool is_attributed(std::string expr);
bool is_int_true(w_variable *r);
#endif