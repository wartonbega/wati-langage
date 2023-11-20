#include <iostream>
#include <map>
#include <string>
#include <stddef.h>
#include <math.h>

#include <chrono>
#include <ctime>

#include "../src/include/node.hpp"
#include "../src/include/variables.hpp"
#include "../src/include/trackers.hpp"
#include "../src/include/visitor.hpp"

#include "../src/include/lexer.hpp"
#include "../src/include/parser.hpp"
#include "../src/include/visitor.hpp"
#include "../src/include/inbuilds.hpp"

#include "../src/include/main.hpp"

void *allocate_mem(int64_t size)
{
    w_variable **tab = new w_variable *[size];
    return (void *)tab;
}

extern "C" w_variable *constructeur_tableau(std::vector<w_variable *> args, variable_table variables_t, std::string reference, int thread_id)
{
    w_variable *holder = new w_variable();
    holder->type = T_OBJECT;
    holder->content = allocate_mem(4);

    return holder;
}

w_variable **agrandis_tableau(w_variable **t, int64_t nouv_puissance)
{
    int64_t anc = pow(2, nouv_puissance - 1);
    int64_t nouv = pow(2, nouv_puissance);
    w_variable **nouv_var = new w_variable *[nouv];
    for (int i = 0; i < anc; i++)
    {
        nouv_var[i] = t[i];
    }
    delete[] t;
    return nouv_var;
}

w_variable **reduis_tableau(w_variable **t, int64_t nouv_puissance)
{
    int64_t anc = pow(2, nouv_puissance);
    int64_t nouv = pow(2, nouv_puissance - 1);
    w_variable **nouv_var = new w_variable *[nouv];
    for (int i = 0; i < nouv; i++)
    {
        nouv_var[i] = t[i];
    }
    return nouv_var;
}

extern "C" w_variable *tableau_plus(std::vector<w_variable *> args, variable_table variables_t, std::string reference, int thread_id)
{
    w_variable *self = args[0];
    w_variable *value = args[1];
    value = visitor_prepare_variable_for_copy(value, variables_t, thread_id);

    w_object *o = (w_object *)self->content;
    w_variable *holder = o->get_attribute("tab");
    w_variable **tab = (w_variable **)holder->content;

    int64_t puissance = *(int64_t *)(o->get_attribute("puissance")->content);
    int64_t taille = *(int64_t *)(o->get_attribute("taille")->content);
    int64_t max_taille = pow(2, puissance);

    if (taille >= max_taille)
    {
        tab = agrandis_tableau(tab, puissance + 1);
        holder->content = (void *)tab;
        o->attribute_attribution("puissance", new w_variable(puissance + 1));
    }

    tab[taille] = value;
    value->use++;
    return self;
}

extern "C" w_variable *tableau_index(std::vector<w_variable *> args, variable_table variables_t, std::string reference, int thread_id)
{
    w_variable *self = args[0];
    w_variable *index_var = args[1];
    int64_t index = index_var->convert_to_int();
    w_object *o = (w_object *)self->content;
    w_variable *holder = o->get_attribute("tab");

    int64_t taille = *(int64_t *)(o->get_attribute("taille")->content);
    if (index >= taille)
        error("L'index demandé est supérieur à la taille du tableau", reference, thread_id);

    w_variable **tab = (w_variable **)holder->content;
    w_variable *r = tab[index];
    return r;
}

extern "C" w_variable *tableau_modifie(std::vector<w_variable *> args, variable_table variables_t, std::string reference, int thread_id)
{
    w_variable *self = args[0];
    w_variable *index_var = args[1];
    int64_t index = index_var->convert_to_int();
    w_variable *value = args[2];

    w_object *o = (w_object *)self->content;
    w_variable *holder = o->get_attribute("tab");

    int64_t taille = *(int64_t *)(o->get_attribute("taille")->content);
    if (index >= taille)
        error("L'index demandé est supérieur à la taille du tableau", reference, thread_id);

    w_variable **tab = (w_variable **)holder->content;
    w_variable *r = tab[index];
    r->use--;
    r->check_garbage();

    value->use++;
    tab[index] = value;

    return self;
}

extern "C" w_variable *tableau_pop(std::vector<w_variable *> args, variable_table variables_t, std::string reference, int thread_id)
{
    w_variable *self = args[0];
    w_variable *index_var = args[1];
    int64_t index = index_var->convert_to_int();

    w_object *o = (w_object *)self->content;
    w_variable *holder = o->get_attribute("tab");

    int64_t taille = *(int64_t *)(o->get_attribute("taille")->content);
    int64_t puissance = *(int64_t *)(o->get_attribute("puissance")->content);
    if (index >= taille)
        error("L'index demandé est supérieur à la taille du tableau", reference, thread_id);

    w_variable **tab = (w_variable **)holder->content;
    w_variable *r = tab[index];

    for (int i = index; i < (taille); i++)
    {
        tab[i] = tab[i + 1];
    }

    if (taille < pow(2, puissance - 1))
    {
        tab = reduis_tableau(tab, puissance);
        holder->content = (void *)tab;
        o->attribute_attribution("puissance", new w_variable(puissance - 1));
    }
    r->use--;
    return r;
}

extern "C" w_variable *tab_comp_equal(std::vector<w_variable *> args, variable_table variables_t, std::string reference, int thread_id)
{
    if (args.size() != 2 || args[0]->get_type() != "tableau" || args[1]->get_type() != "tableau")
        error("!tab_comp_equal neccessite deux arguments de type 'tableau'", reference, thread_id);

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
    w_function *f = functions["!comparaison_iterative_tableau"];
    return visitor_visit(f->trunc, new_table, thread_id);
}