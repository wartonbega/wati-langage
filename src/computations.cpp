#include <iostream>
#include <stack>
#include <dlfcn.h>

#include "include/computations.hpp"
#include "include/visitor.hpp"
#include "include/variables.hpp"
#include "include/types.hpp"
#include "include/inbuilds.hpp"
#include "include/main.hpp"
#include "include/lexer.hpp"
#include "include/parser.hpp"
#include "include/trackers.hpp"
#include "include/trackers.hpp"

extern variable_table *GLOBAL_VARIABLE;

bool is_binary_operator(node *op)
{
    if (op->children.size() == 2 && (op->value == "^" ||
                                     op->value == "==" ||
                                     op->value == "!=" ||
                                     op->value == "<=" ||
                                     op->value == ">=" ||
                                     op->value == "*" ||
                                     op->value == "-" ||
                                     op->value == "+" ||
                                     op->value == "%" ||
                                     op->value == "/" ||
                                     op->value == "<" ||
                                     op->value == ">"))
        return true;
    return false;
}

bool is_logic_operator(node *op)
{
    if (op->children.size() == 2 && (op->value == "&&" ||
                                     op->value == "||"))
        return true;
    return false;
}

bool is_unary_operator(node *op)
{
    if (
        op->children.size() == 1 &&
        (op->value == "&" ||
         op->value == "*" ||
         op->value == "-"))
        return true;
    return false;
}

void check_value_integrity(w_variable *value, std::string ref, int thread_id)
{
    if (value == nullptr)
        error("La valeur n'existe pas !", ref, thread_id);

    if (value->use < 0)
        error("La valeur pour cette variable est inutilisable (non référencée)", ref, thread_id);

    if (value->get_type() == "traqueur")
    {
        tracker *t = (tracker *)value->content;
        if (!t->usable)
            error("Le traqueur n'est pas utilisable, il provient d'un endroit du code qui n'existe plus", ref, thread_id);
    }
}

w_variable *get_name_for_vardef(node *n, variable_table *&variables_t, std::string &name, w_variable *&self, int thread_id)
{ // Renvoie le nom final de la variable et la table de variables necessaire à l'assignation du nom

    if (is_unary_operator(n) and n->value == "*")
    {
        w_variable *index = nullptr;
        get_tracker_val(n, variables_t, thread_id, name, variables_t, self, index);
        return index;
    }
    std::string p_name; // Le nom potentiel pour la variable
    w_variable *last = nullptr;
    for (auto e : n->children)
    {
        if (e->value[0] == '.')
        { // On évalue donc la défintion d'un attribut.

            // On commence par faire les vérifications habituelles, c'est a dire si il y a ou non existence
            // d'un objet où il pourrais y avoir des attributs
            if (last == nullptr)
                error("L'attribut demandé n'as pas de variable de départ", e->reference, thread_id);

            if (!last->is_object())
                error("La dernière valeur doit être un objet", e->reference, thread_id);

            p_name = remove_prefix(e->value); // On fait disparaître le '.'

            w_object *obj = (w_object *)last->content;
            variables_t = obj->attributes; // De toute façon la dernière table de variable doît être celle-ci

            if (variables_t->exist(p_name))
                last = variables_t->get(p_name);
            else
                last = nullptr;
        }
        else if (e->value == "[]")
        {

            // Ici, on vas tricher, le 'nom de variable' qui sera assigné sera en realité
            // Le nom de la fonction !type.index
            // Et donc la table de variable sera les attributs de la dernière variable
            w_variable *b_value = visitor_compute(e->children[0], variables_t, thread_id);

            std::string f_name = "!" + last->get_type() + ".modifie";

            self = last;
            if (!function_exist(f_name, functions))
                error("La fonction '" + f_name + "' n'existe pas", e->reference, thread_id);

            if (last->is_object())
            { // Ici, on vas effectivement faire que la table de variable représente les attributs de l'objet
                variables_t = ((w_object *)last->content)->attributes;
            }
            // Mais si ce n'est pas un objet, alors c'est un char ou qq chose d'autre (comme un char)
            // Et dans ce cas, on ne change pas la table de variables

            // Finalement, la modification d'un élément conclue la défintion de la variable, et tout ce qui suit est ignoré
            if (e != n->children[n->children.size() - 1]) // On lance un joli warning
                warning("Ce qui suit la modification d'un élément est ignoré dans la défintion du nom de la variable", e->reference);

            name = f_name;
            return b_value;
        }
        else
        { // On a une variable 'de base'
            p_name = e->value;
            if (variables_t->exist(p_name))
                last = variables_t->get(p_name);
        }
    }
    // Dernière vérifications :
    name = p_name;
    if (name.empty())
        error("Le nom de variable est vide (?)", n->reference, thread_id);
    return nullptr;
}

