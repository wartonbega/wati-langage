#include <string>
#include <vector>
#include "./node.hpp"

#ifndef PARSER_H
#define PARSER_H

#define COMP_KEYWORD "__compute__" // The keyword for computation in the ast

extern std::vector<std::string> parser_keywords; 

void paser_init_keywords();
bool parser_is_keyword(std::string);

bool parser_is_opening_keyword(std::string);
bool parser_is_closing_keyword(std::string);
bool parser_is_if_keyword(std::string expr);

node *recongize_expression_operator(node *, std::string, std::string);

node *parser_eat_compute(node *);
node *parser_eat_function(node *);
node *parser_eat_method(node *);
node *parser_eat_class(node *);
node *parser_eat_if(node *);
//node *parser_eat_elseif(node *, node *);
//node *parser_eat_else(node *, node *);
node *parser_eat_forloop(node *);

node *parser(std::vector<std::string> lexemes, std::string first_value, std::vector<std::string> ref, std::string first_ref);

#endif