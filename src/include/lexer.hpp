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

bool lexer_is_stoping_expr(std::string expr);

std::vector<std::string> lexer(std::string content, std::vector<std::string> &reference, std::string filename);

#endif