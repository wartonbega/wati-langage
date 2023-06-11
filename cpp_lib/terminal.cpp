#include <vector>
#include <stdio.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <termios.h>

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
    printf("\033[%d;%dH", int(row), int(col)); // Places it
    return basic_return_terminal();
}


int get_pos(int *y, int *x) {

    char buf[30]={0};
    int ret, i, pow;
    char ch;

    *y = 0; *x = 0;

    struct termios term, restore;

    tcgetattr(0, &term);
    tcgetattr(0, &restore);
    term.c_lflag &= ~(ICANON|ECHO);
    tcsetattr(0, TCSANOW, &term);

    write(1, "\033[6n", 4);

    for( i = 0, ch = 0; ch != 'R'; i++ )
    {
        ret = read(0, &ch, 1);
        if ( !ret ) {
        tcsetattr(0, TCSANOW, &restore);
        fprintf(stderr, "getpos: error reading response!\n");
        return 1;
        }
        buf[i] = ch;
    }

    if (i < 2) {
        tcsetattr(0, TCSANOW, &restore);
        return(1);
    }

    for( i -= 2, pow = 1; buf[i] != ';'; i--, pow *= 10)
        *x = *x + ( buf[i] - '0' ) * pow;

    for( i-- , pow = 1; buf[i] != '['; i--, pow *= 10)
        *y = *y + ( buf[i] - '0' ) * pow;

    tcsetattr(0, TCSANOW, &restore);
    return 0;
}

extern "C" w_variable *terminal_curseur_pos(std::vector<w_variable *> args, variable_table variables_t, std::string reference, int thread_id)
{
    int x, y;
    get_pos(&x, &y);
    w_variable *row = new w_variable(x);
    w_variable *col = new w_variable(y);
    std::vector<int64_t> t;
    t.push_back(x);
    t.push_back(y);
    w_variable *r = create_list(t, variables_t, thread_id);
    return r;
}