w_variable *create_list(node *l, variable_table *variables_t, w_variable *last, int thread_id)
{
    w_variable *list = visitor_new_object(list_creation_class_name, new node(COMP_KEYWORD), *variables_t, thread_id);
    for (auto e : l->children)
    {
        w_variable *arg = visitor_compute_recursive(e, variables_t, last, thread_id);
        variable_table variables_t_bis = variable_table();
        w_function *plus = functions["!" + list_creation_class_name + ".plus"];

        variables_t_bis.assign("self", list, thread_id);
        variables_t_bis.assign(plus->get_arg_name(0), arg, thread_id);

        w_variable *r = visitor_visit(plus->trunc, variables_t_bis, thread_id);
        r->use--;
    }
    return list;
}

w_variable *call_object(std::string name, node *arg, variable_table *varibales_t, int thread_id)
{ // Name doit être un nom de classe valable
    return visitor_new_object(name, arg, *varibales_t, thread_id);
}

w_variable *treate_multiple_values(node *c, w_variable *last, variable_table *varibales_t, int thread_id, w_variable *&self, std::string &last_name, bool no_error)
{
    // Permet de traiter une suite de noms et potentiellement une parenthèse à la fin
    if (c->children.size() == 0)
    {
        error("Ne peut rien appeler. Il doit y avoir un nom à la fonction", c->reference, thread_id);
    }
    self = nullptr;
    std::string potential_function; // Si on a juste le nom de la fonction

    last = visitor_compute_recursive(c->children[0], varibales_t, last, thread_id, true, potential_function, last_name);
    for (int i = 1; i < c->children.size(); i++)
    {
        node *e = c->children[i];

        if (e->value == "()" and c->value == "!")
        { // On appel dans ce cas la fonction
            if (last->get_type() != "fonction" && potential_function.empty())
            {
                error("La fonction demandée n'existe pas.", e->reference, thread_id);
            }

            std::string f_name;

            if (last->get_type() == "fonction") // Ici, la fonction était stocké dans une variable
                f_name = last->convert_to_char();

            if (!potential_function.empty()) // Dans ce cas on possède le nom de la fonction
                f_name = potential_function;

            variable_table empty_table = variable_table();
            // De toute façon, on appel une fonction
            if (self == nullptr)
            { // Alors c'est un appel de fonction "simple"
                w_variable *r = visitor_funcall(f_name, e, empty_table, varibales_t, thread_id);
                return r;
            }
            // Sinon c'est que c'est une méthode
            return visitor_funcall_methode(f_name, e, empty_table, varibales_t, self, thread_id);
        }

        if (e->value == "()") // On appel donc à la création d'un nouveau objet
        {
            std::string class_name = c->children[i - 1]->value;
            if (!class_exist(class_name, classes))
                error("Nom de classe inconnu : '" + class_name + "'", e->reference, thread_id);

            w_variable *r = call_object(class_name, e, varibales_t, thread_id);
            return r;
        }

        self = last;
        last = visitor_compute_recursive(e, varibales_t, last, thread_id, true, potential_function, last_name);
    }
    if (!potential_function.empty() and c->value == "!")
    { // Dans ce cas, on a trouvé le nom d'une fonction valable
        if (!function_exist(potential_function, functions))
            error("La fonction '" + potential_function + "' n'existe pas", c->reference, thread_id);
        w_variable *v = new w_variable(potential_function);
        v->type = T_FONCTION;
        return v;
    }
    return last;
}

