#include "pgm.h"

size_t get_file_size(FILE *f) {
  long old_pos = ftell(f);
  fseek(f, 0, SEEK_END);
  size_t sz = ftell(f);
  fseek(f, old_pos, SEEK_SET);
  return sz;
}

//Checks only for P2 type. P5 not implemented and results in error.
error_status is_valid_pgm_file(FILE *f, magic_status *status) {
  error_status err = init_error_status();
  const char *magicP2 = "P2";
  const char *magicP5 = "P5";
  long old_pos = ftell(f);
  rewind(f);
  char buff[3];
  if (fread(buff, sizeof(*buff), 2, f) != 2) {
    err.err_no = 3;
    err.err_t = ERROR_NONCRITICAL;
    return err;
  }
  buff[2] = '\0';
  fseek(f, old_pos, SEEK_SET);
  *status = (strcmp(buff, magicP2) == 0) ? MAGIC_OK : 
            (strcmp(buff, magicP5) == 0) ? MAGIC_NOT_IMPLEMENTED : MAGIC_WRONG;
  return err;
}

void remove_comments(char *content, size_t content_size) {
  for (size_t i = 0; i < content_size; i++) {
    if (content[i] == '#') {
      while (content[i] != '\n') {
        content[i] = ' ';
        i++;
      }
    }
  }
}

error_status parse_string_as_pgm(char *content, pgm *image) {
  error_status err = init_error_status();
  strcpy(image->magic, strtok(content, " \t\r\n")); //Magic
  image->w = atoi(strtok(NULL, " \t\r\n")); //Width
  image->h = atoi(strtok(NULL, " \t\r\n")); //Height
  image->depth = atoi(strtok(NULL, " \t\r\n")); //Depth
  image->data_size = (size_t)(image->w * image->h);
  image->data = (int16_t*)malloc(image->data_size * sizeof(*image->data));
  for (size_t i = 0; i < image->data_size; i++) {
    image->data[i] = atoi(strtok(NULL, " \t\r\n"));
  }
  /* That thing above is REALLY REALLY unsafe, please NEVER do it in production
     Here just for the sake of simplicity, never trust your input */
  return err;
}

error_status load_pgm(char *path, pgm **image) {
  error_status err = init_error_status();
  FILE *f = fopen(path, "r");
  if (f == NULL) {
    err.err_no = 4;
    err.err_t = ERROR_NONCRITICAL;
    return err;
  }

  size_t file_size = get_file_size(f);

  magic_status status;
  err = is_valid_pgm_file(f, &status);
  if (err.err_t != NO_ERROR) return err;

  switch (status) {
  case MAGIC_OK:
    break;
  case MAGIC_NOT_IMPLEMENTED:
    err.err_no = 5;
    err.err_t = ERROR_NONCRITICAL;
    return err;
  case MAGIC_WRONG:
    err.err_no = 6;
    err.err_t = ERROR_NONCRITICAL;
    return err;
  default:
    err.err_no = 7;
    err.err_t = ERROR_NONCRITICAL;
    return err;
  }

  char *file_content = malloc(file_size * sizeof(*file_content) + sizeof(*file_content));
  /*                                                file size + \0                  */
  if (file_content == NULL) {
    err.err_no = 2;
    err.err_t = ERROR_CRITICAL;
    return err;
  }
  while (fread(file_content, sizeof(*file_content), file_size, f) != file_size);
  file_content[file_size] = '\0';
  remove_comments(file_content, strlen(file_content));
  *image = malloc(sizeof(**image));
  err = parse_string_as_pgm(file_content, *image);
  if (err.err_t != NO_ERROR) {
    free(file_content);
    fclose(f);
    return err;
  }
  strcpy((*image)->path, path);
  free(file_content);
  fclose(f);
  return err;
}

error_status save_pgm(char* path, pgm** image) {
  error_status err = init_error_status();
  pgm* img = (*image);
  int ret;
  if (image == NULL) {
    err.err_no = 8;
    err.err_t = ERROR_NONCRITICAL;
    return err;
  }
  FILE *f = fopen(path, "w");
  if (f == NULL) {
    err.err_no = 9;
    err.err_t = ERROR_NONCRITICAL;
    return err;
  }

  ret = fprintf(f, "%s\n%ld %ld\n%d\n", img->magic, img->w, img->h, img->depth);
  if (ret < 0) {
    err.err_no = 10;
    err.err_t = ERROR_NONCRITICAL;
    close(f);
    return err;
  }
  
  for (size_t y = 0; y < img->h; y++) {
    for (size_t x = 0; x < img->w; x++) {
      if (fprintf(f, "%d ", img->data[img->w * y + x]) < 0) {
        err.err_no = 10;
        err.err_t = ERROR_NONCRITICAL;
        close(f);
        return err;
      }
    }
    if (fprintf(f, "%c", '\n') < 0) {
      err.err_no = 10;
      err.err_t = ERROR_NONCRITICAL;
      close(f);
      return err;
    }
  }
  fclose(f);
  return err;
}

void free_pgm(pgm* image) {
  free(image->data);
  free(image);
}