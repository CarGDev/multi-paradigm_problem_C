#include "core/data.h"
#include "core/order_data.h"
#include "tui/menu.h"
#include <assert.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv) {
  Header *headers = csv_headers(argc, argv);
  if (!headers) {
    fprintf(stderr, "statistics: no data\n");
    return 1;
  }

  int option = menu(headers);
  if (option < 0) {
    fprintf(stderr, "statistics: no numeric column selected\n");
    free_headers(headers);
    return 1;
  }

  creatingResults(headers, option, argv);
  free_headers(headers);

  return 0;
}