w_variable *get_tracker_val(node *c, variable_table *variables_t, int thread_id, std::string &ref_name, variable_table *&ref_vart, w_variable *&ref_list, w_variable *&ref_index)
{ // Ici, on récupère la valeur stockée dans le traqueur
    w_variable *content = visitor_compute(c->children[0], variables_t, thread_id);
    if (content->get_type() != "traqueur")
        error("Ne peut pas considérer le type '" + content->get_type() + "' comme un traqueur", c->reference, thread_id);

    tracker *t = (tracker *)content->content;
    if (t->iterable)
    { // On renvoie l'index
        ref_name = "!" + t->list->get_type() + ".modifie";
        ref_list = t->list;
        ref_index = t->index;
    }
    else
    {
        ref_name = t->name_to_track;
        ref_vart = t->variable_t;
    }
    return t->value(thread_id);
}

w_variable *use_tracker_def(node *c, variable_table *variables_t, int thread_id)
{ // 'c' est un node avec la définition d'un traqueur
    // c = node("&")
    // On doit donc vérifier plusieurs chose : l'unicité du nom de la variable
    // mais aussi son existence
    if (c->children.size() == 0)
        error("Pas de valeurs pour la définition du traqueur", c->reference, thread_id);

    if (c->children[0]->value != COMP_KEYWORD)
        error("La valeur doit être un nom de variable pour la défintion du traqueur", c->reference, thread_id);

    if (c->children[0]->children.size() == 0)
        error("Il doit y avoir un nom de variable à la définition du traqueur", c->reference, thread_id);

    variable_table *v = variables_t;
    std::string name;
    w_variable *self;

    // On vas récupérer le nom complet de la variable, ce qui peut permettre certaines fantaisies
    // dont j'espère, l'utilisateur se passera...
    w_variable *potential_index = get_name_for_vardef(c->children[0], v, name, self, thread_id);

    w_variable *track = new w_variable();
    track->type = T_TRACKER;
    tracker *t;

    if (name[0] == '!')
    {
        t = new tracker(self, potential_index);
    }
    else
    {
        t = new tracker(name, v);
    }
    track->content = (void *)t;
    return track;
}

w_variable *visitor_compute_recursive(node *c, variable_table *variables_t, w_variable *last, int thread_id)
{ // Juste pour faire le pont, à la base, on prend automatiquement les erreurs, d'où le false
    std::string onsenfout;
    return visitor_compute_recursive(c, variables_t, last, thread_id, false, onsenfout, onsenfout);
}

w_variable *visitor_link_unary_operator(node *op, variable_table *variables_t, int thread_id)
{
    if (op->value == "&") // On crée un traqueur
        return use_tracker_def(op, variables_t, thread_id);

    if (op->value == "*")
    { // On vas chercher la valeur du traqueur
        std::string n;
        w_variable *onsenfout;
        return get_tracker_val(op, variables_t, thread_id, n, variables_t, onsenfout, onsenfout);
    }
    if (op->value == "-")
    {
        w_variable *pos = visitor_compute_recursive(op->children[0], variables_t, nullptr, thread_id);
        if (pos->get_type() != "int")
            error("La négation '-' n'est possible que pour une variable de type 'int'", op->reference, thread_id);
        int64_t *neg = new int64_t(-*(int64_t *)pos->content);
        pos->content = (void *)neg; // On inverse juste l'entier qui est là
        return pos;
    }
    error("Interne : operateur unitaire '" + op->value + "' non définis", op->reference, thread_id);
    return nullptr;
}

