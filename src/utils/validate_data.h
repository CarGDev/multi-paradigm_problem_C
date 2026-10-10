#ifndef VALIDATE_DATA_H
#define VALIDATE_DATA_H

#include <stdbool.h>
#include <stddef.h>

typedef enum { TYPE_INT, TYPE_DOUBLE, TYPE_INVALID } ColumnType;

ColumnType validate_data(char *val);

#endif
