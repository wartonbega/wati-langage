#include <vector>
#include <iostream>
#include <map>
#include <string>
#include "math.h"

#include "../src/include/node.hpp"
#include "../src/include/variables.hpp"
#include "../src/include/visitor.hpp"

int64_t get_float_content (w_object *f);
int64_t get_float_ten_power (w_object *f);
float create_float(w_object *f);
int give_ten_power(float f);