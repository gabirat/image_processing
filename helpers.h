#ifndef HELPERS_H
#define HELPERS_H

#include <stdio.h>
#include "pgm.h"
#include "errors.h"
#define clear() printf("\e[1;1H\e[2J")

int get_input_int();
float get_input_float();
error_status get_input_string(char** buffer);

#endif /* HELPERS_H */