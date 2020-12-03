#include "pgm_utils.h"

typedef struct s_filter_window {
  size_t size;
  size_t data_size;
  int16_t* data;
} filter_window;

int16_t get_pixel(int x, int y, pgm* image) {
  if (x < 0 || x >= (int)image->w || y < 0 || y >= (int)image->h) return 0;
  return image->data[image->w * y + x];
}

void set_pixel(int x, int y, int16_t value, pgm* image) {
  image->data[image->w * y + x] = value;
}

error_status pgm_rotate(int k_90_deg, pgm* image) {
  error_status err = init_error_status();
  k_90_deg %= 4;
  int16_t* new_data;
  int tmp = 0;
  switch (k_90_deg) {
  case 0:
    break;
  case 1:
    new_data = malloc((image->w * image->h) * sizeof(int16_t));
    if (new_data == NULL) {
      err.err_no = 17;
      err.err_t = ERROR_CRITICAL;
      return err;
    }
    for (size_t x = 0; x < image->w; x++) {
      for (size_t y = 0; y < image->h; y++) {
        new_data[image->h * (image->w - 1 - x) + y] = get_pixel(x , y, image);
      }
    }
    free(image->data);
    image->data = new_data;
    tmp = image->w;
    image->w = image->h;
    image->h = tmp;
    break;
  case 2:
    for (size_t i = 0, backwards = (image->data_size - 1);
         backwards > i; i++, backwards--) {
      int tmp = image->data[i];
      image->data[i] = image->data[backwards];
      image->data[backwards] = tmp;
    }
    break;
  case 3:
    new_data = malloc((image->w * image->h) * sizeof(int16_t));
    if (new_data == NULL) {
      err.err_no = 17;
      err.err_t = ERROR_CRITICAL;
      return err;
    }
    for (size_t x = 0; x < image->w; x++) {
      for (size_t y = 0; y < image->h; y++) {
        new_data[image->h * (image->w - 1 - x) + y] = get_pixel(x , y, image);
      }
    }
    free(image->data);
    image->data = new_data;
    tmp = image->w;
    image->w = image->h;
    image->h = tmp;
    for (size_t i = 0, backwards = (image->data_size - 1);
         backwards > i; i++, backwards--) {
      tmp = image->data[i];
      image->data[i] = image->data[backwards];
      image->data[backwards] = tmp;
    }
    break;
  default:
      break;
  }
  return err;
}

error_status pgm_save_histogram_as_csv(char* path, pgm* image) {
  error_status err = init_error_status();
  int16_t* histogram = malloc(image->depth * sizeof(*histogram));
  if (histogram == NULL) {
    err.err_no = 18;
    err.err_t = ERROR_CRITICAL;
    return err;
  }
  memset(histogram, 0, image->depth * sizeof(*histogram)); //It's ok for zeroing int arrays
  for (size_t i = 0; i < image->data_size; i++) {
    histogram[image->data[i]] += 1;
  } 
  FILE* f = fopen(path, "w");
  if (f == NULL) {
    err.err_no = 19;
    err.err_t = ERROR_NONCRITICAL;
    return err;
  }
  for (size_t i = 0; i < (size_t)image->depth; i++) {
    if (fprintf(f, "%ld;%d\n", i, histogram[i]) < 0) {
      err.err_no = 20;
      err.err_t = ERROR_NONCRITICAL;
      fclose(f);
      return err;
    }
  }
  fclose(f);
  return err;
}

void pgm_negate(pgm* image) {
  for (size_t i = 0; i < image->data_size; i++) {
    image->data[i] = image->depth - image->data[i];
  } 
}

int16_t filter_median(filter_window* window) {
  float tmp = 0;
  for (size_t i = 0; i < window->size - 1; i++) {
    for (size_t j = 0; j < window->size - i - 1; j++) {
      if (window->data[j] > window->data[j+1]) {
        tmp = window->data[j];
        window->data[j] = window->data[j+1];
        window->data[j+1] = tmp;
      }
    }
  }
  return window->data[window->data_size/2];
}

void set_window_pixel(filter_window* window, int x, int y, int16_t value) {
  window->data[window->size * y + x] = value;
}

error_status pgm_median_filter(size_t window_size, pgm* image) {
  error_status err = init_error_status();
  if (window_size % 2 == 0) {
    err.err_no = 21;
    err.err_t = ERROR_NONCRITICAL;
  }
  if (window_size < 3) {
    err.err_no = 22;
    err.err_t = ERROR_NONCRITICAL;
  }
  filter_window* window =  malloc(sizeof(*window));
  if (window == NULL) {
    err.err_no = 23;
    err.err_t = ERROR_CRITICAL;
  }
  window->size = window_size;
  window->data_size = window->size * window->size;
  window->data = malloc(window->data_size * sizeof(*window->data));

  for (size_t x = 0; x < image->w; x++) {
    for (size_t y = 0; y < image->h; y++) {
      for (size_t x_w = 0; x_w < window->size; x_w++) {
        for (size_t y_w = 0; y_w < window->size; y_w++) {
          set_window_pixel(window, x_w, y_w,
                           get_pixel(((x - window->size/2) + x_w),
                                     ((y - window->size/2) + y_w), image));
        }
      }
      set_pixel(x, y, filter_median(window), image);
    }
  }
  free(window);
  return err;
}

void pgm_make_noise(float chance, pgm* image) {
  for (size_t i = 0; i < image->data_size; i++){
    if (RANDOM() < chance) {
      image->data[i] = (RANDOM() < 0.5) ? 0 : image->depth;
    }
  }
}