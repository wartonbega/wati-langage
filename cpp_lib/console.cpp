#include <vector>
#include <iostream>
#include <map>
#include <string>
#include <math.h>

#include <chrono>
#include <ctime>

#include "../src/include/node.hpp"
#include "../src/include/variables.hpp"
#include "../src/include/trackers.hpp"
#include "../src/include/visitor.hpp"
#include "../src/include/inbuilds.hpp"

#include "../src/include/lexer.hpp"
#include "../src/include/parser.hpp"
#include "../src/include/visitor.hpp"

#include "../src/include/main.hpp"

bool is_opening_keyword(std::string c)
{
    if (
        c == "si" ||
        c == "fonction" ||
        c == "classe" ||
        c == "methode" ||
        c == "pour" ||
        c == "tant" ||
        c == "(" ||
        c == "[" ||
        c == "=" ||
        parser_is_keyword(c))

    {
        return true;
    }
    return false;
}
bool is_scope_opening_keyword(std::string c)
{ // Une légère modification de is_opening_keyword mais qui ne prend que les mots clef
    // qui ouvrent des bouts de code et qui se ferment par 'fin'
    if (
        c == "si" ||
        c == "fonction" ||
        c == "classe" ||
        c == "methode" ||
        c == "pour" ||
        c == "tant")
    {
        return true;
    }
    return false;
}

bool is_closing_keyword(std::string c)
{
    if (c == "fin" || c == ")" || c == "]" || c == ";")
    {
        return true;
    }
    return false;
}

bool is_spaces(std::string c)
{
    for (auto e : c)
        if (e != ' ')
            return false;
    return true;
}

bool is_breaking_caracter(std::string c)
{
    if (is_spaces(c) || c == "," || c == "(" || c == ")" || c == "[" || c == "]" || c == "\"" || c == "." || c == "\n" || c == ";" || c == "!" || lexer_is_operator(c) || c == "{" || c == "}")
        return true;
    return false;
}

bool is_beggining_caracter(std::string c)
{ // Renvoie si c'est un caractère qui peut ouvrire un mot
    if (c == "." || c == "!" || lexer_is_operator(c))
        return true;
    return false;
}

bool is_number(std::string s)
{
    std::string::const_iterator it = s.begin();
    while (it != s.end() && std::isdigit(*it))
        ++it;
    return !s.empty() && it == s.end();
}

bool is_arrow_key(std::string s)
{
    if (s == "DROITE" || s == "GAUCHE" || s == "HAUT" || s == "BAS")
        return true;
    return false;
}

std::string matching_opening_keyword(std::vector<std::string> &closed_keyword)
{
    if (closed_keyword.size() == 0)
        return "";
    std::string res = closed_keyword[closed_keyword.size() - 1];
    closed_keyword.pop_back();
    return res;
}

void print_num_char(char chr)
{ // Utilisé pour le debug : permet de comprendre quel touche est pressée
    std::cout << "  >" << std::to_string((int)chr) << "<  ";
}

std::string input()
{ // we use c input to get one caracter by one
    unsigned char chr;
    chr = getchar();
    // print_num_char(chr);
    std::string res = "";

    switch (chr)
    {
    case 127: // retour en arrière '\b'
        res += "\b";
        break;
    case 10:
        res += "\n";
        break;
    case 4: // Ctrl + D
        exit(0);
        break;
    case 11: // Ctrl + D
        system("clear");
        std::cout << ">>> ";
        break;
    case '\t':
    {
        res += "    ";
        break;
    }
    case 27:
    { // Ce sont les touches "spéciales"
        // Permet de faire des 'suites de touches'
        res += "ESP";
        break;

        char one = getchar();
        if (one == 91)
        {
            char two = getchar();
            if (two == 67)
                res += "DROITE";
            else if (two == 68)
                res += "GAUCHE";
            else if (two == 65)
                res += "HAUT";
            else if (two == 66)
                res += "BAS";
        }
    }
    case 195:
    {
        unsigned char one = getchar(); // Le second caractère
        if (one == 160)
            res += "à";
        else if (one == 169)
            res += "é";
        else if (one == 168)
            res += "è";
        else if (one == 185)
            res += "ù";
        else if (one == 180)
            res += "ê";
        else if (one == 171)
            res += "ë";
        else
            res += "¿";
        res = "";
        break;
    }
    default:
    {
        res += chr;
        std::cout << res;
    }
    }
    return res;
}

