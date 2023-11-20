#include "./include/trackers.hpp"
#include "./include/visitor.hpp"
#include "./include/types.hpp"
#include "./include/inbuilds.hpp"

#include <stdlib.h>
#include <setjmp.h>
#include <signal.h>

jmp_buf jump;

void segv(int sig)
{
  longjmp(jump, 1);
}

int memcheck(void *x)
{
  volatile char c;
  int illegal = 0;

  signal(SIGSEGV, segv);

  if (!setjmp(jump))
    c = *(char *)(x);
  else
    illegal = 1;

  signal(SIGSEGV, SIG_DFL);

  return (illegal);
}

tracker::tracker(std::string expr, variable_table *var_t)
{
  if (!var_t->exist(expr))
  {
    std::string err = "la variable '" + expr + "' n'existe pas. On ne peut donc pas la traquer";
    error(err, what_reference(0)->top(), 0);
  }

  this->name_to_track = expr;
  this->variable_t = var_t;
  this->usable = 1;
  this->iterable = false;
  var_t->add_track((void *)this);
}

tracker::tracker(w_variable *l, w_variable *index)
{
  if (!l->is_object())
    error("Pour définir un traqueur pour un itérable, il faut que la variable soit un objet", "", 0);

  w_object *o = (w_object *)l->content;
  if (!o->is_iterable())
    error("La variable doit être un itérable", "", 0);

  this->iterable = true;
  this->index = index;
  this->list = l;
}

w_variable *tracker::value(int thread_id)
{
  if (this->iterable)
  {
    variable_table t = variable_table();
    std::string f_name = "!" + this->list->get_type() + ".index";
    if (!function_exist(f_name, functions))
      error("La fonction " + f_name + "' n'existe pas", "", thread_id);
    w_function *f = functions[f_name];

    t.assign("self", this->list, thread_id);
    t.assign(f->get_arg_name(0), this->index, thread_id);

    return visitor_visit(f->trunc, t, thread_id);
  }
  else
  { // we check if the variable still exists
    if (!this->variable_t->exist(this->name_to_track))
    {
      std::string err = "Le traqueur visant '" + this->name_to_track + "' n'as plus rien à traquer. La variable '" + this->name_to_track + "' n'est plus accessible";
      error(err, what_reference(thread_id)->top(), thread_id);
    }
    // finally we can return the right value
    return this->variable_t->get(this->name_to_track);
  }
}

void tracker::set_value(w_variable *v, int thread_id)
{
  if (this->iterable)
  {
    variable_table t = variable_table();
    std::string f_name = "!" + this->list->get_type() + ".modifie";
    if (!function_exist(f_name, functions))
      error("La fonction " + f_name + "' n'existe pas", "", thread_id);
    w_function *f = functions[f_name];

    t.assign("self", this->list, thread_id);
    t.assign(f->get_arg_name(0), this->index, thread_id);
    t.assign(f->get_arg_name(1), v, thread_id);

    visitor_visit(f->trunc, t, thread_id);
  }
  else
  {
    // On vérifie d'abord que la table de variable est toujours existante
    if (this->usable == 0)
    {
      std::string err = "le traqueur visant '" + this->name_to_track + "' provient d'un endroit maintenant non existant";
      error(err, what_reference(thread_id)->top(), thread_id);
    }

    // we check if the variable still exists
    if (!this->variable_t->exist(this->name_to_track))
    {
      std::string err = "le traqueur visant '" + this->name_to_track + "' n'as plus rien à traquer. La variable '" + this->name_to_track + "' n'est plus accessible";
      error(err, what_reference(thread_id)->top(), thread_id);
    }

    // we assign to the right value
    this->variable_t->assign(this->name_to_track, v, thread_id);
  }
}

std::string tracker::en_string(variable_table var_t, int thread_id)
{
  w_variable *t = this->value(thread_id);
  std::string c;

  // on ne veux pas qu'il y ait une erreur de récursion
  if (t->get_type() == "traqueur")
    c = "<traqueur ...>";
  else
    c = convert_to_string(t, var_t, thread_id);

  if (this->iterable)
    return "<traqueur &={itérable : index=" + convert_to_string(this->index, var_t, thread_id) + "} *=" + c + ">";
  if (t->get_type() == "char")
    return "<traqueur &=" + this->name_to_track + " *=\"" + c + "\">";
  return "<traqueur &=" + this->name_to_track + " *=" + c + ">";
}