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

/*
 * The mode is the most frequent value
 */
void mode(Values_Data *list, ColumnType *type) {
  // current count
  int cc = 1;
  // max count
  int mc = 1;
  // mode
  if (*type == TYPE_INT) {
    long m = list->values.i[0];
    for (size_t i = 1; i < list->count; i++) {
      // current value
      long cv = list->values.i[i];
      if (list->values.i[i - 1] == cv) {
        cc++;
      } else {
        cc = 1;
      }
      if (cc > mc) {
        mc = cc;
        m = cv;
      }
    }
    if (mc == 1) {
      printf("Mode: there is no mode\n");
    }
    printf("Mode: %ld\n", m);
  } else {
    double m = list->values.d[0];
    for (size_t i = 1; i < list->count; i++) {
      // current value
      double cv = list->values.d[i];
      if (list->values.d[i - 1] == cv) {
        cc++;
      } else {
        cc = 1;
      }
      if (cc > mc) {
        mc = cc;
        m = cv;
      }
    }
    if (mc == 1) {
      printf("Mode: there is no mode\n");
    }
    printf("Mode: %f\n", m);
  }
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
      long median = list->values.d[total_values / 2];
      printf("Median: %ld\n", median);
    } else {
      double avg = (list->values.d[total_values / 2 - 1] +
                    list->values.d[total_values / 2]) /
                   2.0;
      printf("Median: %f\n", avg);
    }
  }
}
