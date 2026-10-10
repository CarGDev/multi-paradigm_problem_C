#include "main.h"
#include "../core/order_data.h"
#include "../utils/validate_data.h"
#include <stddef.h>
#include <stdio.h>

double summatory(Values_Data *list, ColumnType *type) {
  double sum = 0;
  if (*type == TYPE_INT) {
    for (size_t i = 0; i < list->count; i++) {
      sum += list->values.i[i];
    }
  } else {
    for (size_t i = 0; i < list->count; i++) {
      sum += list->values.d[i];
    }
  }
  printf("Sum: %f\n", sum);
  return sum;
}

/*
 * The mean is the sum of all values divided by the total number of values
 */
void mean(Values_Data *list, ColumnType *type) {
  if (*type == TYPE_INT) {
    double sum = summatory(list, type);
    double m = sum / list->count;
    printf("Mean: %f\n", m);
  } else {
    double sum = summatory(list, type);
    double m = sum / list->count;
    printf("Mean: %f\n", m);
  }
}

static double at(Values_Data *list, ColumnType type, size_t i) {
  return type == TYPE_INT ? (double)list->values.i[i] : list->values.d[i];
}

static void print_value(double v, ColumnType type) {
  if (type == TYPE_INT) {
    printf(" %ld", (long)v);
  } else {
    printf(" %f", v);
  }
}

/*
 * The mode is the most frequent value (or values, when several tie).
 * The list is sorted, so equal values are contiguous: a run is a value.
 */
void mode(Values_Data *list, ColumnType *type) {
  size_t n = list->count;
  if (n == 0) {
    return;
  }

  // longest run of equal values
  size_t max_run = 1;
  for (size_t i = 0; i < n;) {
    size_t j = i;
    while (j < n && at(list, *type, j) == at(list, *type, i)) {
      j++;
    }
    if (j - i > max_run) {
      max_run = j - i;
    }
    i = j;
  }

  if (max_run == 1) {
    printf("Mode: there is no mode\n");
    return;
  }

  printf("Mode:");
  for (size_t i = 0; i < n;) {
    size_t j = i;
    while (j < n && at(list, *type, j) == at(list, *type, i)) {
      j++;
    }
    if (j - i == max_run) {
      print_value(at(list, *type, i), *type);
    }
    i = j;
  }
  printf("\n");
}

/*
 * The median is the middle value when the data is sorted in order
 */
void median(Values_Data *list, ColumnType *type) {
  size_t total_values = list->count;
  if (total_values == 0) {
    return;
  }

  if (*type == TYPE_INT) {
    long isOdd = total_values % 2;
    if (isOdd != 0) {
      long median = list->values.i[total_values / 2];
      printf("Median: %ld\n", median);
    } else {
      double avg = (list->values.i[total_values / 2 - 1] +
                    list->values.i[total_values / 2]) /
                   2.0;
      printf("Median: %f\n", avg);
    }
  } else {
    long isOdd = total_values % 2;
    if (isOdd != 0) {
      double median = list->values.d[total_values / 2];
      printf("Median: %f\n", median);
    } else {
      double avg = (list->values.d[total_values / 2 - 1] +
                    list->values.d[total_values / 2]) /
                   2.0;
      printf("Median: %f\n", avg);
    }
  }
}
