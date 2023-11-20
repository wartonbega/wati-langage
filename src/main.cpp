#include <stdio.h>
#include "include/main.hpp"
#include "include/lexer.hpp"
#include "include/parser.hpp"
#include "include/visitor.hpp"
#include "include/compiler.hpp"
#include "include/variables.hpp"

extern variable_table *GLOBAL_VARIABLE;

std::string base_dir;
std::string main_filename; 
std::string list_creation_class_name;
bool show_tree;
bool short_error;

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

    complete_error += err_s + TERMINAL_RESET;
	
	if (!short_error)
		complete_error += "\n" + comp_ref + TERMINAL_RESET;

    return complete_error;
}

void init_vars(variable_table &variables_t)
{
	std::string *c;
	w_variable *var;

	var = new w_variable(TERMINAL_BLACK);
	variables_t.assign("terminal_black", var, 0);
	
	var = new w_variable(TERMINAL_ITALIC);
	variables_t.assign("terminal_italic", var, 0);

	var = new w_variable(TERMINAL_RED);
	variables_t.assign("terminal_red", var, 0);

	var = new w_variable(TERMINAL_GREEN);
	variables_t.assign("terminal_green", var, 0);

	var = new w_variable(TERMINAL_YELLOW);
	variables_t.assign("terminal_yellow", var, 0);
	
	var = new w_variable(TERMINAL_BLUE);
	variables_t.assign("terminal_blue", var, 0);

	var = new w_variable(TERMINAL_MAGENTA);
	variables_t.assign("terminal_magenta", var, 0);

	var = new w_variable(TERMINAL_CYAN);
	variables_t.assign("terminal_cyan", var, 0);

	var = new w_variable(TERMINAL_WHITE);
	variables_t.assign("terminal_white", var, 0);

	var = new w_variable(TERMINAL_RESET);
	variables_t.assign("terminal_reset", var, 0);

	var = new w_variable(TERMINAL_BOLD);
	variables_t.assign("terminal_bold", var, 0);
	
	var = new w_variable(TERMINAL_UNDERLINE);
	variables_t.assign("terminal_underline", var, 0);

	var = new w_variable(TERMINAL_REVERSE);
	variables_t.assign("terminal_reverse", var, 0);

	var = new w_variable(TERMINAL_BLINK);
	variables_t.assign("terminal_blink", var, 0);

	list_creation_class_name = "tableau";
}


int main(int argc, char *argv[])
{
	system("stty -icanon");
	system("export PATH=/usr/local/lib/wati1.2/:$PATH");
	std::string filename;
	if (argc < 2) // the command wich was typed is just 'wati'
	{
		filename += "/usr/local/lib/wati1.2/lib/console.wati"; // therefore we lunch the standard console
	}
	else 
	{
		filename += argv[1];
	}

	bool compile = 0;
	bool show_ast = 0;
	bool include_std = 1;
	short_error = 1;
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
		else if (std::string(argv[i]) == "--no-std" || std::string(argv[i]) == "-s")
		{
			include_std = 0;
		}
		else if (std::string(argv[i]) == "--short-error" || std::string(argv[i]) == "-S")
			short_error = false;
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
	if (include_std)
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
	init_vars(variables_t);

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