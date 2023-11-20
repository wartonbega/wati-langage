

#include <map>
#include <vector>
#include <string>

#include "node.hpp"
#include "variables.hpp"

#ifndef TRACKERS_H  
#define TRACKERS_H


struct tracker 
{
    variable_table *variable_t;
    std::string name_to_track;
    bool usable; // Permet de savoir si la table de variable existe toujours
    
    // Si le traqueur traque un index dans un itérable
    w_variable *list; // la liste potentielle
    w_variable *index; // l'index traqué dans la liste (sous forme de w_variable)
    bool iterable; // permet de savoir si on traque un itérable ou non

    tracker(std::string, variable_table *);
    tracker(w_variable *, w_variable *);
    w_variable *value(int);
    void set_value(w_variable *, int);
    std::string en_string(variable_table var_t, int thread_id);
};


#endif