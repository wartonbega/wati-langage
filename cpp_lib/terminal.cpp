#include <vector>
#include <stdio.h>
#include <sys/ioctl.h>
#include <unistd.h>

#include "../src/include/node.hpp"
#include "../src/include/variables.hpp"
#include "../src/include/visitor.hpp"
#include "./lists.hpp"

w_variable *basic_return_terminal()
{
    w_variable *t = new w_variable(0);
    return t;
}

extern "C" w_variable *terminal_taille(std::vector<w_variable *> args, variable_table variables_t, std::string reference, int thread_id)
{
    // Returns a list containing the number of columns and rows in the current terminal
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

    int64_t row = int64_t(w.ws_row);
    int64_t col = int64_t(w.ws_col);

    std::vector<int64_t> s;
    s.push_back(row);
    s.push_back(col);
    w_variable *r = create_list(s, variables_t, thread_id);
    return r;
}

extern "C" w_variable *terminal_curseur(std::vector<w_variable *> args, variable_table variables_t, std::string reference, int thread_id)
{
    // Places the cursor in the terminal in specific coordinates (x, y)
    if (args.size() != 2 || args[0]->get_type() != "int" || args[1]->get_type() != "int")
    {
        error("!terminal_curseur : doit avoir deux arguments de type 'int'", reference, thread_id);
    }

    int64_t row = *(int64_t *)(args[0]->content);
    int64_t col = *(int64_t *)(args[1]->content);
    printf("\033[%d;%dH", int(row), int(col));
    return basic_return_terminal();
}
