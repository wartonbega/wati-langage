#include "./include/trackers.hpp"
#include "./include/visitor.hpp"
#include "./include/types.hpp"
#include "./include/inbuilds.hpp"


tracker::tracker(std::string expr, variable_table *var_t)
{
    if (is_attributed(expr))
    {
        std::string first_var_n;
        int index = 0;
        while (expr[index] != '.')
        {
            first_var_n += expr[index];
            index++;
        }
        w_variable *first_var;
        if (first_var_n == "")
        {
            std::string err = "pas de varibale de départ indiquée dans '" + expr + "'";
            error(err,  what_reference(0)->top(), 0);
        }
        else
        {
            if (!var_t->exist(first_var_n))
            {
                std::string err = "la variable '" + first_var_n + "' n'existe pas";
                error(err, what_reference(0)->top(), 0);
            }
            first_var = var_t->get(first_var_n);
        }   
        if (!first_var->is_object())
        {
            std::string err = "la variable '" + first_var_n + "' (" + *(std::string *)(w_char(first_var, var_t->vars, 0)->content) + ") doit être un objet";
            error(err, what_reference(0)->top(), 0);
        }

        w_object *last_o;
        if (first_var->content != NULL)
            last_o = (w_object *)first_var->content;
        else
        {
            std::string err = "tentative d'accès à une partie de la mémoire interdite";
            error(err, what_reference(0)->top(), 0);
        }

        std::string patent;
        for (int y = index; y < expr.size(); y++)
        {
            if (expr[y] == '.')
            {
                if (!patent.empty())
                {
                    if (!last_o->attribute_exist(patent))
                    {
                        std::string err = "'" + last_o->name + " n'as pas d'attribut '" + patent + "'";
                        error(err, what_reference(0)->top(), 0);
                    }
                    w_variable *last_var = last_o->get_attribute(patent);
                    if (!last_var->is_object())
                    {
                        std::string err = "la variable '" + last_o->name + "." + patent + " doit être un objet";
                        error(err, what_reference(0)->top(), 0);
                    }
                    if (last_var->content != NULL)
                        last_o = (w_object *)last_var->content;
                    else
                    {
                        std::string err = "tentative d'accès à une partie de la mémoire interdite";
                        error(err, what_reference(0)->top(), 0);
                    }
                    patent.clear();
                }
            }
            else
                patent += expr[y];
        }
        // patent is therefore the name of the variable we need to track
        // and the variable table is in last_o
        expr = patent;
        var_t = last_o->attributes;
    }

    if (!var_t->exist(expr))
    {
        std::string err = "la variable '" + expr + "' n'existe pas. On ne peut donc pas la traquer";
        error(err, what_reference(0)->top(), 0);
    }

    this->name_to_track = expr;
    this->variable_t = var_t;
}

w_variable *tracker::value(int thread_id)
{
    // we check if the variable still exists
    if (!this->variable_t->exist(this->name_to_track))
    {
        std::string err = "le traqueur visant '" + this->name_to_track + "' n'as plus rien à traquer. La variable '" + this->name_to_track + "' n'est plus accessible";
        error(err, what_reference(thread_id)->top(), thread_id);
    }

    // finally we can return the right value
    return this->variable_t->get(this->name_to_track);
}