#ifndef DATA_H
#define DATA_H

#include "../utils/validate_data.h"
#include <stdbool.h>
#include <stddef.h>

typedef struct {
  char *name;
  size_t index;
  ColumnType type;
} Header;

Header *csv_headers(int argc, char **argv);

#endif
