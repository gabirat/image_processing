#ifndef ERRORS_H
#define ERRORS_H

#include <stdio.h>

typedef enum e_err_t {
    ERROR_CRITICAL,
    ERROR_NONCRITICAL,
    NO_ERROR
} error_type;

typedef struct s_err_status {
    error_type err_t;
    int err_no;
} error_status;

error_status init_error_status();
void print_error_message(int error_code);

#endif /* ERRORS_H */