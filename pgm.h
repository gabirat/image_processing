#ifndef PGM_H
#define PGM_H

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include "helpers.h"

#define PGM_PATH_LENGTH 512

typedef struct {
  char magic[3];
  char path[PGM_PATH_LENGTH];
  int16_t depth;
  size_t w;
  size_t h;
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

int save_pgm(char* path, pgm** image);
/* saves image to sepcified path*/

void free_pgm(pgm* image);
/* frees image data and image structure*/

#endif /* PGM_H */