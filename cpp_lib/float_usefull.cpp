#include <vector>
#include <iostream>
#include <map>
#include <string>
#include "math.h"

#include "float_usefull.hpp"

int64_t get_float_content (w_object *f)
{
    return *(int64_t*)(f->get_attribute("content")->content);
}

int64_t get_float_ten_power (w_object *f)
{
    return *(int64_t*)(f->get_attribute("ten_pow")->content);
}

float create_float(w_object *f)
{
    int64_t content = get_float_content(f);
    int64_t tp = get_float_ten_power(f);

    return content * pow(10, tp);
}

int give_ten_power(float f)
{
    int tp = 0;
    while (int64_t(f * pow(10, tp)) / pow(10, tp) != f)
    {
        tp += 1;
    }
    return tp;
}