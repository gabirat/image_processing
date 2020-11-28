#include "pgm.h"
#include <stdio.h>


int parse_pgm_header(pgm_hdr hdr) {

}

size_t get_file_size(FILE* f) {
    long old_pos = ftell(f);
    fseek(f, 0, SEEK_END);
    size_t sz = ftell(f);
    fseek(f, old_pos, SEEK_SET);
    return sz;
}

int load_pgm(char* path, pgm* image) {
    FILE* f = fopen(path, "r");
    size_t file_size = get_file_size(f);
    fclose(f);
}