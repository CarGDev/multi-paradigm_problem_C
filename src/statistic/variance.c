#include "../core/order_data.h"
#include "../utils/validate_data.h"
#include "main.h"
#include <math.h>
#include <stddef.h>
#include <stdio.h>

static double at(Values_Data *list, ColumnType type, size_t i) {
  return type == TYPE_INT ? (double)list->values.i[i] : list->values.d[i];
}

/*
 * Variance is the average of the squared distances to the mean. Population
 * variance divides by n, sample variance divides by n - 1.
 */
void variance(Values_Data *list, ColumnType *type) {
  size_t n = list->count;
  if (n == 0) {
    return;
  }

  double sum = 0;
  for (size_t i = 0; i < n; i++) {
    sum += at(list, *type, i);
  }
  double avg = sum / (double)n;

  double squares = 0;
  for (size_t i = 0; i < n; i++) {
    double d = at(list, *type, i) - avg;
    squares += d * d;
  }

  double population = squares / (double)n;
  printf("Population variance: %f\n", population);
  printf("Population std dev: %f\n", sqrt(population));

  if (n < 2) {
    printf("Sample variance: needs at least 2 values\n");
    return;
  }
  double sample = squares / (double)(n - 1);
  printf("Sample variance: %f\n", sample);
  printf("Sample std dev: %f\n", sqrt(sample));
}
