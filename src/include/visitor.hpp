#include <map>
#include <string>
#include <tuple>
#include "node.hpp"
#include "variables.hpp"

#ifndef VISITOR_H
#define VISITOR_H

void error(std::string err);
bool variable_exist(std::string name, std::map<std::string, w_variable*> variables_t);
void visitor_init_inbuild_functions();
node *visitor_separate_listed(node *parent);
w_variable *visitor_function_inbuild(std::string name, node *args, std::map<std::string, w_variable *> variables_t);
bool visitor_is_inbuild(std::string name);
w_variable *visitor_new_object(std::string name, node *args, std::map<std::string, w_variable *> variables_t);
w_variable *visitor_funcall(std::string name, node *args, std::map<std::string, w_variable *> variables_t);
w_variable *visitor_funcall_methode(std::string name, node *args, std::map<std::string, w_variable *> variables_t, w_variable *self);
w_variable *visitor_use_inbuild_int(int a, int b, std::string opera);
w_variable *visitor_use_inbuild_char(std::string a,  std::string b, std::string opera);
w_variable *visitor_use_inbuild(w_variable *a, w_variable *b, std::string opera);
w_variable *visitor_link_operator(w_variable *a, w_variable *b, std::string opera);
w_variable *visitor_compute(node *c, std::map<std::string, w_variable *> variables_t);
w_variable *visitor_keyword_return(node *trunc, std::map<std::string, w_variable *> variables_t);
void visitor_funcdef(node *trunc);
void visitor_vardef(node *trunc, std::map<std::string, w_variable *> &variables_t);
void visitor_classdef(node *trunc);
std::tuple<std::string, w_variable*> visitor_if_declaration(node *trunc, std::map<std::string, w_variable *> &variables_t);
std::tuple<std::string, w_variable*> visitor_forloop(node *trunc, std::map<std::string, w_variable *> &variables_t);
std::tuple<std::string, w_variable *> visitor_whileloop(node *trunc, std::map<std::string, w_variable *> &variables_t);
std::tuple<std::string, w_variable*> visitor_visit_incode(node *trunc, std::map<std::string, w_variable *> &variables_t);
w_variable *visitor_visit(node *trunc, std::map<std::string, w_variable *> variables_t);

#endif