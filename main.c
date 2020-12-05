#include <time.h>
#include "pgm.h"
#include "errors.h"
#include "menu.h"

int main() {
  srand(time(NULL));
  error_status err = init_error_status();
  ctx* context;
  err = init_ctx(&context);
  if (err.err_t != NO_ERROR) {
    print_error_message(err.err_no);
    return err.err_no;
  }
  err = main_menu_loop(context);
  if (err.err_t != NO_ERROR) {
    print_error_message(err.err_no);
    return err.err_no;
  }
  free_ctx(context);
  return 0;
}
