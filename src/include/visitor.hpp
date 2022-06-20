#include <map>
#include <string>
#include <tuple>
#include <stack>
#include "node.hpp"
#include "variables.hpp"

#ifndef VISITOR_H
#define VISITOR_H

extern std::stack<std::string> references;

std::vector<std::string> cut_error_ref(std::string ref);
std::string give_file_error(std::string filename, int line, int column, std::string full_ref);
void error(std::string err, std::string ref);
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
void visitor_keyword_include(node *trunc, std::map<std::string, w_variable *> variables_t);
void visitor_keyword_free(node *trunc, std::map<std::string, w_variable *> variables_t);
w_variable *visitor_keyword_return(node *trunc, std::map<std::string, w_variable *> variables_t);
void visitor_funcdef(node *trunc);
void visitor_vardef(node *trunc, std::map<std::string, w_variable *> &variables_t);
void visitor_classdef(node *trunc);
std::tuple<std::string, w_variable*> visitor_if_declaration(node *trunc, std::map<std::string, w_variable *> &variables_t);
std::tuple<std::string, w_variable*> visitor_forloop(node *trunc, std::map<std::string, w_variable *> &variables_t);
std::tuple<std::string, w_variable *> visitor_whileloop(node *trunc, std::map<std::string, w_variable *> &variables_t);
std::tuple<std::string, w_variable*> visitor_visit_incode(node *trunc, std::map<std::string, w_variable *> &variables_t);
w_variable *visitor_visit(node *trunc, std::map<std::string, w_variable *> variables_t);

#define TERMINAL_RESET   std::string("\033[0m")
#define TERMINAL_BLACK   std::string("\033[30m")      
#define TERMINAL_RED     std::string("\033[31m")      
#define TERMINAL_GREEN   std::string("\033[32m")      
#define TERMINAL_YELLOW  std::string("\033[33m")      
#define TERMINAL_BLUE    std::string("\033[34m")      
#define TERMINAL_MAGENTA std::string("\033[35m")      
#define TERMINAL_CYAN    std::string("\033[36m")      
#define TERMINAL_WHITE   std::string("\033[37m")      
#define TERMINAL_BOLD     std::string("\033[1m")
#define TERMINAL_UNDERLINE std::string("\033[4m")
#define TERMINAL_REVERSE std::string("\033[7m") // reverse color
#define TERMINAL_BLINK   std::string("\033[5m") // blink
#define TERMINAL_BOLDBLACK   std::string("\033[1m\033[30m")
#define TERMINAL_BOLDRED     std::string("\033[1m\033[31m")
#define TERMINAL_BOLDGREEN   std::string("\033[1m\033[32m")
#define TERMINAL_BOLDYELLOW  std::string("\033[1m\033[33m")
#define TERMINAL_BOLDBLUE    std::string("\033[1m\033[34m")
#define TERMINAL_BOLDMAGENTA std::string("\033[1m\033[35m")
#define TERMINAL_BOLDCYAN    std::string("\033[1m\033[36m")
#define TERMINAL_BOLDWHITE   std::string("\033[1m\033[37m")

#endif