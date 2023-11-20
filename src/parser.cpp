#include <iostream>
#include "include/parser.hpp"
#include "include/lexer.hpp"
#include "include/visitor.hpp"
#include "include/types.hpp"

std::vector<std::string> parser_keywords; // the vector containing all the keywords

void paser_init_keywords()
{
    parser_keywords.push_back("renvoie");
    parser_keywords.push_back("libere");
    parser_keywords.push_back("continue");
    parser_keywords.push_back("casse");
    parser_keywords.push_back("inclue");
    parser_keywords.push_back("tache"); // launch a thread
}

bool parser_is_keyword(std::string expr)
{
    // returns true wether expr is in keyword (std::vector<std::string>)
    for (auto i : parser_keywords)
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
bool parser_is_if_keyword(std::string expr)
{
    // Renvoie true si c'est un mot-clef à 'si'
    if (
        expr == "sinonsi" ||
        expr == "sinon")
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

node *recongize_expression_operator(node *trunc, std::string opera1, std::string opera2)
{
    // Reconnais les expression en fonction de l'opérateur
    node *patient = new node("*"); // Un containeur temporaire
    for (int i = 0; i < trunc->children.size(); i++)
    {
        if (trunc->children[i]->value == opera1 or trunc->children[i]->value == opera2)
        {
            node *first = new node(COMP_KEYWORD); // La première partie de l'expression

            first->push_multiple_childs(patient);

            node *second = new node(COMP_KEYWORD);
            for (int j = i + 1; j < trunc->children.size(); j++)
            {
                second->push_child(trunc->children[j]);
            }
            first = parser_eat_compute(first); // on traite récursivement le calcul qu'on viens de trouver
            second = parser_eat_compute(second);
            node *r = new node(trunc->children[i]->value);
            if (first->children.size() != 0)
                r->push_child(first);
            if (second->children.size() != 0)
                r->push_child(second);
            return r;
        }
        else
        {
            patient->push_child(trunc->children[i]);
        }
    }
    return nullptr;
}

node *recongize_single_expression_operator(node *trunc, std::string opera)
{
    // Reconnais les expression en fonction de l'opérateur
    for (int i = 0; i < trunc->children.size(); i++)
    {
        if (trunc->children[i]->value == opera)
        {
            node *full_r = new node(COMP_KEYWORD);
            node *r = new node(trunc->children[i]->value);
            r->reference = trunc->children[i]->reference;

            for (int j = 0; j < i; j++)
            {
                full_r->push_child(trunc->children[j]);
            }
            full_r->push_child(r);

            node *following = new node(COMP_KEYWORD);
            for (int j = i + 1; j < trunc->children.size(); j++)
            {
                following->push_child(trunc->children[j]);
            }
            following = parser_eat_compute(following);
            if (following->value == COMP_KEYWORD)
                r->push_multiple_childs(following);
            else
                r->push_child(following);
            if (full_r->children.size() == 1)
                return r;
            return full_r;
        }
    }
    return nullptr;
}

node *parser_eat_compute(node *trunc)
{
    // Ici, on organise tous les calculs
    // Sous la forme d'un arbre binaire

    node *p; // Contient temporairement les résultats

    // l'ordre des opérateurs est :
    // 1. + et -
    // 2. * et /
    // 3. ^
    // 4. < >
    // 5. <= >=
    // 6. ==
    // 7. || et &&

    // On cherche le premier symbole apparaissant dans la liste précédente

    // On traite maintenant les && et ||
    p = recongize_expression_operator(trunc, "&&", "||");
    if (p != nullptr)
        return p;

    p = recongize_expression_operator(trunc, "<", ">");
    if (p != nullptr)
        return p;

    p = recongize_expression_operator(trunc, "<=", ">=");
    if (p != nullptr)
        return p;

    p = recongize_expression_operator(trunc, "==", "!=");
    if (p != nullptr)
        return p;

    // On cherche les + et -
    p = recongize_expression_operator(trunc, "+", "-");
    if (p != nullptr)
        return p;

    // On cherche maintenant les * et /
    p = recongize_expression_operator(trunc, "*", "/");
    if (p != nullptr)
        return p;

    // On cherche maintenant les %
    p = recongize_expression_operator(trunc, "%", "%");
    if (p != nullptr)
        return p;

    // On traite maintenant les ^
    p = recongize_expression_operator(trunc, "^", "^");
    if (p != nullptr)
        return p;

    p = recongize_single_expression_operator(trunc, "!");
    if (p != nullptr)
    {
        if (p->children.size() != 0 and p->children[0]->value == COMP_KEYWORD)
        {                             // On enlève le COMP_KEYWORD dans l'expression pour avoir directement les enfants
            node *n = p->children[0]; // Le COMP_KEYWORD en question
            p->children = n->children;
        }
        return p;
    }

    p = recongize_expression_operator(trunc, "&", "&"); // Operateur à traqueur
    if (p != nullptr)
        return p;

    return trunc;
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
    elements->reference = trunc->reference;

    name = trunc->children[0]->children[0];
    if (is_char(name->value))
    { // Feature qui permet de donner des noms "compliqués" à la fonction
        // Utile par exemple dans fonctions.wati où on définis une fonction : "fonction.ne"
        name->value = del_string(name->value);
    }
    argument = trunc->children[0]->children[1];

    // The code '...'
    // Le code est définis à partir de 'fait' qui est donc en seconde place
    // après la définition des arguments
    for (int i = 2; i < trunc->children.size(); i++)
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
    func->reference = trunc->reference;
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
    elements->reference = trunc->reference;

    name = trunc->children[0]->children[0];
    argument = trunc->children[0]->children[1];

    // The code '...'
    for (int i = 2; i < trunc->children.size(); i++)
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
    meth->reference = trunc->reference;
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

    name = trunc->children[0]->children[0];
    if (trunc->children[1]->value != "contient")
    {
        error("Mauvais mot-clef : s'attendais à \"contient\", à eut \"" + trunc->children[1]->value + "\"", trunc->children[1]->reference, 0);
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
    classdef->reference = trunc->reference;
    classdef->push_child(name);
    classdef->push_child(attributes);
    classdef->push_child(methods);
    return classdef;
}

node *parser_eat_if(node *trunc)
{
    node *condition = new node(COMP_KEYWORD);
    node *code = new node("{}");
    int i = 0;

    node *res = new node("ifdec"); // ifdec is the codename
    res->reference = trunc->reference;
    res->push_child(condition);         // first condition
    res->push_child(code);              // then code
    code->reference = trunc->reference; // the reference for the code (we probably don't need it)

    while (trunc->children[i]->value != "alors" and trunc->children[i]->value != "fait")
    { // the condition before 'alors'
        condition->push_child(trunc->children[i]);
        condition->reference = trunc->children[i]->reference;
        i++;
    }
    node *sinonsi_holder = new node("elseif");
    node *sinonsi = NULL;
    node *sinonsi_condition;
    node *sinonsi_code;

    res->push_child(sinonsi_holder);

    node *sinon_holder = new node("else");
    res->push_child(sinon_holder);

    node *sinon = NULL;
    node *sinon_code;

    for (int y = i + 1; y < trunc->children.size(); y++)
    { // The code
        // We search ofr more complex declaration using 'sinonsi' 'sinon'
        if (trunc->children[y]->value == "sinonsi")
        {
            if (sinon != NULL)
            {
                std::string err = "sémantique : ne peux pas avoir de 'sinonsi' après un 'sinon'";
                error(err, trunc->children[y]->reference, 0);
            }
            if (sinonsi != NULL)
            {
                sinonsi_holder->push_child(sinonsi);
            }
            sinonsi = new node("-");
            sinonsi_condition = new node(COMP_KEYWORD);
            sinonsi_code = new node("{}");
            sinonsi->push_child(sinonsi_condition);
            sinonsi->push_child(sinonsi_code);
            y ++;
            while (trunc->children[y]->value != "alors" and trunc->children[y]->value != "fait")
            { 
                sinonsi_condition->push_child(trunc->children[y]);
                sinonsi_condition->reference = trunc->children[y]->reference;
                y++;
            }
            y++;
            code = sinonsi_code;
        }
        else if (trunc->children[y]->value == "sinon")
        {
            if (sinon != NULL)
            {
                std::string err = "ne peut pas avoir plusieurs 'sinon' dans une seul déclaration 'si'";
                error(err, trunc->children[y]->reference, 0);
            }
            sinon = new node("else");
            if (sinonsi != NULL)
            {
                sinonsi_holder->push_child(sinonsi);
            }
            sinon_code = new node("{}");
            sinon->push_child(sinon_code);
            code = sinon_code;
            y++;
        }
        code->push_child(trunc->children[y]);
    }
    if (sinon != NULL)
    {
        sinon_holder->push_child(sinon);
    }
    else if (sinonsi != NULL)
    {
        sinonsi_holder->push_child(sinonsi);
    }

    return res;
}

node *parser_eat_forloop(node *trunc)
{
    node *varname;
    node *borne = new node("borne");
    node *code = new node("{}");
    // pour | i (dans) iterable fait {} fin
    varname = trunc->children[0]->children[0];           // varname
    borne->reference = trunc->children[0]->children[2]->reference;
    node *comput = new node(COMP_KEYWORD);
    borne->push_child(comput);
    for (int i = 2; i < trunc->children[0]->children.size(); i++)
        comput->push_child(trunc->children[0]->children[i]); 
    
    for (int i = 2; i < trunc->children.size(); i++)
        code->push_child(trunc->children[i]);

    free(trunc);
    node *forloop = new node("forloop");
    forloop->reference = trunc->reference;
    forloop->push_child(varname);
    forloop->push_child(borne);
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
    total->reference = trunc->reference;
    node *condition = new node(COMP_KEYWORD); // contains both _num1 and _num2
    node *code = new node("{}");
    code->reference = trunc->reference;

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
        condition->reference = trunc->children[i]->reference;
        i++;
    }
    for (int y = i + 1; y < trunc->children.size(); y++) // y commence à (i + 1) car i c'est 'fait'
    {                                                    // the code
        code->push_child(trunc->children[y]);
    }

    total->push_child(condition);
    total->push_child(code);
    return total;
}

std::vector<std::string> insert(std::vector<std::string> l, std::string elemnt, int i)
{
    std::vector<std::string> r;
    for (int j = 0; j < i; j++)
    {
        r.push_back(l[j]);
    }
    r.push_back(elemnt);
    for (int j = i; j < l.size(); j++)
    {
        r.push_back(l[j]);
    }
    return r;
}

node *parser(std::vector<std::string> lexemes, std::string first_value, std::vector<std::string> ref, std::string first_ref)
{
    paser_init_keywords();
    // Parse the lexemes vector and returns an ast with all the instructions
    node *ast = new node(first_value); // the trunc of the ast
    if (!ref.empty())
        ast->reference = ref[0];
    else
        ast->reference = first_ref;
    for (int i = 0; i < lexemes.size(); i++)
    {
        if (lexemes[i] == "(")
        {
            std::vector<std::string> b;
            std::vector<std::string> r;
            i++;
            std::string c_reference = ref[i];
            int between = 0;
            while (true)
            {
                if (i >= lexemes.size())
                { // we reached eof
                    error("N'as pas put trouver de completion pour '('", c_reference, 0);
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
                r.push_back(ref[i]);
                i++;
            }
            node *parenthesis = parser(b, "()", r, c_reference);
            if (parenthesis->children.size() == 0)
            { // Il faut différencier le cas des parenthèses vide
                // Dans ce cas, on rajoute un COMP_KEYWORD dans la parenthèse pour éviter le plantage
                parenthesis->push_child(new node(COMP_KEYWORD)); // Rend le code plus flexible
            }
            if (ast->children.size() == 0 || ast->children[ast->children.size() - 1]->value != COMP_KEYWORD)
            {
                ast->push_child(new node(COMP_KEYWORD));
            }
            ast->children[ast->children.size() - 1]->push_child(parenthesis);
        }
        else if (lexemes[i] == "{")
        {
            std::vector<std::string> b;
            std::vector<std::string> r;
            i++;
            std::string c_reference = ref[i];
            int between = 0;
            while (true)
            {
                if (i >= lexemes.size())
                { // we reached eof
                    for (auto e : lexemes)
                        std::cout << " " << e;
                    error("N'as pas put trouver de completion pour '{'", c_reference, 0);
                }
                if (lexemes[i] == "}" and between == 0)
                {
                    break;
                }
                else if (lexemes[i] == "}")
                {
                    between--;
                }
                else if (lexemes[i] == "{")
                {
                    between++;
                }
                b.push_back(lexemes[i]);
                r.push_back(ref[i]);
                i++;
            }
            node *parenthesis = parser(b, "{}", r, c_reference);
            if (parenthesis->children.size() == 0)
            { // Il faut différencier le cas des parenthèses vide
                // Dans ce cas, on rajoute un COMP_KEYWORD dans la parenthèse pour éviter le plantage
                parenthesis->push_child(new node(COMP_KEYWORD)); // Rend le code plus flexible
            }
            if (ast->children.size() == 0 || ast->children[ast->children.size() - 1]->value != COMP_KEYWORD)
            {
                ast->push_child(new node(COMP_KEYWORD));
            }
            ast->children[ast->children.size() - 1]->push_child(parenthesis);
        }
        else if (lexemes[i] == "[")
        {
            std::vector<std::string> b;
            std::vector<std::string> r;
            i++;
            std::string c_reference = ref[i];
            int between = 0;
            while (true)
            {
                if (i >= lexemes.size())
                { // we reached eof
                    error("N'as pas put trouver de completion pour '['", c_reference, 0);
                }
                if (lexemes[i] == "]" and between == 0)
                {
                    break;
                }
                else if (lexemes[i] == "]")
                {
                    between--;
                }
                else if (lexemes[i] == "[")
                {
                    between++;
                }
                b.push_back(lexemes[i]);
                r.push_back(ref[i]);
                i++;
            }
            node *parenthesis = parser(b, "[]", r, c_reference);
            if (parenthesis->children.size() == 0)
            { // Il faut différencier le cas des parenthèses vide
                // Dans ce cas, on rajoute un COMP_KEYWORD dans la parenthèse pour éviter le plantage
                parenthesis->push_child(new node(COMP_KEYWORD)); // Rend le code plus flexible
            }
            if (ast->children.size() == 0 || ast->children[ast->children.size() - 1]->value != COMP_KEYWORD)
            {
                ast->push_child(new node(COMP_KEYWORD));
            }
            ast->children[ast->children.size() - 1]->push_child(parenthesis);
        }
        else if (lexemes[i] == "=")
        { // A variable assignement

            if (ast->children.size() > 0 and ast->children[ast->children.size() - 1]->value == COMP_KEYWORD)
            { // On traite le calcul qui a été enregistré tel quel
                ast->children[ast->children.size() - 1] = parser_eat_compute(ast->children[ast->children.size() - 1]);
            }
            // the disposition of the variable assignement node :
            // vardef
            //      name
            //      *
            //          ...
            std::string c_reference = ref[i];
            node *var_asign = new node("vardef");
            var_asign->reference = ref[i];

            if (ast->children.size() == 0 || ast->children[ast->children.size() - 1]->value != COMP_KEYWORD)
            {
                // [TODO] : error
            }

            node *name = ast->children[ast->children.size() - 1];
            var_asign->push_child(name);

            ast->children.pop_back();
            i++; // we pass the '='

            std::vector<std::string> b;
            std::vector<std::string> r;
            int between = 0;
            while (true)
            {
                if (i == lexemes.size())
                    error("N'a pas trouvé de completion pour la définition de la variable", c_reference, 0);
                if (between == 0 && lexemes[i] == ";")
                    break;
                if (lexer_is_opening_char(lexemes[i]))
                    between++;
                else if (lexer_is_closing_char(lexemes[i]))
                    between--;
                b.push_back(lexemes[i]);
                r.push_back(ref[i]);
                i++;
            }
            node *value = parser(b, COMP_KEYWORD, r, c_reference);
            var_asign->push_child(value);
            ast->push_child(var_asign);
        }
        else if (lexemes[i] == ",")
        {
            if (ast->children.size() > 0 and ast->children[ast->children.size() - 1]->value == COMP_KEYWORD)
            { // On traite le calcul qui a été enregistré tel quel
                ast->children[ast->children.size() - 1] = parser_eat_compute(ast->children[ast->children.size() - 1]);
            }
            ast->push_child(new node(","));
            ast->push_child(new node(COMP_KEYWORD));
        }
        else if (parser_is_keyword(lexemes[i]))
        {
            if (ast->children.size() > 0 and ast->children[ast->children.size() - 1]->value == COMP_KEYWORD)
            { // On traite le calcul qui a été enregistré tel quel
                ast->children[ast->children.size() - 1] = parser_eat_compute(ast->children[ast->children.size() - 1]);
            }
            // works the same way than a variable definition
            // keyword
            std::string c_reference = ref[i];
            node *keyword_call = new node(lexemes[i]);
            keyword_call->reference = ref[i];
            i++;
            std::vector<std::string> b;
            std::vector<std::string> r;
            bool untuched = false;
            int between = 0;
            while (true)
            {
                if (i == lexemes.size())
                    error("N'a pas trouvé de completion pour la définition de la variable", c_reference, 0);
                if (between == 0 && lexemes[i] == ";")
                    break;
                if (lexer_is_opening_char(lexemes[i]))
                    between++;
                else if (lexer_is_closing_char(lexemes[i]))
                    between--;
                b.push_back(lexemes[i]);
                r.push_back(ref[i]);
                i++;
            }
            node *value = parser(b, COMP_KEYWORD, r, c_reference);
            keyword_call->push_child(value);
            ast->push_child(keyword_call);
        }
        else if (parser_is_opening_keyword(lexemes[i]))
        {
            if (ast->children.size() > 0 and ast->children[ast->children.size() - 1]->value == COMP_KEYWORD)
            { // On traite le calcul qui a été enregistré tel quel
                ast->children[ast->children.size() - 1] = parser_eat_compute(ast->children[ast->children.size() - 1]);
            }

            std::string name = lexemes[i];
            std::string c_reference = ref[i];
            std::vector<std::string> b;
            std::vector<std::string> r;
            int true_ref_debug = i;
            int between = 0;
            i++;
            while (true)
            {
                if (i >= lexemes.size())
                {
                    error("N'as pas put trouver de completion pour '" + name + "'", c_reference, 0);
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
                r.push_back(ref[i]);
                i++;
            }
            node *function = parser(b, name, r, c_reference);

            if (name == "fonction")
                ast->push_child(parser_eat_function(function));
            else if (name == "methode")
                ast->push_child(parser_eat_method(function));
            else if (name == "classe")
                ast->push_child(parser_eat_class(function));
            else if (name == "si")
                ast->push_child(parser_eat_if(function));
            else if (name == "pour")
                ast->push_child(parser_eat_forloop(function));
            else if (name == "tant")
                ast->push_child(parser_eat_whileloop(function));
            // else if (name == "sinonsi")
            //     parser_eat_elseif(function, ast->children[ast->children.size() - 1]);
            // else if (name == "sinon")
            //     parser_eat_else(function, ast->children[ast->children.size() - 1]);
        }
        else if (lexemes[i] == "fait" || lexemes[i] == "que" || lexemes[i] == "alors" || lexemes[i] == "sinonsi" || lexemes[i] == "sinon" || lexemes[i] == "contient")
        { // Exceptions pour les boucles
            if (ast->children.size() > 0 and ast->children[ast->children.size() - 1]->value == COMP_KEYWORD)
            { // On traite le calcul qui a été enregistré tel quel
                ast->children[ast->children.size() - 1] = parser_eat_compute(ast->children[ast->children.size() - 1]);
            }
            ast->push_child(new node(lexemes[i]));
        }
        else if (lexemes[i] == ";")
        {
            if (ast->children.size() > 0 and ast->children[ast->children.size() - 1]->value == COMP_KEYWORD)
            { // On traite le calcul qui a été enregistré tel quel
                ast->children[ast->children.size() - 1] = parser_eat_compute(ast->children[ast->children.size() - 1]);
            }
            ast->push_child(new node(COMP_KEYWORD));
        }
        else
        {
            node *expr = new node(lexemes[i]);
            expr->reference = ref[i];
            if (ast->children.size() == 0 || ast->children[ast->children.size() - 1]->value != COMP_KEYWORD)
            {
                ast->push_child(new node(COMP_KEYWORD));
            }
            ast->children[ast->children.size() - 1]->push_child(expr);
            if (is_explicit(lexemes[i]))
            {
                w_variable *last_value;
                if (is_char(lexemes[i]))
                {
                    last_value = new w_variable(del_string(lexemes[i]));
                }
                else if (is_digit(lexemes[i]))
                {
                    last_value = new w_variable(atoi(lexemes[i].c_str()));
                }
                expr->set_prevalue(last_value); // Aussi, la variable est 'utilisée' une fois de manière indéfiniement longue
            }
        }
    }
    if (ast->children.size() > 0 and ast->children[ast->children.size() - 1]->value == COMP_KEYWORD)
    { // On traite le calcul qui a été enregistré tel quel
        // node *r = new node(COMP_KEYWORD);
        // r->push_child(parser_eat_compute(ast->children[ast->children.size() - 1]));
        ast->children[ast->children.size() - 1] = parser_eat_compute(ast->children[ast->children.size() - 1]);
    }
    return ast;
}