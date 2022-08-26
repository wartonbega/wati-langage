//#include <iostream>
//
//#include "./include/compiler.hpp"
//#include "./include/visitor.hpp"
//
//#include "./include/parser.hpp"
//#include "./include/lexer.hpp"
//#include "./include/main.hpp"
//
//#include "./include/types.hpp"
//
//std::string dirname;
//
//std::map<std::string, int> variables_apparition;
//
//std::map<std::string, node *> functions_definition;
//
//std::vector<std::string> includes;
//
//void compiler_info(std::string content)
//{
//    std::cout << TERMINAL_BOLDYELLOW + "info : " + TERMINAL_RESET + content << std::endl;
//}
//
//bool is_included(std::string name)
//{
//    for (auto i : includes)
//    {
//        if (i == name)
//        {
//            return true;
//        }
//    }
//    return false;
//}
//
//bool function_count_exist(std::string name, std::map<std::string, int> funcs)
//{
//    if (funcs.find(name) != funcs.end() || visitor_is_inbuild(name))
//    {
//        return true;
//    }
//    return false;
//}
//
//bool function_definition_exist(std::string name, std::map<std::string, node *> funcs)
//{
//    if (visitor_is_inbuild(name))
//    {
//        return false;
//    }
//    if (funcs.find(name) != funcs.end())
//    {
//        return true;
//    }
//    return false;
//}
//
//bool is_funcall(std::string expr)
//{
//    if (expr[0] == '!')
//    {
//        return true;
//    }
//    return false;
//}
//
//node *dead_code_remover_func(node *ast)
//{ // just ignore the rest of the code after "renvoie" keyword, because it will never be executed
//    node *n_ast = new node(ast->value);
//    for (auto instruction : ast->children)
//    {
//        n_ast->push_child(instruction);
//        if (instruction->value == "renvoie")
//        {
//            break;
//        }
//
//        else if (instruction->value == "ifdec")
//        {
//            node *code = instruction->children[1]; // {}
//            instruction->children[1] = dead_code_remover_inline(code);
//            for (auto elseif : instruction->children[2]->children)
//            {
//                node *ei_code = elseif->children[1]; // {}
//                elseif->children[1] = dead_code_remover_inline(ei_code);
//            }
//            if (instruction->children[3]->children.size() != 0)
//            {
//                node *else_code = instruction->children[3]->children[0]->children[0];
//                instruction->children[3]->children[0]->children[0] = dead_code_remover_inline(else_code);
//            }
//        }
//        else if (instruction->value == "forloop")
//        {
//            node *code = instruction->children[2]; // {}
//            instruction->children[2] = dead_code_remover_inline(code);
//        }
//        else if (instruction->value == "whileloop")
//        {
//            node *code = instruction->children[1]; // {}
//            instruction->children[1] = dead_code_remover_inline(code);
//        }
//        else if (instruction->value == "funcdef")
//        {
//            node *code = instruction->children[2];
//            instruction->children[2] = dead_code_remover_func(code);
//        }
//        else if (instruction->value == "classdef")
//        {
//            for (auto funcdef : instruction->children[1]->children)
//            {
//                node *code = funcdef->children[2];
//                funcdef->children[2] = dead_code_remover_func(code);
//            }
//        }
//        else if (instruction->value == "casse")
//        {
//            std::string err = "ne peux pas supporter le mot-clé 'casse' en dehors d'une boucle";
//            error(err, instruction->reference, 0);
//        }
//    }
//    return n_ast;
//}
//
//node *dead_code_remover_inline(node *ast)
//{ // just ignore the rest of the code after "renvoie" and "casse" keyword, because it will never be executed
//    node *n_ast = new node(ast->value);
//    for (auto instruction : ast->children)
//    {
//        n_ast->push_child(instruction);
//        if (instruction->value == "renvoie")
//        {
//            break;
//        }
//        else if (instruction->value == "casse")
//        {
//            break;
//        }
//        else if (instruction->value == "ifdec")
//        {
//            node *code = instruction->children[1]; // {}
//            instruction->children[1] = dead_code_remover_inline(code);
//            for (auto elseif : instruction->children[2]->children)
//            {
//                node *ei_code = elseif->children[1]; // {}
//                elseif->children[1] = dead_code_remover_inline(ei_code);
//            }
//            if (instruction->children[3]->children.size() != 0)
//            {
//                node *else_code = instruction->children[3]->children[0]->children[0];
//                instruction->children[3]->children[0]->children[0] = dead_code_remover_inline(else_code);
//            }
//        }
//        else if (instruction->value == "forloop")
//        {
//            node *code = instruction->children[2]; // {}
//            instruction->children[2] = dead_code_remover_inline(code);
//        }
//        else if (instruction->value == "whileloop")
//        {
//            node *code = instruction->children[1]; // {}
//            instruction->children[1] = dead_code_remover_inline(code);
//        }
//        else if (instruction->value == "funcdef")
//        {
//            node *code = instruction->children[2];
//            instruction->children[2] = dead_code_remover_func(code);
//        }
//        else if (instruction->value == "classdef")
//        {
//            for (auto funcdef : instruction->children[1]->children)
//            {
//                node *code = funcdef->children[2];
//                funcdef->children[2] = dead_code_remover_func(code);
//            }
//        }
//    }
//    return n_ast;
//}
//
//void count_functions_apparition(node *ast, std::map<std::string, int> &f_appartion)
//{
//    if (is_funcall(ast->value))
//    {
//        if (function_count_exist(ast->value, f_appartion))
//        {
//            f_appartion[ast->value]++;
//        }
//        else
//        {
//            if (ast->value != "!=")
//                f_appartion[ast->value] = 1;
//        }
//        if (f_appartion[ast->value] == 1)
//        {
//            if (function_definition_exist(ast->value, functions_definition))
//            {
//                count_functions_apparition(functions_definition[ast->value], f_appartion);
//            }
//            else
//            {
//                if (!visitor_is_inbuild(ast->value)  && !is_attributed(ast->value))
//                {
//                    std::string warn = "la fonction '" + ast->value + "' est inconnue (pour le moment ?).\nContinuation de l'optimisation";
//                    warning(warn, ast->reference);
//                }
//            }
//        }
//    }
//    for (auto instruction : ast->children)
//    {
//        if (instruction->children.size() != 0)
//        {
//            for (auto c : instruction->children)
//            {
//                count_functions_apparition(c, f_appartion);
//            }
//        }
//        else if (is_funcall(instruction->value))
//        {
//            std::string name = instruction->value;
//            if (function_count_exist(name, f_appartion))
//            {
//                f_appartion[name]++;
//            }
//            else
//                f_appartion[name] = 1;
//            if (f_appartion[name] == 1)
//            {
//                if (function_definition_exist(name, functions_definition))
//                {
//                    count_functions_apparition(functions_definition[name], f_appartion);
//                }
//            }
//        }
//    }
//}
//
//void functions_definitions(node *&ast)
//{
//    std::vector<node *> c;
//    for (int i = 0; i < ast->children.size(); i++)
//    {
//        node *inst = ast->children[i];
//        if (inst->value == "funcdef")
//        {
//            std::string name = inst->children[0]->value;
//            node *trunc = inst;
//            functions_definition["!" + name] = trunc;
//        }
//        else
//            c.push_back(inst);
//    }
//    ast->children = c;
//}
//
//node *remove_unused_functions(node *ast)
//{
//    std::map<std::string, int> function_c;
//    count_functions_apparition(ast, function_c);
// 
//    node *n_ast = new node(ast->value);
//    for (auto i : function_c)
//    {
//        std::string name = std::get<0>(i);
//        if (function_definition_exist(name, functions_definition))
//        {
//            n_ast->push_child(functions_definition[name]);
//        }
//    }
//    for (auto other : functions_definition)
//    {
//        std::string name_bis = std::get<1>(other)->children[0]->value;
//        bool yes = false;
//        for (auto i : function_c)
//        {
//            std::string name = std::get<0>(i);
//            if (name == ("!" + name_bis))
//            {
//                yes = true;
//                break;
//            }
//        }
//        if (!yes)
//        {
//            if (!visitor_is_inbuild(name_bis) && !is_attributed(name_bis))
//                compiler_info("\t  -> fonction '" + name_bis + "' inutilisée");
//        }
//    }
//    return n_ast;
//}
//
//node *optimisator(node *ast, int optimisation_level)
//{
//    compiler_info("Optimisation du code : ");
//    compiler_info("\timportation des librairies");
//    includer(ast);
//    if (optimisation_level >= 1)
//    {
//        compiler_info("\tdestuction du code inutile");
//        ast = dead_code_remover_func(ast);
//    }
//    compiler_info("\tdénombrement des fonctions");
//    functions_definitions(ast);
//
//    compiler_info("\tdesctruction des fonctions inutilisées");
//    node *n_ast = remove_unused_functions(ast);
//
//    for (auto i : ast->children)
//    {
//        n_ast->push_child(i);
//    }
//    return n_ast;
//}
//
//void includer(node *&ast)
//{
//    node *r_ast = new node(ast->value);
//    r_ast->value = ast->value;
//    for (int index = 0; index < ast->children.size(); index++)
//    {
//        node *trunc = ast->children[index];
//
//        if (trunc->value == "inclue")
//        {
//            if (trunc->children.size() < 1)
//            {
//                std::string err = "le mot-clé 'inclue' doit avoir au moins un argument";
//                error(err, trunc->reference, 0);
//            }
//            node *arg = trunc->children[0];
//            w_variable *result = visitor_compute(arg, std::map<std::string, w_variable *>(), 0);
//            if (result->get_type() != "char")
//            {
//                std::string err = "le nom de fichier doit être de type 'char'";
//                error(err, trunc->reference, 0);
//            }
//
//            std::string filename = result->convert_to_char();
//            std::string dir = base_dir;
//
//            std::string filename2 = base_dir + filename;
//            base_dir += separate_base_dir(filename);
//            if (!is_included(filename2))
//            {
//                includes.push_back(filename2);
//                compiler_info("\t  -> importation de " + filename2);
//                std::string r = open_file(filename2.c_str());
//                if (r == "file_not_found")
//                {
//                    std::string err = "fichier introuvable '" + filename2 + "', continue la compilation sans importer le fichier, peut causer des problèmes";
//                    warning(err, trunc->children[0]->reference);
//                    continue;
//                }
//                std::vector<std::string> ref;
//                std::vector<std::string> lexemes = lexer(r, ref, filename2);
//
//                node *n_ast = parser(lexemes, filename2, ref, filename2 + "1:1");
//                includer(n_ast);
//                for (auto i : n_ast->children)
//                {
//                    r_ast->push_child(i);
//                }
//            }
//            base_dir = dir;
//        }
//        else 
//        {
//
//            r_ast->push_child(trunc);
//        }
//    }
//    ast->children = r_ast->children;
//}
//
//std::string compute_convert_to_string(node *trunc)
//{
//    std::string res = "";
//    if (trunc->value == "()")
//        res = "(";
//    if (trunc->value == "[]")
//        res = "[";
//    for (auto i : trunc->children)
//    {
//        if (i->value == "()")
//        {
//            res += compute_convert_to_string(i);
//        }
//        else if (i->value == "[]")
//        {
//            res += compute_convert_to_string(i);
//        }
//        else
//        {
//            res += i->value + " ";
//        }
//    }
//    if (trunc->value == "()")
//        res += ")";
//    if (trunc->value == "[]")
//        res += "]";
//    return res;
//}
//
//std::string prepare_arguments ( node * args )
//{   // prepars arguments for a funcall, as a string(node *)
//    // example : (arg1, arg2) --> (new node("()"))->push_child("arg1")->push_child(",")->push_child("arg2")
//    std::string full = "(new node(\"()\")";
//    for (auto i : args->children)
//    {
//        std::string val = get_value_for_compilation(i);
//        full += "->push_child(\" " + get_value_for_compilation(i) + ")";
//    }
//    return full;
//}
//
//std::string get_value_for_compilation(node *v)
//{
//    // return the value of the node argument as a string (nothing if the node has no children)
//    if (v->children.size() == 0)
//    {
//        return "";
//    }
//    else if (is_explicit(v->children[0]->value))
//    { // just like a char or an int
//        return "new w_variable(v->children[0]->value)";
//    }
//    else if (is_funcall(v->children[0]->value) || v->children.size() != 1 || v->children[1]->value == "()")
//    { // a funcall, we need to prepare the arguments
//        return "visitor_funcall( " + v->children[0]->value + ", " + prepare_arguments(v->children[1]) + ", std::map<std::string, w_variable *>(), 0)";
//    }
//    else if (is_funcall(v->children[0]->value))
//    { // it is not a funcall, just a function passed as a value
//        return v->children[0]->value;
//    }
//    else if (v->children.size() != 1 || v->children[1]->value == "()")
//    { // just a class call (object creation)
//        return "visitor_new_function( " + v->children[0]->value + ", " + prepare_arguments(v->children[1]) + ", std::map<std::string, w_variable *>(), 0)";
//    }
//    else if (v->children[0]->value == "()")
//    { // just parenthesis
//
//    }
//}
//
//std::string compile_value (std::string opera, std::string varname1, std::string varname2, std::string base_var_name, node * val1, node * val2, int tabs)
//{
//    std::string total;
//    total = "w_variable *a;  \nw_variable *var_1 = new w_variable(1);  \nw_variable *var_2 = new w_variable(2);  \na = visitor_funcall_methode(\"!\" + var_1 + \".plus\", (new node(\"()\"))->push_child((new node(\"*\"))->set_prevalue(  var_2  )), std::map<std::string, w_variable *>(), var_1, 0";
//}
//
//std::string compile_computation (node * ast, int tabs)
//{
//
//}
//
//std::string compile_vardef(node *ast, int tabs)
//{
//
//}
//
//std::string compiler_cpp(node *ast, int tabs)
//{
//
//}
//
//std::string compiler(node *ast, int tabs)
//{
//    // compiles in c++ the ast
//    paser_init_keywords();
//    std::string result;
//    std::string tabulations;
//    for (int i = 0; i < tabs; i++)
//    {
//        tabulations += "\t";
//    }
//    for (auto i : ast->children)
//    {
//        if (i->value == "vardef")
//        {
//            std::string name = i->children[0]->value;
//            std::string compute = compute_convert_to_string(i->children[1]);
//            std::string r = tabulations + name + " = " + compute + ";" + tabulations + "\n";
//            result += r;
//        }
//        else if (i->value == "funcdef")
//        {
//            std::string name = i->children[0]->value;
//            std::string args = compute_convert_to_string(i->children[1]);
//            std::string code = compiler(i->children[2], tabs + 1);
//            std::string r = tabulations + "fonction " + name + " " + args + " fait" + tabulations + "\n" + code + "" + tabulations + "\n" + tabulations + "fin" + tabulations + "\n";
//            result += r;
//        }
//        else if (i->value == "methdef")
//        {
//            std::string name = i->children[0]->value;
//            std::string args = compute_convert_to_string(i->children[1]);
//            std::string code = compiler(i->children[2], tabs + 1);
//            std::string r = tabulations + "methode " + name + " " + args + " fait" + tabulations + "\n" + code + "" + tabulations + "\n" + tabulations + "fin" + tabulations + "\n";
//            result += r;
//        }
//        else if (i->value == "classdef")
//        {
//            std::string name = i->children[0]->value;
//            std::string attribtues;
//            attribtues += compiler(i->children[1], tabs + 1);
//            std::string methods;
//            methods += compiler(i->children[2], tabs + 1);
//            std::string r = tabulations + "classe " + name + " contient" + "\n" + attribtues + methods + "" + tabulations + "\n" + tabulations + "fin" + tabulations + "\n";
//            result += r;
//        }
//        else if (i->value == "ifdec")
//        {
//            std::string condition = compute_convert_to_string(i->children[0]);
//            std::string code = compiler(i->children[1], tabs + 1);
//            std::string else_if;
//            for (auto e_i : i->children[2]->children)
//            {
//                std::string cond = compute_convert_to_string(e_i->children[0]);
//                std::string code = compiler(e_i->children[1], tabs + 1);
//                else_if += tabulations + "" + tabulations + "\nsinonsi " + cond + " alors" + tabulations + "\n " + code;
//            }
//            std::string e;
//            for (auto e_i : i->children[3]->children)
//            {
//                std::string e_code = compiler(e_i->children[0], tabs + 1);
//                e += tabulations + "" + tabulations + "\nsinon" + tabulations + "\n " + e_code;
//            }
//            std::string r = tabulations + "si " + condition + " alors " + tabulations + "\n" + code + else_if + e + "\n" + tabulations + "fin" + tabulations + "\n";
//            result += r;
//        }
//        else if (i->value == "forloop")
//        {
//            std::string varname = i->children[0]->value;
//            node *b1_c = new node("*");
//            b1_c->push_child(i->children[1]->children[0]);
//            std::string b1 = compute_convert_to_string(b1_c);
//            
//            node *b2_c = new node("*");
//            b2_c->push_child(i->children[1]->children[1]);
//            std::string b2 = compute_convert_to_string(b2_c);
//
//            std::string code = compiler(i->children[2], tabs + 1);
//            std::string r = tabulations + "pour " + varname + " allant de " + b1 + " a " + b2 + " fait " + tabulations + "\n" + code + "" + tabulations + "\n" + tabulations + "fin" + tabulations + "\n";
//            result += r;
//        }
//        else if (i->value == "whileloop")
//        {
//            std::string condition = compute_convert_to_string(i->children[0]);
//            std::string code = compiler(i->children[1], tabs + 1);
//            std::string r = tabulations + "tant que " + condition + " fait" + "\n " + code + tabulations + "fin\n";
//            result += r;
//        }
//        else if (parser_is_keyword(i->value))
//        {
//            std::string k_name = i->value;
//            std::string args;
//            if (i->children.size() != 0)
//            {
//                args = compute_convert_to_string(i->children[0]);
//            }
//            std::string r = tabulations + k_name + " " + args + ";" + tabulations + "\n";
//            result += r;
//        }
//        else if (i->value[0] == '!')
//        {
//            result += tabulations + i->value;
//        }
//        else
//        {
//            result += tabulations + compute_convert_to_string(i) + "\n";
//        }
//    }
//    return result;
//}