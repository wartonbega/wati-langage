#include "variables.h"

void w_variable::garbage_collector() {
    if (this->use == 0) 
    {
        delete this;
        return;
    }
}

int main()
{
    return 0;
}