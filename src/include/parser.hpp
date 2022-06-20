#include <string>
#include <vector>
#include "./node.hpp"

#ifndef PARSER_H
#define PARSER_H

void paser_init_keywords();
bool parser_is_keyword(std::string);

bool parser_is_opening_keyword(std::string);
bool parser_is_closing_keyword(std::string);

node *parser_eat_function(node *);
node *parser_eat_method(node *);
node *parser_eat_class(node *);
node *parser_eat_if(node *);
node *parser_eat_forloop(node *);

node *parser(std::vector<std::string> lexemes, std::string first_value, std::vector<std::string> ref, std::string first_ref);

#endif