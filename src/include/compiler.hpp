#include <string>
#include <vector>
#include <map>

#include "node.hpp"

#ifndef COMPILER_H
#define COMPILER_H

void compiler_info(std::string content);

bool is_included(std::string name);

bool is_funcall(std::string expr);
bool function_count_exist(std::string name, std::map<std::string, int *> funcs);

void count_functions_apparition(node *ast, std::map<std::string, int> &f_appartion);
void functions_definitions (node * &ast);



node *dead_code_remover_func(node *ast);
node *dead_code_remover_inline(node *ast);


node *optimisator(node *, int);

void includer(node *&ast);


std::string compute_convert_to_string(node *trunc);
std::string compiler(node *, int tabs);

#endif