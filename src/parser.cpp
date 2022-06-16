#include <iostream>
#include "include/parser.hpp"

std::vector<std::string> keywords; // the vector containing all the keywords

void paser_init_keywords()
{
    keywords.push_back("renvoie");
    keywords.push_back("libere");
    keywords.push_back("casse");
}

bool parser_is_keyword(std::string expr)
{
    // returns true wether expr is in keyword (std::vector<std::string>)
    for (auto i : keywords)
    {
        if (i == expr)
        {
            return true;
        }
    }
    return false;
}

bool parser_is_opening_keyword(std::string expr)
{
    // returns true wether expr is a keyword that opens 'code brackets'
    if (
        expr == "si" ||
        expr == "fonction" ||
        expr == "classe" ||
        expr == "methode" ||
        expr == "pour" ||
        expr == "tant")
    {
        return true;
    }
    return false;
}

bool parser_is_closing_keyword(std::string expr)
{
    // returns true if expr == "fin" else false
    if (expr == "fin")
    {
        return true;
    }
    return false;
}

bool paser_is_funcall(std::string expr)
{ // returns true if the expression starts with '!' (a function call prefix)
    if (expr[0] == '!')
    {
        return true;
    }
    return false;
}

node *parser_eat_function(node *trunc)
{
    // trunc : the function's trunc :
    //     fonction
    //          name
    //          ()
    //                args
    //          fait
    //          ...
    node *argument;                  // All the arguments between ()
    node *name;                      // The name
    node *elements = new node("{}"); // The code inside 'fait' et 'fin'

    name = trunc->children[0];
    argument = trunc->children[1];
    if (trunc->children[2]->value != "fait")
    {
        // [TODO]: Error
    }

    // The code '...'
    for (int i = 3; i < trunc->children.size(); i++)
    {
        elements->push_child(trunc->children[i]);
    }

    // Push all the children in the good order :
    // funcdef
    //      name
    //      ()
    //          args
    //      {}
    //          ...

    node *func = new node("funcdef");
    func->push_child(name);
    func->push_child(argument);
    func->push_child(elements);

    free(trunc); // we don't need it anymore

    return func;
}

node *parser_eat_method(node *trunc)
{
    // trunc : the method's trunc :
    //     methode
    //          name
    //          ()
    //                args
    //          fait
    //          ...
    node *argument;                  // All the arguments between ()
    node *name;                      // The name
    node *elements = new node("{}"); // The code inside 'fait' et 'fin'

    name = trunc->children[0];
    argument = trunc->children[1];
    if (trunc->children[2]->value != "fait")
    {
        // [TODO]: Error
    }

    // The code '...'
    for (int i = 3; i < trunc->children.size(); i++)
    {
        elements->push_child(trunc->children[i]);
    }

    // Push all the children in the good order :
    // methdef
    //      name
    //      ()
    //          args
    //      {}
    //          ...

    node *meth = new node("methdef");
    meth->push_child(name);
    meth->push_child(argument);
    meth->push_child(elements);

    free(trunc); // we don't need it anymore

    return meth;
}

node *parser_eat_class(node *trunc)
{
    // the class
    // class
    //      name
    //      contient
    //      -> *methods
    //      -> *atributs
    node *name; // the name
    node *attributes = new node("attributes");
    node *methods = new node("methods");

    name = trunc->children[0];
    if (trunc->children[1]->value != "contient")
    {
        // [TODO]: Error
    }
    for (int i = 2; i < trunc->children.size(); i++)
    {
        if (trunc->children[i]->value == "methdef")
            methods->push_child(trunc->children[i]);
        if (trunc->children[i]->value == "vardef")
            attributes->push_child(trunc->children[i]);
    }
    // the order of the class definition :
    // classdef
    //      name
    //      * attributes
    //      * methods
    node *classdef = new node("classdef");
    classdef->push_child(name);
    classdef->push_child(attributes);
    classdef->push_child(methods);
    return classdef;
}

node *parser_eat_if(node *trunc)
{
    // The if declaration :
    // si
    //    *condition
    //    alors
    //    ...
    node *condition = new node("condition");
    node *code = new node("{}");
    int i = 0;

    // the result :
    // si
    //      condition
    //          *
    //      code
    //          ...

    while (trunc->children[i]->value != "alors" and trunc->children[i]->value != "fait")
    { // the condition before 'alors'
        condition->push_child(trunc->children[i]);
        i++;
    }

    for (int y = i + 1; y < trunc->children.size(); y++)
    { // The code
        code->push_child(trunc->children[y]);
    }

    // we don't need this
    free(trunc);

    node *res = new node("ifdec"); // ifdec is the codename
    res->push_child(condition);    // first condition
    res->push_child(code);         // then code
    return res;
}

