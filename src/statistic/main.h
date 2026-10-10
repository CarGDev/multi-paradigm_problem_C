#ifndef MEAN_H
#define MEAN_H

#include "../core/order_data.h"
#include "../utils/validate_data.h"
#include <stdbool.h>
#include <stddef.h>

double summatory(Values_Data *list);
void mean(Values_Data *list, ColumnType *type);
void mode(Values_Data *list, ColumnType *type);
void median(Values_Data *list, ColumnType *type);

#endif
