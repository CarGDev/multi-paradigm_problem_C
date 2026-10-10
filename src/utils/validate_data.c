#include "validate_data.h"
#include <stdlib.h>

/*
 * Classifies a text cell: TYPE_INT if all of it is an integer, TYPE_DOUBLE if
 * all of it is a decimal number, TYPE_INVALID if it is empty, NULL or text.
 */
ColumnType validate_data(char *val) {
  if (val == NULL || *val == '\0') {
    return TYPE_INVALID;
  }

  char *endptr;

  strtol(val, &endptr, 10);

  if (endptr != val && *endptr == '\0') {
    return TYPE_INT;
  }

  strtod(val, &endptr);
  if (endptr != val && *endptr == '\0') {
    return TYPE_DOUBLE;
  }
  return TYPE_INVALID;
}
