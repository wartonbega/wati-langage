
#include "include/types.hpp"
#include "include/lexer.hpp"

bool is_char(std::string expr)
{
    if (expr[0] == '\"' and expr[expr.size() - 1] == '\"')
    {
        return true;
    }
    return false;
}

bool is_digit(std::string expr)
{
    for (auto i : expr)
    {
        if (!(i == '0' ||
              i == '1' ||
              i == '2' ||
              i == '3' ||
              i == '4' ||
              i == '5' ||
              i == '6' ||
              i == '7' ||
              i == '8' ||
              i == '9'))
            return false;
    }
    return true;
}

bool is_explicit(std::string expr)
{
    if (is_char(expr) or is_digit(expr))
    {
        return true;
    }
    return false;
}

bool is_operator(std::string expr)
{
    return lexer_is_operator(expr);
}

std::string del_string(std::string expr)
{
    std::string res;
    for (int i = 1; i + 1 < expr.size(); i++)
    {
        res += expr[i];
    }
    return res;
}

bool is_keyword(std::string expr)
{
    if (expr == "renvoie")
        return true;
    return false;
}

bool is_attributed(std::string expr)
{
    for (auto i : expr)
    {
        if (i == '.')
            return true;
    }
    return false;
}

bool is_int_true(w_variable *r)
{
    if (r->get_type() != "int")
        return false;
    
    if (r->convert_to_int() == 1)
        return true;

    return false;
}