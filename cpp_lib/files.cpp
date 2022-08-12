#include <vector>
#include <iostream>
#include <map>
#include <string>

#include "../src/include/node.hpp"
#include "../src/include/variables.hpp"
#include "../src/include/visitor.hpp"
#include "../src/include/main.hpp"

extern "C" w_variable *ouvre(std::vector<w_variable *> args, std::map<std::string, w_variable *> variables_t, std::string reference, int thread_id)
{
    if (args.size() != 1 || args[0]->get_type() != "char")
    {
        error("!ouvre : doit avoir un argument de type 'char' : 'nom_de_fichier'", reference, thread_id);
    }

    std::string filename = *(std::string *)(args[0]->content);

    if (filename == "file_not_found")
    {
        std::string err = "le fichier '" + filename + "' n'as pas put être ouvert (est introuvable).";
        error(err, reference, thread_id);
    }

    w_variable *r = new w_variable(open_file(filename.c_str()));

    return r;
}

extern "C" w_variable *ecrit(std::vector<w_variable *> args, std::map<std::string, w_variable *> variables_t, std::string reference, int thread_id)
{
    if (args.size() != 2 || args[0]->get_type() != "char" || args[1]->get_type() != "char")
    {
        error("!ecrit : doit avoir deux arguments de type 'char' : 'nom_de_fichier' et 'contenu'", reference, thread_id);
    }

    std::string filename = *(std::string *)(args[0]->content);
    std::string file_content = *(std::string *)(args[1]->content);

    write_file(filename, file_content);

    return new w_variable(0);
}