node *parser_eat_forloop(node *trunc)
{
    // the for loop declaration
    // pour
    //      _varname    (0)
    //      allant    (1)
    //      de    (2)
    //      _num1    (3)
    //      a    (4)
    //      _num2    (5)
    //      fait    (6)
    //      ...    (7+)

    node *varname;
    node *bornes = new node("bornes"); // contains both _num1 and _num2
    node *code = new node("{}");

    // the result :
    // pour
    //      _varname
    //      bornes
    //          _num1
    //          _num2
    //      {}
    //          ...

    varname = trunc->children[0];           // varname
    bornes->push_child(trunc->children[3]); // _num1
    bornes->push_child(trunc->children[5]); // _num2

    for (int i = 7; i < trunc->children.size(); i++)
    { // the code
        code->push_child(trunc->children[i]);
    }

    free(trunc);
    node *forloop = new node("forloop");
    forloop->push_child(varname);
    forloop->push_child(bornes);
    forloop->push_child(code);
    return forloop;
}


node *parser_eat_whileloop(node *trunc)
{
    // the while loop declaration
    // tant 
    //      que       (0)
    //      condition (1)  
    //          ...       (*)
    //      fait    (+)
    //          ...    (+++)
    node *total = new node("whileloop");
    node *condition = new node("condition"); // contains both _num1 and _num2
    node *code = new node("{}");

    // the result :
    // tant
    //      condition
    //          ...
    //      {}
    //          ...
    int i = 1;
    while (trunc->children[i]->value != "fait")
    {
        condition->push_child(trunc->children[i]);
        i ++;
    }
    for (int y = i + 1; y < trunc->children.size(); y++)
    { // the code
        code->push_child(trunc->children[y]);
    }
    
    total->push_child(condition);
    total->push_child(code);
    return total;
}

node *parser(std::vector<std::string> lexemes, std::string first_value)
{
    paser_init_keywords();
    // Parse the lexemes vector and returns an ast with all the instructions
    node *ast = new node(first_value); // the trunc of the ast
    for (int i = 0; i < lexemes.size(); i++)
    {
        if (lexemes[i] == "(")
        {
            std::vector<std::string> b;
            i++;
            int between = 0;
            while (true)
            {
                if (i >= lexemes.size())
                { // we reached eof
                    std::cout << "END OF FILE" << std::endl;
                    exit(1);
                    // [TODO]: Errors
                }
                if (lexemes[i] == ")" and between == 0)
                {
                    break;
                }
                else if (lexemes[i] == ")")
                {
                    between--;
                }
                else if (lexemes[i] == "(")
                {
                    between++;
                }
                b.push_back(lexemes[i]);
                i++;
            }
            node *parenthesis = parser(b, "()");
            ast->push_child(parenthesis);
        }
        else if (lexemes[i] == "=")
        { // A variable assignement
            if (i > 1) // We need to know the variable name
            {
                // the disposition of the variable assignement node :
                // (node) variable
                // -> name
                // -> *
                //     -> ...
                node *var_asign = new node("vardef");
                node *name = new node(lexemes[i - 1]);
                var_asign->push_child(name);
                i++;
                std::vector<std::string> b;

                while (lexemes[i] != ";")
                {
                    b.push_back(lexemes[i]);
                    i++;
                }
                node *value = parser(b, "*");
                var_asign->push_child(value);
                ast->push_child(var_asign);
            }
        }
        else if (parser_is_keyword(lexemes[i]))
        { // works the same way than a variable definition
            node *keyword_call = new node(lexemes[i]);
            i++;
            std::vector<std::string> b;
            while (lexemes[i] != ";")
            {
                b.push_back(lexemes[i]);
                i++;
            }
            node *value = parser(b, "*");
            keyword_call->push_child(value);
            ast->push_child(keyword_call);
        }
        else if (parser_is_opening_keyword(lexemes[i]))
        {
            std::string name = lexemes[i];
            std::vector<std::string> b;
            int between = 0;
            i++;
            while (true)
            {
                if (i >= lexemes.size())
                {
                    std::cout << "END OF FILE (full declaration)" << std::endl;
                    exit(1);
                }
                if (parser_is_closing_keyword(lexemes[i]))
                {
                    if (between == 0)
                    {
                        break;
                    }
                    between -= 1;
                }
                else if (parser_is_opening_keyword(lexemes[i]))
                {
                    between++;
                }
                b.push_back(lexemes[i]);
                i++;
            }
            node *function = parser(b, name);
            if (name == "fonction")
                function = parser_eat_function(function);
            else if (name == "methode")
                function = parser_eat_method(function);
            else if (name == "classe")
                function = parser_eat_class(function);
            else if (name == "si")
                function = parser_eat_if(function);
            else if (name == "pour")
                function = parser_eat_forloop(function);
            else if (name == "tant")
                function = parser_eat_whileloop(function);
            ast->push_child(function);
        }
        else
        {
            if (lexemes[i + 1] != "=")
            {
                node *expr = new node(lexemes[i]);
                ast->push_child(expr);
            }
        }
    }
    return ast;
}