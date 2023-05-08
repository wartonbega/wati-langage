#include "lists.hpp"

w_variable *create_list(std::vector<int64_t>L, variable_table variables_t, int thread_id)
{
    // Creates a wati-lists of ints
    // Uses an already existing list in cpp and 'translates' it in thr wati-lang
    w_variable *list = visitor_new_object("list", new node("*"), variables_t, thread_id);
    for (auto elmts : L) 
    {
        int64_t *e = new int64_t(elmts);
        w_variable *arg = new w_variable();
        arg->type = T_INT;
        arg->content = (void *)e;
        variable_table variables_t_bis = variable_table(variables_t);
        variables_t_bis.assign("self", list, thread_id);
        variables_t_bis.assign("content", arg, thread_id);

        w_function *plus = functions["!list.plus"];
        visitor_visit(plus->trunc, variables_t_bis, thread_id);
    }
    return list;
}

w_variable *create_list(std::vector<std::string>L, variable_table variables_t, int thread_id)
{
    // Creates a wati-lists of strings
    // Uses an already existing list in cpp and 'translates' it in thr wati-lang
    w_variable *list = visitor_new_object("list", new node("*"), variables_t, thread_id);
    for (auto elmts : L) 
    {
        std::string *e = new std::string(elmts);
        w_variable *arg = new w_variable();
        arg->type = T_CHAR;
        arg->content = (void *)e;
        variable_table variables_t_bis = variable_table(variables_t);
        variables_t_bis.assign("self", list, thread_id);
        variables_t_bis.assign("content", arg, thread_id);

        w_function *plus = functions["!list.plus"];
        visitor_visit(plus->trunc, variables_t_bis, thread_id);
    }
    return list;
}