bool match_opening_keywords(std::string word, std::vector<std::string> &opended_keywords, std::vector<std::string> &closed_keywords, std::vector<std::string> &stack_error)
{                                                    // On vérifie ici que les mots-clefs se competent bien
    if (opended_keywords.size() == 0 && word != ";") // On peut mettre des ; partout où on veut
    {
        std::cout << TERMINAL_RED + "Ne peut pas matcher '" + word + "'" + TERMINAL_RESET << std::endl;
        return false;
    }
    std::string last = opended_keywords[opended_keywords.size() - 1];
    bool to_ret = true;
    if (word == ")")
    {
        if (last != "(")
        {
            std::cout << TERMINAL_RED + "N'a pas matché ')' : on a eut '" + last + "'" + TERMINAL_RESET << std::endl;
            to_ret = false;
        }
    }
    else if (word == "]")
    {
        if (last != "[")
        {
            std::cout << TERMINAL_RED + "N'a pas matché ']' : on a eut '" + last + "'" + TERMINAL_RESET << std::endl;
            to_ret = false;
        }
    }
    else if (word == ";")
        if (last == "=" || parser_is_keyword(last))
            to_ret = true;
        else
            to_ret = false;
    else if (word == "fin")
    {
        if (!is_scope_opening_keyword(last))
        {
            std::cout << TERMINAL_RED + "N'a pas matché '" + last + "' : on a eut 'fin'" + TERMINAL_RESET << std::endl;
            to_ret = false;
        }
        
    }

    if (to_ret)
    {
        opended_keywords.pop_back();
        closed_keywords.push_back(last);
    }
    return to_ret;
}

std::string generate_word(int number, std::string filling)
{
    std::string res = "";
    for (int i = 0; i < number; i++)
        res += filling;
    return res;
}

std::string treat_backspace(std::vector<std::string> &line)
{
    if (line.size() == 0)
        return "";

    std::string last_char = "";
    last_char = line[line.size() - 1];
    line.pop_back();

    std::string backspaces = "";
    std::string spaces = "";
    for (auto _ : last_char)
    {
        backspaces += "\b";
        spaces += " ";
    }
    std::cout << backspaces << spaces << backspaces;
    if (line.size() == 0)
        return "";

    std::string word = "";
    int i = line.size();
    while ((!(is_breaking_caracter(line[i - 1])) or is_beggining_caracter(line[i - 1])) and i > 0)
    {
        i--;
        word = line[i] + word;
    }
    return word;
}

void syntax_highliting(std::string word)
{
    if (word.size() == 0)
        return;

    std::string backspaces = "";
    for (auto _ : word)
        backspaces += "\b";
    std::cout << backspaces;

    std::string colors = "";
    if (is_number(word))
        colors += TERMINAL_BOLDWHITE;
    else if (word[0] == '!') // C'est une fonction qu'on a
    {                        // On veut colorier le nom de la fonction, mais pas le '!'
        std::cout << "!";
        for (int i = 1; i < word.size(); i++) // On shift le mot pour enlever le '!'
            word[i - 1] = word[i];
        word.pop_back();

        colors += TERMINAL_CYAN;
    }
    else if (word[0] == '.')
        colors += TERMINAL_ITALIC;
    else if (lexer_is_operator(word))
        colors += TERMINAL_MAGENTA;
    else if (parser_is_keyword(word))
        colors += TERMINAL_RED;
    else if (word == "alors" || word == "fait" || word == "fin" || word == "contient") // les mots annonçant un bloc de code
        colors += TERMINAL_MAGENTA;
    else if (word == "pour" || word == "tant" || word == "que" || word == "dans") // les boucles
        colors += TERMINAL_GREEN;
    else if (word == "si" || word == "sinonsi" || word == "sinon" || word == "fonction" || word == "methode" || word == "classe") // Les mots-clef usuels
        colors += TERMINAL_BLUE;
    else if (word == "self")
        colors += TERMINAL_ITALIC + TERMINAL_YELLOW;

    std::cout << colors << word << TERMINAL_RESET;
}

