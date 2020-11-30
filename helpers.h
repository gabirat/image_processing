#ifndef HELPERS_H
#define HELPERS_H

#include <stdio.h>
#include "pgm.h"
#include "errors.h"

int get_input_int();
error_status get_input_string(char** buffer);

#endif /* HELPERS_H */