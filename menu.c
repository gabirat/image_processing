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

error_status check_image_index(ctx* context, int idx) {
  error_status err = init_error_status();
  if (idx < 0 || (size_t)idx >= context->images_size) {
    err.err_no = 15;
    err.err_t = ERROR_NONCRITICAL;
    return err;
  }
  return err;
}

error_status add_image(ctx* context) {
  error_status err = init_error_status();
  char* path = NULL;
  printf("Podaj sciezke do pliku PGM do wczytania: ");
  err = get_input_string(&path);
  if (err.err_t != NO_ERROR) {
    return err;
  }
  context->images = (pgm**)realloc(context->images, 
                                   sizeof(pgm*) * (context->images_size + 1));
  if (context->images == NULL) {
    err.err_no = 13;
    err.err_no = ERROR_CRITICAL;
    return err;
  }
  err = load_pgm(path, &context->images[context->images_size]);
  if (err.err_t != NO_ERROR) {
    return err;
  }
  context->images_size++;
  context->active_image = context->images[context->images_size - 1];
  free(path);
  return err;
}

error_status delete_image(ctx* context) {
  error_status err = init_error_status();
  printf("Podaj numer obrazu to usuniecia: ");
  int to_del = get_input_int(); //Index of image + 1
  err = check_image_index(context, to_del - 1);
  if (err.err_t != NO_ERROR) return err;
  if (context->images[to_del - 1] == context->active_image) {
    context->active_image = NULL;
  }
  free_pgm(context->images[to_del - 1]);
  memmove(context->images[to_del - 1], 
          context->images[to_del],
         (context->images_size - to_del) * sizeof(pgm*));
  context->images = realloc(context->images, 
                           (context->images_size - 1) * sizeof(pgm*));
  if (context->images == NULL) {
    err.err_no = 14;
    err.err_t = ERROR_CRITICAL;
    return err;
  }
  return err;
}

error_status delete_active_image(ctx* context) {
  error_status err = init_error_status();
  int to_del = 0; //Actual index of the image
  while(context->images[to_del] != context->active_image) to_del++; //Get index of active img
  context->active_image = NULL;
  free_pgm(context->images[to_del]);
  memmove(context->images[to_del], 
          context->images[to_del + 1],
         (context->images_size - to_del) * sizeof(pgm*));
  context->images = realloc(context->images, 
                           (context->images_size - 1) * sizeof(pgm*));
  if (context->images == NULL) {
    err.err_no = 14;
    err.err_t = ERROR_CRITICAL;
    return err;
  }
  return err;
}

void display_images_list(ctx* context) {
  if (context->images_size == 0) {
    printf("Nie wczytano jeszczse zadnego pliku");
    return;
  }
  printf("Lista zaladowanych plikow:\n");
  for (size_t i = 0; i < context->images_size; i++) {
    printf("[%ld] %s\n", i, context->images[i]->path);
  }
}

error_status choose_active_image(ctx* context) {
  error_status err = init_error_status();
  printf("Jaki obraz ma byc aktywny?: ");
  int to_be_active = get_input_int();
  err = check_image_index(context, to_be_active - 1);
  if (err.err_t != NO_ERROR) return err;
  context->active_image = context->images[to_be_active - 1];
  return err;
}

error_status save_image(ctx* context) {
  error_status err = init_error_status();
  char* path = NULL;
  printf("Podaj numer obrazu zapisu: ");
  int to_save = get_input_int();
  err = check_image_index(context, to_save - 1);
  if (err.err_t != NO_ERROR) return err;
  printf("Podaj sciezke do zapisu: ");
  err = get_input_string(&path);
  if (err.err_t != NO_ERROR) {
    return err;
  }
  save_pgm(path, context->images[to_save - 1]);
  free(path);
  return err;
}

error_status save_active_image(ctx* context) {
  error_status err = init_error_status();
  if (context->active_image == NULL) {
    err.err_no = 16;
    err.err_t = ERROR_NONCRITICAL;
    return err;
  }
  char* path = NULL;
  printf("Podaj sciezke do zapisu: ");
  err = get_input_string(&path);
  if (err.err_t != NO_ERROR) {
    return err;
  }
  save_pgm(path, context->active_image);
  free(path);
  return err;
}

void display_menu(ctx* context) {
  const char* menu_string =
    "1. Wczytaj obraz\n"
    "2. Usun obraz\n"
    "3. Usun aktywny obraz\n"
    "4. Zapisz obraz z bazy\n"
    "5. Zapisz aktywny obraz\n"
    "6. Wyswietl wczytane obrazy\n"
    "7. Wybierz aktywny obraz\n"
    "8. Edytuj aktywny obraz\n"
    "9. Wyjdz\n"
    "\nObecnie aktywny obraz: %s\n";
  char* active_path = (context->active_image != NULL) ? context->active_image->path : "Nie wybrano";
  printf(menu_string, active_path);
}

void display_edit_menu(ctx* context) {
  const char* menu_string =
    "1. Obrot o 90*k stopni\n"
    "2. Zapisz histogram jako .csv\n"
    "3. Negatyw\n"
    "4. Zaszum\n"
    "5. Odszum filtrem medianowym\n"
    "6. Cofnij\n"
    "\nObecnie aktywny obraz: %s\n";
  char* active_path = (context->active_image != NULL) ? context->active_image->path : "Nie wybrano";
  printf(menu_string, active_path);
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

error_status menu_loop(ctx* context) {
  while (/* condition */) {
    /* code */
  }
}