std::string completion(std::string word, std::vector<std::string> registered_names)
{
    if (word.size() <= 1)
        return "";
    for (auto p : registered_names)
    {
        if (p.size() <= word.size())
            continue;
        int i = 0;
        while (word[i] == p[i] and i < word.size()) // On chercher le nombre maximum de caractères en commun
            i++;

        if (i == word.size()) // si les caractères en commun correspondent au mot, alors c'est bon
        {
            std::string res = "";
            for (int j = i; j < p.size(); j++)
                res += p[j];
            return res;
        }
    }
    return "";
}

void treat_input_char(std::string &e, std::string &word, std::vector<std::string> &line, std::string &complet, std::vector<std::string> &opened_keywords, std::vector<std::string> &closed_keywords, std::vector<std::string> &registered_names, bool &enable_completion, bool &string_state)
{
    std::vector<std::string> stack_errors;
    if (e == "\"")
    {
        string_state = !string_state;
        if (string_state)
            std::cout << "\b" + TERMINAL_YELLOW + "\"";
        else
            std::cout << TERMINAL_RESET;
        line.push_back(e);
        return;
    }
    if (string_state && e == "\b")
    {
        std::string p = line[line.size() - 1];
        if (p == "\"")
        {
            string_state = !string_state;
            std::cout << TERMINAL_RESET;
        }
        try
        {
            word = treat_backspace(line);
        }
        catch (std::bad_alloc error)
        {
            return;
        }
        return;
    }
    else if (string_state && e == "\n")
    {
        line.push_back(e);
        e = "";
    }
    else if (string_state)
    {
        line.push_back(e);
        return;
    }
    if (e == "    " and complet == "")
        std::cout << e;
    if (complet.size() >= 1)
        std::cout << generate_word(complet.size(), " ") << generate_word(complet.size(), "\b");
    if (e == "\b")
    { // un peut dégueu
        // Mais globalement :
        // on vérifie que le mot en traitement n'est pas un mot d'ouverture / fermeture de bloc de code
        // puis on supprime le mot de la pile des mots ouvert si le mot étant supprimé est un mot d'ouverture
        // ou de rouvrire un bloc de code si le mot supprimé est un mot de fermeture
        if (is_opening_keyword(word))
            opened_keywords.push_back(word);
        if (is_closing_keyword(word))
            match_opening_keywords(word, opened_keywords, closed_keywords, stack_errors);

        if (line.size() > 0 && opened_keywords.size() > 0 && is_opening_keyword(word))
            opened_keywords.pop_back();
        else if (line.size() > 0 && opened_keywords.size() > 0 && is_opening_keyword(line[line.size() - 1]))
            opened_keywords.pop_back();

        if (line.size() > 0 && is_closing_keyword(word))
            opened_keywords.push_back(matching_opening_keyword(closed_keywords));
        else if (line.size() > 0 && is_closing_keyword(line[line.size() - 1]))
            opened_keywords.push_back(matching_opening_keyword(closed_keywords));

        // Il y a manifestement une erreur liée au \b. On corrige rapidement
        try
        {
            word = treat_backspace(line);
        }
        catch (std::bad_alloc error)
        {
            return;
        }
    }
    else if (e == "ESP")
        enable_completion = false;
    else if ((e == "    " or e == "\n") and complet != "")
    {
        for (auto ch : complet)
        {
            word += ch;
            std::cout << ch;
            line.push_back(std::string("") + ch);
        }
        e = "";
    }
    else if (is_breaking_caracter(e))
    {
        enable_completion = true;
        if (is_opening_keyword(word))
            opened_keywords.push_back(word);
        if (is_opening_keyword(e))
            opened_keywords.push_back(e);
        if (is_closing_keyword(word))
            match_opening_keywords(word, opened_keywords, closed_keywords, stack_errors);
        if (is_closing_keyword(e))
            match_opening_keywords(e, opened_keywords, closed_keywords, stack_errors);
        word = is_beggining_caracter(e) ? e : "";
        line.push_back(e);
    }
    else
    {
        line.push_back(e);
        word += e;
    }
    syntax_highliting(word);
    if (lexer_is_operator(word))
        word = "";
    if (enable_completion)
        complet = completion(word, registered_names);
    else
        complet = "";

    // On écrit l'autocomplétion
    std::cout << TERMINAL_REVERSE << complet << generate_word(complet.size(), TERMINAL_REVERSE + "\b") << TERMINAL_RESET;
}

