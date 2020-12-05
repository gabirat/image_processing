#include "menu.h"

error_status init_ctx(ctx** context) {
  error_status err = init_error_status();
  *context = malloc(sizeof(**context));
  if (*context == NULL) {
    err.err_no = 12;
    err.err_t = ERROR_CRITICAL;
  }
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
  if (context->images_size == 0) {
    err.err_no = 25;
    err.err_t = ERROR_NONCRITICAL;
    return err;
  }
  printf("Podaj numer obrazu to usuniecia: ");
  int to_del = get_input_int(); //Index of image + 1
  to_del -= 1; //Actual index of the image
  err = check_image_index(context, to_del);
  if (err.err_t != NO_ERROR) return err;
  if (context->images[to_del] == context->active_image) {
    context->active_image = NULL;
  }
  free_pgm(context->images[to_del]);
  if ((size_t)to_del != context->images_size - 1) { //Don't move if last index deleted
    memmove(context->images[to_del], 
        context->images[to_del + 1],
        (context->images_size - to_del) * sizeof(pgm*));
  }
  context->images = realloc(context->images, 
                           (context->images_size) * sizeof(pgm*));
  context->images_size -= 1;
  if (context->images == NULL && context->images_size > 0) {
    err.err_no = 14;
    err.err_t = ERROR_CRITICAL;
    return err;
  }
  return err;
}

error_status delete_active_image(ctx* context) {
  error_status err = init_error_status();
  if (context->active_image == NULL) {
    err.err_no = 24;
    err.err_t = ERROR_NONCRITICAL;
    return err;
  }
  size_t to_del = 0; //Actual index of the image
  while(context->images[to_del] != context->active_image) to_del++; //Get index of active img
  context->active_image = NULL;
  free_pgm(context->images[to_del]);
  if (to_del != context->images_size - 1) { //Don't move if last index deleted
    memmove(context->images[to_del], 
        context->images[to_del + 1],
        (context->images_size - to_del) * sizeof(pgm*));
  }
  context->images = realloc(context->images, 
                           (context->images_size - 1) * sizeof(pgm*));
  context->images_size -= 1;
  if (context->images == NULL && context->images_size > 0) {
    err.err_no = 14;
    err.err_t = ERROR_CRITICAL;
    return err;
  }
  return err;
}

error_status display_images_list(ctx* context) {
  error_status err = init_error_status();
  if (context->images_size == 0) {
    err.err_no = 25;
    err.err_t = ERROR_NONCRITICAL;
    return err;
  }
  printf("Lista zaladowanych plikow:\n");
  for (size_t i = 0; i < context->images_size; i++) {
    printf("[%ld] %s\n", i + 1, context->images[i]->path);
  }
  printf("\n");
  return err;
}

error_status choose_active_image(ctx* context) {
  error_status err = init_error_status();
  err = display_images_list(context);
  if (err.err_t != NO_ERROR) return err;
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
  char* active_path = (context->active_image != NULL) ?
                       context->active_image->path : "Nie wybrano";
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
  char* active_path = (context->active_image != NULL) ? 
                       context->active_image->path : "Nie wybrano";
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

error_status edit_rotate(ctx* context) {
  error_status err = init_error_status();
  printf("O ile k*90 obrocic? k: ");
  int k = get_input_int();
  err = pgm_rotate(k, context->active_image);
  return err;
}

error_status edit_save_histogram(ctx* context) {
  error_status err = init_error_status();
  char* path = NULL;
  printf("Podaj sciezke do zapisania histogramu: ");
  err = get_input_string(&path);
  if (err.err_t != NO_ERROR) return err;
  err = pgm_save_histogram_as_csv(path, context->active_image);
  free(path);
  return err;
}

error_status edit_negate(ctx* context) {
  error_status err = init_error_status();
  pgm_negate(context->active_image);
  return err;
}

error_status edit_noise(ctx* context) {
  error_status err = init_error_status();
  float chance = 0;
  do {
    printf("Podaj szanse na zaszumienie pixela (0.0 - 1.0): ");
    chance = get_input_float();
  } while(chance < 0 || chance > 1.0);
  pgm_make_noise(chance, context->active_image);
  return err;
}

error_status edit_filter(ctx* context) {
  error_status err = init_error_status();
  int window_size = 0;
  do {
    printf("Podaj wielkosc okna (musi byc > 0 i nieparzysta): ");
    window_size = get_input_int();
  } while((window_size < 0) || (window_size % 2 == 0));
  err = pgm_median_filter(window_size, context->active_image);
  return err;
}

error_status edit_menu_loop(ctx* context) {
  error_status err = init_error_status();
  if (context->active_image == NULL) {
    err.err_no = 16;
    err.err_t = ERROR_NONCRITICAL;
    return err;
  }
  
  int is_finished = 0;
  int choice = 0;
  while (!is_finished) {
    display_edit_menu(context);
    if (err.err_t == ERROR_CRITICAL) {
      return err;
    }
    if (err.err_t == ERROR_NONCRITICAL) {
      print_error_message(err.err_no);
    }
    do {
      printf("Wybor: ");
      choice = get_input_int();
    } while (choice < 1 || choice > 6);

    switch (choice) {
    case 1:
      err = edit_rotate(context);
      clear();
      break;
    case 2:
      err = edit_save_histogram(context);
      clear();
      break;
    case 3:
      err = edit_negate(context);
      clear();
      break;
    case 4:
      err = edit_noise(context);
      clear();
      break;
    case 5:
      err = edit_filter(context);
      clear();
      break;
    case 6:
      is_finished = 1;
      err = init_error_status();
      break;
    default:
      break;
    }
  }
  return err;
}

error_status main_menu_loop(ctx* context) {
  error_status err = init_error_status();
  int is_finished = 0;
  int choice = 0;
  while (!is_finished) {
    display_menu(context);
    if (err.err_t == ERROR_CRITICAL) {
      return err;
    }
    if (err.err_t == ERROR_NONCRITICAL) {
      print_error_message(err.err_no);
    }
    do {
      printf("Wybor: ");
      choice = get_input_int();
    } while (choice < 1 || choice > 9);

    switch (choice) {
    case 1:
      err = add_image(context);
      clear();
      break;
    case 2:
      err = delete_image(context);
      clear();
      break;
    case 3:
      err = delete_active_image(context);
      clear();
      break;
    case 4:
      err = save_image(context);
      clear();
      break;
    case 5:
      err = save_active_image(context);
      clear();
      break;
    case 6:
      clear();
      err = display_images_list(context);
      break;
    case 7:
      err = choose_active_image(context);
      clear();
      break;
    case 8:
      clear();
      err = edit_menu_loop(context);
      clear();
      break;
    case 9:
      is_finished = 1;
      err = init_error_status();
      break;
    default:
      break;
    }
  }
  return err;
}