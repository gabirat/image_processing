#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdarg.h>
#define PGM_NAME_LENGTH 255

typedef struct {
  char magic[3];
  char name[PGM_NAME_LENGTH];
  int16_t depth;
  int32_t w;
  int32_t h;
  size_t data_size;
  int16_t* data;
} pgm;

typedef enum {
  MAGIC_WRONG,
  MAGIC_NOT_IMPLEMENTED,
  MAGIC_OK
} magic_status;

int load_pgm(char* path, pgm** image);
/* load_pgm checks only if magic is correct and assumes rest of the 
   file follows the specification */