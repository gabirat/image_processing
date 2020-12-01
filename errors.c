#include "errors.h"

const char* SIGNALS_ERROR_CODES[] = {
  "Not a valid PGM file (magic mismatch)",
  "Can't allocate memory to load the file",
  "Error reading file magic",
  "Can't open image file",
  "File version not implemented",
  "Wrong file signature",
  "Unknown error after file signature check",
  "Can't save - image is NULL",
  "Can't open file for writing",
  "Error writing file",
  "Can't allocate memory to load a string",
  "Can't allocate memory to initialize context",
  "Can't reallocate memory for a new image",
  "Can't reallocate memory after deleting",
  "Input value out of range",
  "No active image"
};

error_status inline init_error_status() {
  error_status err;
  err.err_no = 0;
  err.err_t = NO_ERROR;
  return err;
}

void print_error_message(int error_code) {
  fprintf(stderr, "Error code: %d\nMessage: %s\n", error_code, SIGNALS_ERROR_CODES[error_code - 1]);
}