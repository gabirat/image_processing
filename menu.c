#include "menu.h"

error_status init_ctx(ctx** context) {
  error_status err = init_error_status();
  *context = malloc(sizeof(**context));
  if (*context == NULL) {
    err.err_no = 12;
    err.err_t = ERROR_CRITICAL;
  }
  (*context)->err_stat.err_t = NO_ERROR;
  (*context)->err_stat.err_no = 0;
  (*context)->images_size = 0;
  (*context)->images = NULL;
  return err;
}

error_status add_image(ctx* context) {
  error_status err = init_error_status();
  char* path = NULL;
  err = get_input_string(&path);
  if (err.err_t != NO_ERROR) {
    return err;
  }
  context->images = (pgm**)realloc(context->images, sizeof(pgm*) * (context->images_size + 1));
  if (context->images == NULL) {
    err.err_no = 13;
    err.err_no = ERROR_CRITICAL;
    return err;
  }
  err = load_pgm(path, context->images[context->images_size]);
  if (err.err_t != NO_ERROR) {
    return err;
  }
  context->images_size++;
  free(path);
  return err;
}

void free_ctx(ctx* context) {
  if (context->images != NULL) {
      for (size_t i = 0; i < context->images_size; i++) {
      free_pgm(context->images[i]);
    }
  }
  free(context->images);
  free(context);
}