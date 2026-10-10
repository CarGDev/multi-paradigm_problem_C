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
 * The p-th percentile (0-100) of the already sorted list, using linear
 * interpolation between the two closest ranks.
 */
double percentile(Values_Data *list, ColumnType *type, double p) {
  size_t n = list->count;
  if (n == 0) {
    return 0;
  }
  double rank = p / 100.0 * (double)(n - 1);
  size_t lo = (size_t)floor(rank);
  size_t hi = lo + 1 < n ? lo + 1 : lo;
  double frac = rank - (double)lo;
  double a = at(list, *type, lo);
  double b = at(list, *type, hi);
  return a + frac * (b - a);
}

/*
 * Quartiles split the sorted data in four parts: Q1 (25%), Q2 (50%, the
 * median) and Q3 (75%). The IQR is Q3 - Q1.
 */
void quartiles(Values_Data *list, ColumnType *type) {
  if (list->count == 0) {
    return;
  }
  double q1 = percentile(list, type, 25);
  double q2 = percentile(list, type, 50);
  double q3 = percentile(list, type, 75);
  printf("Q1: %f\n", q1);
  printf("Q2: %f\n", q2);
  printf("Q3: %f\n", q3);
  printf("IQR: %f\n", q3 - q1);
}
