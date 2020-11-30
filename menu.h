#ifndef MENU_H
#define MENU_H

#include "helpers.h"
#include "pgm.h"

typedef struct main_ctx {
  int err_no;
  pgm* active_image;
  size_t images_size;
  pgm* images;
} ctx;

int add_image(ctx* context);
void delete_image(ctx* context);
void show_images_list(ctx* context);
void choose_active_image(ctx* context);

void free_ctx(ctx* context);

#endif /* MENU_H */