w_variable *visitor_link_logic_operator(node *op, variable_table *variables_t, int thread_id)
{
    // Vus que op a verifié le test de logic operator,
    // alors, il a deux arguments (et uniquement deux)
    if (op->value == "&&") // C'est une porte logique 'et'
    {
        // il faut donc que les deux conditions soient satisfaitent
        w_variable *a = visitor_compute(op->children[0], variables_t, thread_id);
        if (a->get_type() != "int")
            error("L'opérateur && necessite des valeurs de type 'int', pas '" + a->get_type() + "'", op->reference, thread_id);
        if (a->convert_to_int() == 0)
        {
            a->check_garbage();
            return new w_variable(0);
        }

        w_variable *b = visitor_compute(op->children[1], variables_t, thread_id);
        if (b->get_type() != "int")
            error("L'opérateur && necessite des valeurs de type 'int', pas '" + b->get_type() + "'", op->reference, thread_id);

        if (b->convert_to_int() == 0)
        {
            b->check_garbage();
            return new w_variable(0);
        }

        a->check_garbage();
        b->check_garbage();
        return new w_variable(1);
    }
    else if (op->value == "||") // Porte logique 'ou'
    {
        // il faut donc que une des deux conditions soient satisfaite
        w_variable *a = visitor_compute(op->children[0], variables_t, thread_id);

        if (a->get_type() != "int")
            error("L'opérateur || necessite des valeurs de type 'int', pas '" + a->get_type() + "'", op->reference, thread_id);

        if (a->convert_to_int() != 0)
        {
            a->check_garbage();
            return new w_variable(1);
        }

        w_variable *b = visitor_compute(op->children[1], variables_t, thread_id);

        if (b->get_type() != "int")
            error("L'opérateur || necessite des valeurs de type 'int', pas '" + b->get_type() + "'", op->reference, thread_id);

        if (b->convert_to_int() != 0)
        {
            a->check_garbage();
            return new w_variable(1);
        }
        a->check_garbage();
        b->check_garbage();
        return new w_variable(0);
    }
    else
        error("Interne : l'opérateur logique '" + op->value + "' n'a pas été définis", op->reference, thread_id);
    return nullptr;
}

