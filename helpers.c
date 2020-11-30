#include "helpers.h"

#define BUFFER_SIZE 512

int get_input_int() {
  char buffer[BUFFER_SIZE];
  int num;
  int ret;
  do {
    while(fgets(buffer, BUFFER_SIZE, stdin) == NULL);
    ret = sscanf(buffer, "%d", &num);
  } while (ret != 1);
  return num;
}

error_status get_input_string(char** buffer) {
  error_status err = init_error_status();
  *buffer = malloc(BUFFER_SIZE * sizeof(*buffer));
  if (*buffer == NULL) {
    err.err_no = 11;
    err.err_t = ERROR_CRITICAL;
  }
  int ret;
    do {
    while(fgets(*buffer, BUFFER_SIZE, stdin) == NULL);
    ret = sscanf(*buffer, "%s", *buffer);
  } while (ret != 1);
  return err;
}