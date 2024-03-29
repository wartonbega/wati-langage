#include <map>
#include <string>
#include <tuple>
#include <stack>
#include <future>
#include <thread>
#include "node.hpp"
#include "variables.hpp"

#ifndef VISITOR_H
#define VISITOR_H

extern std::vector<std::stack<std::string> *>references;

extern std::vector<std::string> inbuild_funcs;
extern std::vector<std::string> inbuild_funcs_documentation;
extern void *cpp_lib_handler;

extern int thread_utilized;

extern std::map<std::string, w_function *> functions;
extern std::map<std::string, std::string> functions_documentation;
extern std::map<std::string, w_class_template *> classes;

std::string ref_pop(int thread_id);

//void variable_asignement(std::string name, w_variable *v, variable_table  *variables_t);
variable_table prepare_arguments(std::vector<w_variable *>, node *, variable_table);
std::stack<std::string> *what_reference(int thread_id);
bool function_exist(std::string name, std::map<std::string, w_function *> funcs);
bool class_exist(std::string name, std::map<std::string, w_class_template *>);
std::vector<std::string> cut_error_ref(std::string ref);
std::string give_file_error(std::string filename, int line, int column, std::string full_ref);
void error(std::string err, std::string ref, int thread_id);
void warning(std::string err, std::string ref);
std::string remove_prefix(std::string name);
bool variable_exist(std::string name, std::map<std::string, w_variable*> variables_t);
void visitor_init_inbuild_functions();
std::vector<w_variable *> visitor_separate_listed_computed(node *parent, variable_table *variables_t, int thread_id);
node *visitor_separate_listed(node *parent);
w_variable *visitor_function_inbuild(std::string name, node *args, variable_table *variables_t, int thread_id);
bool visitor_is_inbuild(std::string name);
bool parentethis_is_listed(node *trunc);
w_variable *generate_function_variable(std::string name, int thread_id);

w_variable *visitor_prepare_variable_for_argument(w_variable *var, variable_table variables_t, int thread_id, std::string original_f_name);
w_variable *visitor_prepare_variable_for_copy(w_variable *var, variable_table variables_t, int thread_id);

w_variable *visitor_funcall(std::string name, node *args, variable_table variables_t, variable_table *variables_t_parent, int thread_id);
w_variable *visitor_funcall_methode(std::string name, node *args, variable_table variables_t, variable_table *variables_t_parent, w_variable *self, int thread_id);
w_variable *visitor_use_inbuild_int(int64_t a, int64_t b, std::string opera, int thread_id);
w_variable *visitor_use_inbuild_char(std::string a, std::string b, std::string opera, int thread_id);
w_variable *visitor_use_inbuild(w_variable *a, w_variable *b, std::string opera, int thread_id);
w_variable *visitor_link_operator(w_variable *a, w_variable *b, std::string opera, int thread_id);
w_variable *visitor_new_object(std::string name, node *args, variable_table variables_t, int thread_id);

w_variable *visitor_keyword_return(node *trunc, variable_table  variables_t, int thread_id);

void visitor_keyword_free(node *trunc, variable_table *variables_t, int thread_id);
void visitor_keyword_tache(node *trunc, variable_table variables_t, int thread_id);
void visitor_keyword_include(node *trunc, variable_table *variables_t, int thread_id);
void visitor_funcdef(node *trunc);
void visitor_vardef(node *trunc, variable_table *variables_t, int thread_id);
void visitor_classdef(node *trunc);

std::tuple<std::string, w_variable *> visitor_if_declaration(node *trunc, variable_table  *variables_t, int thread_id);
std::tuple<std::string, w_variable *> visitor_forloop(node *trunc, variable_table *variables_t, int thread_id);
std::tuple<std::string, w_variable *> visitor_whileloop(node *trunc, variable_table *variables_t, int thread_id);
std::tuple<std::string, w_variable *> visitor_visit_incode(node *trunc, variable_table *variables_t, int thread_id);
w_variable *visitor_visit(node *trunc, variable_table variables_t, int thread_id);

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
#define TERMINAL_ITALIC   std::string("\033[3m") // italic
#define TERMINAL_BOLDBLACK   std::string("\033[1m\033[30m")
#define TERMINAL_BOLDRED     std::string("\033[1m\033[31m")
#define TERMINAL_BOLDGREEN   std::string("\033[1m\033[32m")
#define TERMINAL_BOLDYELLOW  std::string("\033[1m\033[33m")
#define TERMINAL_BOLDBLUE    std::string("\033[1m\033[34m")
#define TERMINAL_BOLDMAGENTA std::string("\033[1m\033[35m")
#define TERMINAL_BOLDCYAN    std::string("\033[1m\033[36m")
#define TERMINAL_BOLDWHITE   std::string("\033[1m\033[37m")

#endif