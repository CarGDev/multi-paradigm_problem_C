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
  }
  int option = menu(headers);
  creatingResults(headers, option, argv);

  return 0;
}
