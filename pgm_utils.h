#ifndef PGM_UTILS_H
#define PGM_UTILS_H

#include "errors.h"
#include "pgm.h"

#define RANDOM() (((float)rand())/((float)RAND_MAX)) //Returns a float in range 0.0 - 1.0

// typedef enum {
//   FILTER_MEDIAN,
//   FILTER_GAUSSIAN
// } filter_type;

error_status pgm_rotate(int k_90_deg, pgm* image);
error_status pgm_save_histogram_as_csv(char* path, pgm* image);
void pgm_negate(pgm* image);
void pgm_make_noise(float chance, pgm* image);
error_status pgm_median_filter(size_t window_size, pgm* image);

#endif