#include "pgm.h"

size_t get_file_size(FILE *f) {
  long old_pos = ftell(f);
  fseek(f, 0, SEEK_END);
  size_t sz = ftell(f);
  fseek(f, old_pos, SEEK_SET);
  return sz;
}

//Checks only for P2 type. P5 not implemented and results in error.
int is_valid_pgm_file(FILE *f, magic_status *status) {
  const char *magicP2 = "P2";
  const char *magicP5 = "P5";
  long old_pos = ftell(f);
  rewind(f);
  char buff[3];
  if (fread(buff, sizeof(*buff), 2, f) != 2)
    return 3;
  buff[2] = '\0';
  fseek(f, old_pos, SEEK_SET);
  *status = (strcmp(buff, magicP2) == 0) ? MAGIC_OK : 
            (strcmp(buff, magicP5) == 0) ? MAGIC_NOT_IMPLEMENTED : MAGIC_WRONG;
  return 0;
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

int parse_string_as_pgm(char *content, pgm *image) {
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
  return 0;
}

void cleanup(size_t, ...);

void cleanup(size_t no_args, ...) {
  va_list ap;
  va_start(ap, no_args);
  for(size_t i = 0; i < no_args; i++) {
    free(va_arg(ap, void*));
  }
  va_end(ap);
}

int load_pgm(char *path, pgm **image) {
  int err_no = 0;
  FILE *f = fopen(path, "r");
  if (f == NULL) return 4;
  size_t file_size = get_file_size(f);

  magic_status status;
  err_no = is_valid_pgm_file(f, &status);
  if (err_no != 0) return err_no;
  switch (status) {
  case MAGIC_OK:
    break;
  case MAGIC_NOT_IMPLEMENTED:
    return 5;
  case MAGIC_WRONG:
    return 6;
  default:
    return 7;
  }

  char *file_content = malloc(file_size * sizeof(*file_content) + sizeof(*file_content));
  /*                                                file size + \0                  */
  if (file_content == NULL) return 2;
  while (fread(file_content, sizeof(*file_content), file_size, f) != file_size);
  file_content[file_size] = '\0';
  remove_comments(file_content, strlen(file_content));
  *image = malloc(sizeof(**image));
  err_no = parse_string_as_pgm(file_content, *image);
  if (err_no != 0) return err_no;
  strcpy((*image)->path, path);
  cleanup(1, file_content);
  fclose(f);
  return 0;
}

int save_pgm(char* path, pgm** image) {
  pgm* img = (*image);
  int ret;
  if (image == NULL) return 8;
  FILE *f = fopen(path, "w");
  if (f == NULL) return 9;

  ret = fprintf(f, "%s\n", img->magic);
  ret = fprintf(f, "%ld %ld\n", img->w, img->h);
  ret = fprintf(f, "%d\n", img->depth);
  if (ret < 0) return 10;
  
  for (size_t y = 0; y < img->h; y++) {
    for (size_t x = 0; x < img->w; x++) {
      if (fprintf(f, "%d ", img->data[img->w * y + x]) < 0) return 10;
    }
    fprintf(f, "%c", '\n');
  }
  fclose(f);
  return 0;
}

void free_pgm(pgm** image) {
  free((*image)->data);
  free((*image));
}