std::string input_line(std::vector<std::string> &opened_keywords, std::vector<std::string> &closed_keywords, std::vector<std::string> registered_names)
{
    std::vector<std::string> line; // l'entièreté de la ligne, caractère par caractère
    std::string full_line = "";
    std::string e = "";    // le caractère en cours de traitement
    std::string word = ""; // construction des mots pour la coloration syntaxique

    if (opened_keywords.size() != 0)
        std::cout << "••• ";
    else
        std::cout << ">>> ";
    std::string complet = ""; // le mot de 'completion'
    bool enable_completion = true;
    bool string_state = false;

    while (e != "\n")
    {
        e = input();
        treat_input_char(e, word, line, complet, opened_keywords, closed_keywords, registered_names, enable_completion, string_state);
    }

    for (auto c : line)
        full_line += c;
    return full_line;
}

bool contains(std::string t, std::vector<std::string> l)
{
    for (auto e : l)
        if (t == e)
            return true;
    return false;
}

extern "C" w_variable *CLI(std::vector<w_variable *> args, variable_table variables_t, std::string reference, int thread_id)
{ // Console line interpreter
    system("stty -echo");
    if (args.size() != 0)
        error("!CLI ne prend pas d'arguments", reference, thread_id);

    std::vector<std::string> opened_keywords; // On vas utiliser ce vecteur comme une pile
    std::vector<std::string> closed_keywords; // On vas utiliser ce vecteur comme une pile (aussi)
    std::string total = "";
    std::vector<std::string> registered_names;
    registered_names.push_back("fonction");
    registered_names.push_back("classe");
    registered_names.push_back("contient");
    registered_names.push_back("methode");
    registered_names.push_back("pour");
    registered_names.push_back("dans");
    registered_names.push_back("tant");
    registered_names.push_back("self");

    for (auto key : parser_keywords)
        registered_names.push_back(key);

    for (auto key : inbuild_funcs)
        registered_names.push_back(key);

    for (auto const &[key, val] : functions)
        registered_names.push_back(key);

    for (auto const &[key, val] : classes)
        registered_names.push_back(key);

    // Le but est de pouvoir matcher toutes les expression qu'on y met
    while (1)
    {
        std::string r = input_line(opened_keywords, closed_keywords, registered_names);
        total += r;
        std::cout << std::endl;
        if (opened_keywords.size() == 0)
        {
            std::cout << "\r    \r";
            std::vector<std::string> ref;
            std::vector<std::string> lexemes = lexer(total, ref, reference);
            node *trunc = parser(lexemes, "execution", ref, reference);
            std::tuple<std::string, w_variable *> ret;

            try
            {
                ret = visitor_visit_incode(trunc, &variables_t, thread_id);
            }
            catch (w_variable *error)
            {
                std::cout << "—————————— " << TERMINAL_BOLDRED << "ERREUR" << TERMINAL_RESET << " ——————————";
                std::cout << throw_error(error) << std::endl;
                std::cout << "—————————————————————————————\n";
            }

            if (std::get<1>(ret) != nullptr)
            {
                // if (!(std::get<1>(ret)->get_type() == "int" and std::get<1>(ret)->convert_to_int() == 0))
                {
                    std::string to_print = beautify_for_print(std::get<1>(ret), variables_t);
                    std::cout << to_print << std::endl;
                }
            }
            for (auto const &[key, _] : variables_t.vars)
                registered_names.push_back(key);
            for (auto const &[key, val] : functions)
            {
                if (!contains(key, registered_names))
                    registered_names.push_back(key);
            }
            total = "";
        }
    }
    return new w_variable(0);
}
