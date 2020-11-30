#include "pgm.h"
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

int get_input_string(char** buffer) {
  *buffer = malloc(BUFFER_SIZE * sizeof(*buffer));
  if (*buffer == NULL) return 11;
  int ret;
    do {
    while(fgets(*buffer, BUFFER_SIZE, stdin) == NULL);
    ret = sscanf(*buffer, "%s", *buffer);
  } while (ret != 1);
  return 0;
}