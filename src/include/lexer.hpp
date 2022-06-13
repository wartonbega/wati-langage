#include <string>
#include <vector>

#ifndef LEXER_H
#define LEXER_H

void lexer_init_operators();

bool lexer_is_closing_char(std::string expr);

bool lexer_is_opening_char(std::string expr);

bool lexer_is_operator(std::string expr);

bool lexer_is_withespace(std::string expr);

bool lexer_is_breaking_expr(std::string);

std::vector<std::string> lexer(std::string content);

#endif