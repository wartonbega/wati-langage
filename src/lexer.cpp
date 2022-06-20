#include "include/main.hpp"
#include "include/lexer.hpp"

std::vector<std::string> operators;

bool lexer_is_operator(std::string expr)
{
    if (expr == "^" ||
        expr == "==" ||
        expr == "!=" ||
        expr == "<=" ||
        expr == ">=" ||
        expr == "*" ||
        expr == "-" ||
        expr == "+" ||
        expr == "%" ||
        expr == "/" ||
        expr == "<" ||
        expr == ">")
    {
        return true;
    }
    return false;
}

bool lexer_is_withespace(std::string expr)
{
    if (expr == "\n")
    {
        return true;
    }
    if (expr == " ")
    {
        return true;
    }
    if (expr == "\t")
    {
        return true;
    }
    return false;
}

bool lexer_is_opening_char(std::string expr)
{
    if (expr == "(")
    {
        return true;
    }
    return false;
}

bool lexer_is_closing_char(std::string expr)
{
    if (expr == ")")
    {
        return true;
    }
    return false;
}

bool lexer_is_breaking_expr(std::string expr)
{
    if (lexer_is_operator(expr))
    {
        return true;
    }
    if (expr == ";" or expr == ",")
    {
        return true;
    }
    if (lexer_is_withespace(expr))
    {
        return true;
    }
    if (lexer_is_opening_char(expr))
    {
        return true;
    }
    if (lexer_is_closing_char(expr))
    {
        return true;
    }
    return false;
}

std::string add_char(char one, char two)
{
    std::string res;
    res += one;
    res += two;
    return res;
}

std::vector<std::string> lexer(std::string content, std::vector<std::string> &reference, std::string filename)
{
    std::vector<std::string> lexemes;
    std::string word;
    int lines = 1;
    int column = 1;

    int r_column = 1;
    int r_lines = 1;

    int index = 0;
    while (index < content.size())
    {
        if (content[index] == '\\' and content[index + 1] == '\\')
        { // this is a comment untill the end of the line
            while (content[index] != '\n')
            {
                index++;
                column++;
            }
        }
        if (content[index] == '\n')
        {
            lines++;
            column = 1;
        }
        if (content[index] == '"')
        {
            std::string between = "\"";
            index++;
            column ++;
            while (content[index] != '"')
            {
                if (content[index] == '\\' and content[index + 1] == 'n')
                {
                    if (content[index + 1] == 't')
                    {
                        index++;
                        column++;
                        between += "\t";
                    }
                    else if (content[index + 1] == 'n')
                    {
                        index++;
                        column++;
                        between += "\n";
                    }
                    else if (content[index + 1] == 'b')
                    {
                        index++;
                        column++;
                        between += '\b';
                    }
                }
                else
                    between += content[index];
                index++;
                column++;
            }
            between += '"';
            lexemes.push_back(between);
            reference.push_back(filename + ":" + std::to_string(lines) + ":" + std::to_string(column));
        }
        else if (index + 1 < content.size() and lexer_is_breaking_expr(add_char(content[index], content[index + 1])))
        {
            if (word != "")
            {
                lexemes.push_back(word);
                reference.push_back(filename + ":" + std::to_string(r_lines) + ":" + std::to_string(r_column));
                word = "";
            }
            lexemes.push_back(add_char(content[index], content[index + 1]));
            reference.push_back(filename + ":" + std::to_string(lines) + ":" + std::to_string(column));
            index++;
            column++;
            index++;
            column++;
        }
        else if (lexer_is_breaking_expr(std::string(1, content[index])))
        {
            if (word != "")
            {
                lexemes.push_back(word);
                reference.push_back(filename + ":" + std::to_string(r_lines) + ":" + std::to_string(r_column));
                word = "";
            }
            std::string ws(1, content.at(index));
            if (!lexer_is_withespace(ws))
            {
                lexemes.push_back(ws);
                reference.push_back(filename + ":" + std::to_string(lines) + ":" + std::to_string(column));
            }
        }
        else
        {
            word += content[index];
            r_lines = lines;
            r_column = column;
        }
        index++;
        column++;
    }
    if (word != "")
    {
        lexemes.push_back(word);
        reference.push_back(filename + ":" + std::to_string(r_lines) + ":" + std::to_string(r_column));
    }
    return lexemes;
}