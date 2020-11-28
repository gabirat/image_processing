#include <stdint.h>
#define NAME_LENGTH 128

typedef struct {
    char magic[2];
    char name[NAME_LENGTH];
    int16_t depth;
    int32_t w;
    int32_t h;
} pgm_hdr;

typedef struct {
    pgm_hdr header;
    int16_t* data;
} pgm;

int load_pgm(char* path, pgm* image);