#ifndef MENU_H
#define MENU_H

#include "helpers.h"
#include "pgm.h"
#include "pgm_utils.h"
#include "errors.h"

typedef struct s_main_ctx {
  pgm* active_image;
  size_t images_size;
  pgm** images;
} ctx;

error_status init_ctx(ctx** context);
error_status add_image(ctx* context);
error_status delete_image(ctx* context);
error_status choose_active_image(ctx* context);
error_status save_image(ctx* context);
error_status save_active_image(ctx* context);
error_status main_menu_loop(ctx* context);
error_status display_images_list(ctx* context);

error_status edit_menu_loop(ctx* context);
error_status edit_rotate(ctx* context);
error_status edit_save_histogram(ctx* context);
error_status edit_negate(ctx* context);
error_status edit_noise(ctx* context);
error_status edit_filter(ctx* context);

void display_menu(ctx* context);
void display_edit_menu(ctx* context);
void free_ctx(ctx* context);


#endif /* MENU_H */