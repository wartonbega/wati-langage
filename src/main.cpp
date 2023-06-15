#include <stdio.h>
#include "include/main.hpp"
#include "include/lexer.hpp"
#include "include/parser.hpp"
#include "include/visitor.hpp"
#include "include/compiler.hpp"

std::string base_dir;
std::string main_filename; 
bool show_tree;

void write_file(std::string name, std::string content)
{
	std::string command = "touch " + name;
	system(command.c_str());
	std::ofstream fichier;
	fichier.open(name);
	fichier << content;
	fichier.close();
}

std::string open_file(const char *name)
{
	std::ifstream fichier(name);
	if (fichier)
	{
		char lettre;
		std::string file;
		while (fichier.get(lettre))
		{
			file += lettre;
		}

		return file;
	}
	return "file_not_found";
}

std::string separate_base_dir(std::string expr)
{
	std::string patent;
	std::string total;
	for (auto i : expr)
	{
		if (i == '/')
		{
			total += patent + "/";
			patent = "";
		}
		else
			patent += i;
	}
	return total;
}

std::string throw_error(w_variable *err_)
{
	w_object *f = (w_object*)err_->content;
	std::string err = f->get_attribute("erreur")->convert_to_char();
	std::string ref = f->get_attribute("reference")->convert_to_char();
	std::string comp_ref = f->get_attribute("appel")->convert_to_char();

    std::string complete_error = "\n";
    std::string err_s = TERMINAL_BOLDRED + ref + TERMINAL_BOLDBLACK + " erreur : " + TERMINAL_BOLDRED + err;

    complete_error += err_s + "\n";
	complete_error += comp_ref;
    return complete_error;
}

void init_vars(std::map<std::string, w_variable *> &variables_t)
{
	std::string *c;
	w_variable *var;

	var = new w_variable();
	c = new std::string(TERMINAL_BLACK);
	var->type = 1; // char
	var->content = (void *)c;
	variables_t["terminal_black"] = var;

	var = new w_variable();
	c = new std::string(TERMINAL_RED);
	var->type = 1; // char
	var->content = (void *)c;
	variables_t["terminal_red"] = var;

	var = new w_variable();
	c = new std::string(TERMINAL_GREEN);
	var->type = 1; // char
	var->content = (void *)c;
	variables_t["terminal_green"] = var;

	var = new w_variable();
	c = new std::string(TERMINAL_YELLOW);
	var->type = 1; // char
	var->content = (void *)c;
	variables_t["terminal_yellow"] = var;
	
	var = new w_variable();
	c = new std::string(TERMINAL_BLUE);
	var->type = 1; // char
	var->content = (void *)c;
	variables_t["terminal_blue"] = var;

	var = new w_variable();
	c = new std::string(TERMINAL_MAGENTA);
	var->type = 1; // char
	var->content = (void *)c;
	variables_t["terminal_magenta"] = var;

	var = new w_variable();
	c = new std::string(TERMINAL_CYAN);
	var->type = 1; // char
	var->content = (void *)c;
	variables_t["terminal_cyan"] = var;

	var = new w_variable();
	c = new std::string(TERMINAL_WHITE);
	var->type = 1; // char
	var->content = (void *)c;
	variables_t["terminal_white"] = var;

	var = new w_variable();
	c = new std::string(TERMINAL_RESET);
	var->type = 1; // char
	var->content = (void *)c;
	variables_t["terminal_reset"] = var;

	var = new w_variable();
	c = new std::string(TERMINAL_BOLD);
	var->type = 1; // char
	var->content = (void *)c;
	variables_t["terminal_bold"] = var;
	
	var = new w_variable();
	c = new std::string(TERMINAL_UNDERLINE);
	var->type = 1; // char
	var->content = (void *)c;
	variables_t["terminal_underline"] = var;

	var = new w_variable();
	c = new std::string(TERMINAL_REVERSE);
	var->type = 1; // char
	var->content = (void *)c;
	variables_t["terminal_reverse"] = var;

	var = new w_variable();
	c = new std::string(TERMINAL_BLINK);
	var->type = 1; // char
	var->content = (void *)c;
	variables_t["terminal_blink"] = var;
}


int main(int argc, char *argv[])
{
	system("stty -icanon");
	system("export PATH=/usr/local/lib/wati/:$PATH");
	std::string filename;
	if (argc < 2) // the command wich was typed is just 'wati'
	{
		filename += "/usr/local/lib/wati/lib/console.wati"; // therefore we lunch the standard console
	}
	else 
	{
		filename += argv[1];
	}

	bool compile = 0;
	bool show_ast = 0;
	std::string output_name = "output.wati";
	for (int i = 2; i < argc; ++i)
	{
		if (std::string(argv[i]) == "--compile" || std::string(argv[i]) == "-c")
		{
			compile = 1;
			if (i + 1 < argc)
			{
				output_name = std::string(argv[i + 1]);
			}
			else 
			{
				std::string err = "en cas de compilation, le nom du fichier de sortie doit être indiqué. Example : `wati fichier_source.wati -c sortie.wati`";
			}
		}
		else if (std::string(argv[i]) == "--tree" || std::string(argv[i]) == "-t")
		{
			show_ast = 1;
			show_tree = true;
		}
	}
	

	base_dir = separate_base_dir(filename);
	std::string r = open_file(filename.c_str());
	if (r == "file_not_found") 
	{
		warning("Fichier non-trouvé : '" + filename + "'", "");
		exit(2);
	}
	// We absolutly need to include the standard library in ordre for the error
	// system to work.
	main_filename = filename;
	r = "inclue \"std.wati\";\n" + r;


	std::vector<std::string> ref;
	std::vector<std::string> lexemes = lexer(r, ref, filename);

	node *ast = parser(lexemes, "main", ref, filename + "1:1");

	if (show_ast)
	{
		std::cout << ast->to_string(0) << std::endl;
	}

	references.push_back(new std::stack<std::string>);
	
	variable_table variables_t = variable_table();
	init_vars(variables_t.vars);
	try
	{
		visitor_visit(ast, variables_t, 0);
	}
	catch (w_variable *error)
	{
		std::cout << throw_error(error) << std::endl;
		exit(1);
	}
	
	delete ast;
	return 0;
}