w_variable *visitor_compute_recursive(node *c, variable_table *variables_t, w_variable *last, int thread_id, bool no_error, std::string &potential_function, std::string &last_name)
{
    std::string function = "!" + c->value; // Contre un bug dans la recherche par les std::map
    std::string varname = c->value;        // Contre un bug dans la recherche par les std::map
    what_reference(thread_id)->push(c->reference);
    if (is_binary_operator(c))
    { // C'est un operateur donc on utilise directement les fonctions de lien entre
        // opérateurs et valeurs
        w_variable *a = visitor_compute_recursive(c->children[0], variables_t, last, thread_id, no_error, potential_function, last_name);
        w_variable *b = visitor_compute_recursive(c->children[1], variables_t, last, thread_id, no_error, potential_function, last_name);

        // On vérifie que les variables qu'on vient de créer sont bien désatribuées si elles sont inutiles
        w_variable *r = visitor_link_operator(a, b, c->value, thread_id);
        a->check_garbage();
        b->check_garbage();
        ref_pop(thread_id);
        return r;
    }
    else if (is_unary_operator(c))
    { // On s'attend donc à un traqueur
        w_variable *r = visitor_link_unary_operator(c, variables_t, thread_id);
        ref_pop(thread_id);
        return r;
    }
    else if (is_logic_operator(c))
    {
        return visitor_link_logic_operator(c, variables_t, thread_id);
    }
    else if (c->value == "{}")
    {
        return std::get<1>(visitor_visit_incode(c, variables_t, thread_id));
    }
    else if (c->pre_value != nullptr)
    { // On a déjà trouvé une valeur lors de la lecture du fichier, comme un entier ou une chaine de caractère
        ref_pop(thread_id);
        return c->pre_value;
    }
    else if (varname[0] == '.')
    {
        if (!last->is_object())
        {
            error("La dernière variable doît être un objet", c->reference, thread_id);
        }
        if (function_exist("!" + ((w_object *)last->content)->name + c->value, functions))
        {
            potential_function = "!" + ((w_object *)last->content)->name + c->value;
        }
        std::string n = remove_prefix(c->value); // On enlève le '.' au début du mot (fonction equivalente)
        w_object *o = (w_object *)last->content;
        // On crée un nouveau noeud égal, mais en ayant enlevé le préfixe '.'
        last_name = n;
        node *c_prime = new node(*c);
        c_prime->value = n;
        ref_pop(thread_id);
        return visitor_compute_recursive(c_prime, o->attributes, last, thread_id, no_error, potential_function, last_name); // On le réévalue donc en ayant connaissance de son objet parent
    }
    else if (variables_t->exist(varname))
    { // Dans ce cas on ne renvoie que la variable déjà existante
        last_name = varname;
        ref_pop(thread_id);
        return variables_t->get(varname);
    }
    else if (function_exist(function, functions))
    {
        w_variable *f = new w_variable("!" + c->value);
        f->type = T_FONCTION;
        ref_pop(thread_id);
        return f;
    }
    else if (parentethis_is_listed(c) and c->value == "()")
    { // On a plusieurs éléments, avec au moins une virgule entre chaque
        node *l = visitor_separate_listed(c);
        w_variable *r = create_list(l, variables_t, last, thread_id);
        ref_pop(thread_id);
        return r;
    }
    else if (c->value == "()")
    { // L'arbre se sépare de cette manière :
        // () -> __compute__ -> *
        node *n = new node(*c);
        n->value = COMP_KEYWORD;
        w_variable *r = visitor_compute_recursive(n, variables_t, last, thread_id, no_error, potential_function, last_name);
        ref_pop(thread_id);
        return r;
    }
    else if (c->value == "[]")
    {
        if (last == nullptr)
        {
            if (!parentethis_is_listed(c))
                error("Ne peux pas créer une plage sans 'listage'", c->reference, thread_id);
            node *n = visitor_separate_listed(c);
            std::vector<w_variable *> args;
            for (auto k : n->children)
                args.push_back(visitor_compute(k, variables_t, thread_id));
            if (args.size() != 2)
                error("Besoin de 2 arguments (bornes) pour créer une plage", c->reference, thread_id);
            w_object *r = new w_object();
            r->name = "plage";
            w_variable *self = new w_variable();
            self->type = T_OBJECT; // an object
            self->content = (void *)r;
            w_function *creator = functions["!plage.constructeur"];
            variable_table v = variable_table();
            v.assign("self", self, thread_id);
            v.assign(creator->arguments->children[0]->children[0]->children[0]->value, args[0], thread_id);
            v.assign(creator->arguments->children[1]->children[0]->children[0]->value, args[1], thread_id);
            visitor_visit(creator->trunc, v, thread_id);
            return self;
        }
        else
        {
            w_variable *r;
            {
                w_variable *b_value = visitor_compute(c->children[0], variables_t, thread_id);

                std::string name = "!" + last->get_type() + ".index";

                if (!function_exist(name, functions))
                {
                    std::string err = "la fonction '" + name + "' n'existe pas";
                    error(err, c->reference, thread_id);
                }
                w_function *f = functions[name];

                variable_table variables_t_bis = variable_table();
                variables_t_bis.assign("self", last, thread_id);
                variables_t_bis.assign(f->arguments->children[0]->children[0]->children[0]->value, b_value, thread_id);
                ref_pop(thread_id);
                r = visitor_visit(f->trunc, variables_t_bis, thread_id);
            }
            r->use--;
            return r;
        }
    }
    else if (c->children.size() > 1 || c->value == "!")
    { // On a soit une fonction, soit un nouvel objet, soit des attributs
        w_variable *s = (w_variable *)nullptr;
        std::string n;
        w_variable *r = treate_multiple_values(c, last, variables_t, thread_id, s, n, no_error);
        ref_pop(thread_id);
        return r;
    }
    else if (c->children.size() > 0 and (c->value == COMP_KEYWORD || c->value == "{}"))
    { // C'est un nouveau calcul qui est caché en dessous
        ref_pop(thread_id);
        w_variable *last_to_return = visitor_compute_recursive(c->children[0], variables_t, last, thread_id, no_error, potential_function, last_name);
        for (int i = 1; i < c->children.size(); i++)
        {
            node *e = c->children[i];
            last_to_return->check_garbage();
            last_to_return = visitor_compute_recursive(e, variables_t, last, thread_id, no_error, potential_function, last_name);
        }
        return last_to_return;
    }
    else if (c->children.size() == 0 and c->value == COMP_KEYWORD)
    {
        ref_pop(thread_id);
        return nullptr;
    }
    else
    {
        if (!no_error)
            error("Instruction ou variable inconnue : '" + c->value + "'", c->reference, thread_id);
    }
    return last;
}

w_variable *visitor_compute(node *c, variable_table *variables_t, int thread_id)
{
    // Fait le lien entre la fonction recursive et la fonction appelée
    // Permet de lancer quelques erreurs (au cas ou)
    std::string onsenfout;
    return visitor_compute_recursive(c, variables_t, nullptr, thread_id, false, onsenfout, onsenfout);
}
