#include <string>

enum types 
{
    T_FONCTION,
    T_CHAR,
    T_INT,
    T_OBJECT,
    T_TRACKER
};



struct w_variable
{
    // the type of the variable : 
    // 0 -> function
    // 1 -> char
    // 2 -> int
    // 3 -> object
    // 4 -> tracker
    int type;
    
    // the pointer to the real value
    void *content;
    // we can cast it according to the type of the variable
    bool lock; // if true, then the memory is being accessed by a thread. In this case, do not try to access it.
    int use; // Le nombre d'usage de cette variable. Si 0 et que garbage_collector est appelé, alors elle est supprimée

    ~w_variable();
    w_variable();
    w_variable(int64_t);
    w_variable(std::string);

    void garbage_collector();

    std::string get_type();
    bool is_object();
    std::string convert_to_char();
    int64_t convert_to_int();

};
