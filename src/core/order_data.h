#ifndef ORDER_DATA_H
#define ORDER_DATA_H

#include "../utils/validate_data.h"
#include "data.h"
#include <stdbool.h>
#include <stddef.h>

typedef struct {
  ColumnType type;

  union {
    long *i;
    double *d;
  } values;

  size_t count;
} Values_Data;

Values_Data *order(Header *headers, int option, char **argv);

#endif
