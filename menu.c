#include "menu.h"

int add_image(ctx* context) {
  
}

void free_ctx(ctx* context) {
  for (size_t i = 0; i < context->images_size; i++) {
      free_pgm(context->images[i]);
  }
  free(context->images);
}