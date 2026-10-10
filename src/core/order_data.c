#include "order_data.h"
#include "../statistic/main.h"
#include "../utils/delimiter.h"
#include "../utils/validate_data.h"
#include "data.h"
#include <assert.h>
#include <ctype.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define CHAR_SIZE 4096

/*
 * Reads every row of the CSV in argv[1] and collects the values of the chosen
 * column into an array of long or double, depending on the column type. Cells
 * that are not valid numbers are skipped. The caller owns the result and must
 * free values.i (the union shares storage with values.d) and the struct.
 * Returns NULL if memory runs out.
 */
Values_Data *getting_all_data(Header *headers, int option, char **argv) {
  FILE *fp = fopen(argv[1], "r");
  if (!fp) {
    perror("fopen");
    exit(1);
  }

  ColumnType type = headers[option].type;
  size_t column_index = headers[option].index;
  Values_Data *values = calloc(1, sizeof(*values));
  if (!values) {
    fclose(fp);
    exit(1);
  }

  values->type = type;
  size_t cap = 0;

  char str[CHAR_SIZE];
  fgets(str, CHAR_SIZE, fp);

  while (fgets(str, CHAR_SIZE, fp) != NULL) {
    char **tokens = str_split(str, ',');
    if (!tokens) {
      break;
    }
    size_t token_count = 0;
    while (tokens[token_count] != NULL) {
      token_count++;
    }
    if (column_index < token_count) {
      char *text = tokens[column_index];
      char *end;

      if (type == TYPE_INT) {
        long v = strtol(text, &end, 10);
        if (end != text && *end == '\0') {
          if (values->count == cap) {
            size_t new_cap = cap ? cap * 2 : 64;
            long *tmp = realloc(values->values.i, new_cap * sizeof(*tmp));
            if (!tmp) {
              for (size_t i = 0; tokens[i]; i++) {
                free(tokens[i]);
              }
              free(tokens);
              fclose(fp);
              free(values->values.i);
              free(values);
              return NULL;
            }
            values->values.i = tmp;
            cap = new_cap;
          }

          values->values.i[values->count++] = v;
        }
      } else {
        double v = strtod(text, &end);
        if (end != text && *end == '\0') {
          if (values->count == cap) {
            size_t new_cap = cap ? cap * 2 : 64;
            double *tmp = realloc(values->values.d, new_cap * sizeof(*tmp));
            if (!tmp) {
              for (size_t i = 0; tokens[i]; i++) {
                free(tokens[i]);
              }
              free(tokens);
              fclose(fp);
              free(values->values.i);
              free(values);
              return NULL;
            }
            values->values.d = tmp;
            cap = new_cap;
          }
          values->values.d[values->count++] = v;
        }
      }
    }
    for (size_t i = 0; tokens[i]; i++) {
      free(tokens[i]);
    }
    free(tokens);
  }
  fclose(fp);
  return values;
}

/*
 * Sift-down step of heap sort for long: restores the max-heap property for the
 * subtree rooted at index i, considering only the first n elements.
 */
void heaping_long(long *arr, int n, int i) {
  int largest = i;
  int l = 2 * i + 1;
  int r = 2 * i + 2;

  if (l < n && arr[l] > arr[largest])
    largest = l;

  if (r < n && arr[r] > arr[largest])
    largest = r;

  if (largest != i) {
    long temp = arr[i];
    arr[i] = arr[largest];
    arr[largest] = temp;

    heaping_long(arr, n, largest);
  }
}

/*
 * Sift-down step of heap sort for double: restores the max-heap property for
 * the subtree rooted at index i, considering only the first n elements.
 */
void heaping_double(double *arr, int n, int i) {
  int largest = i;
  int l = 2 * i + 1;
  int r = 2 * i + 2;

  if (l < n && arr[l] > arr[largest])
    largest = l;

  if (r < n && arr[r] > arr[largest])
    largest = r;

  if (largest != i) {
    double temp = arr[i];
    arr[i] = arr[largest];
    arr[largest] = temp;

    heaping_double(arr, n, largest);
  }
}

/*
 * Sorts the values of the list in ascending order, in place, with heap sort.
 * The sorted order is what median, mode and quartiles rely on.
 */
void sorting(Values_Data *list, ColumnType type) {
  int n = (int)list->count;
  if (type == TYPE_INT) {
    long *values = list->values.i;
    for (int i = n / 2 - 1; i >= 0; i--)
      heaping_long(values, n, i);

    for (int i = n - 1; i > 0; i--) {
      long temp = values[0];
      values[0] = values[i];
      values[i] = temp;

      heaping_long(values, i, 0);
    }
  } else {
    double *values = list->values.d;
    for (int i = n / 2 - 1; i >= 0; i--)
      heaping_double(values, n, i);

    for (int i = n - 1; i > 0; i--) {
      double temp = values[0];
      values[0] = values[i];
      values[i] = temp;

      heaping_double(values, i, 0);
    }
  }
}

/*
 * Loads the chosen column and returns it sorted in ascending order. Exits if
 * the data cannot be loaded.
 */
Values_Data *order(Header *headers, int option, char **argv) {
  Values_Data *list = getting_all_data(headers, option, argv);
  if (list == NULL) {
    exit(1);
  }
  ColumnType type = headers[option].type;
  sorting(list, type);
  return list;
}

/*
 * Prints all the statistics of the chosen column (median, sum, mean, mode,
 * quartiles, variance, min, max and range) and frees the loaded data.
 */
void creatingResults(Header *headers, int option, char **argv) {
  Values_Data *list = order(headers, option, argv);
  if (list == NULL) {
    exit(1);
  }
  ColumnType type = headers[option].type;
  if (list->count == 0) {
    fprintf(stderr, "statistics: the column has no valid values\n");
    free(list->values.i);
    free(list);
    return;
  }
  median(list, &type);
  mean(list, &type);
  mode(list, &type);
  quartiles(list, &type);
  variance(list, &type);
  if (type == TYPE_INT) {
    long min = list->values.i[0];
    long max = list->values.i[list->count - 1];
    double range = max - min;
    printf("Min: %ld\n", min);
    printf("Max: %ld\n", max);
    printf("Range: %f\n", range);
  } else {
    double min = list->values.d[0];
    double max = list->values.d[list->count - 1];
    double range = max - min;
    printf("Min: %f\n", min);
    printf("Max: %f\n", max);
    printf("Range: %f\n", range);
  }
  // values.i and values.d share the same union storage, free it once
  free(list->values.i);
  free(list);
}
