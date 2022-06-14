#include <stdio.h>  

#include "include/main.hpp"
#include "include/lexer.hpp"
#include "include/parser.hpp"
#include "include/visitor.hpp"


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
    return "";
}

int main(int argc, char *argv[])
{
	system("stty -icanon");
	std::string r = open_file("test.wati");
	std::vector<std::string> lexemes = lexer(r);
	node *ast = parser(lexemes, "main");
	//std::cout << ast->to_string(0) << std::endl;
	visitor_visit(ast, std::__1::map<std::__1::string, w_variable *>());
    return 0;
}