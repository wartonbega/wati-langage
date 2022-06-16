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

std::vector<std::string> lexer(std::string content, std::vector<std::string> &reference)
{
    std::vector<std::string> lexemes;
    std::string word;
    int lines = 0;
    int column = 0;

    int index = 0;
    while (index < content.size())
    {
        if (content[index] == '\n')
        {
            lines ++;

        }
        if (content[index] == '"')
        {
            std::string between = "\"";
            index++;
            while (content[index] != '"')
            {
                if (content[index] == '\\' and content[index + 1] == 'n')
                {
                    index++;
                    between += "\n";
                }
                else
                    between += content[index];
                index++;
            }
            between += '"';
            lexemes.push_back(between);
        }
        else if (index + 1 < content.size() and lexer_is_breaking_expr(add_char(content[index], content[index + 1])))
        {
            if (word != "")
            {
                lexemes.push_back(word);
                word = "";
            }
            lexemes.push_back(add_char(content[index], content[index + 1]));
            index++;
            index++;
        }
        else if (lexer_is_breaking_expr(std::string(1, content[index])))
        {
            if (word != "")
            {
                lexemes.push_back(word);
                word = "";
            }
            std::string ws(1, content.at(index));
            if (!lexer_is_withespace(ws))
            {
                lexemes.push_back(ws);
            }
        }
        else
        {
            word += content[index];
        }
        index++;
    }
    if (word != "")
    {
        lexemes.push_back(word);
    }
    return lexemes;
}