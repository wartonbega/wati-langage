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

w_variable *node_next(w_variable *l)
{
    return ((w_object *)l->content)->get_attribute("next");
}

//int64_t iterative_comparison(w_object *o_l1, w_object *o_l2)
//{
//    w_variable *h1 = o_l1->get_attribute("head"); // le début
//    w_variable *t1 = o_l1->get_attribute("tail"); // la fin
//
//    w_variable *h2 = o_l2->get_attribute("head"); // le début
//    w_variable *t2 = o_l2->get_attribute("tail"); // la fin
//
//    while (node_next(h1) != t1 && node_next(h2) != t2)
//    {
//        h1 = node_next(h1);
//        h2 = node_next(h2);
//    }
//}

extern "C" w_variable *list_comp_equal(std::vector<w_variable *> args, variable_table variables_t, std::string reference, int thread_id)
{
    if (args.size() != 2 || args[0]->get_type() != "list" || args[1]->get_type() != "list")
        error("!list_comp_equal neccessite deux arguments de type 'list'", reference, thread_id);
 
    w_variable *list1 = args[0];
    w_object *o_list1 = (w_object *)list1->content;
    w_variable *list2 = args[1];
    w_object *o_list2 = (w_object *)list2->content;
    
    // On commence par checker si les deux listes ont la même addresse
    if (list1 == list2)
        return new w_variable(1);
    
    // Puis si elles ont (ou pas) la même taille
    int64_t t_1 = *(int64_t *)(o_list1->get_attribute("taille"));
    int64_t t_2 = *(int64_t *)(o_list2->get_attribute("taille"));
    if (t_1 != t_2)
        return new w_variable(0);
    

    variable_table new_table = variable_table();
    new_table.assign("self", list1, thread_id);
    new_table.assign("x", list2, thread_id);
    w_function *f = functions["!comparaison_iterative"];
    return visitor_visit(f->trunc, new_table, thread_id);
}