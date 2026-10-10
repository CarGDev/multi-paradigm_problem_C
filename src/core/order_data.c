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

Values_Data *getting_all_data(Header *headers, int option, char **argv) {
  printf("User has choosen: %s\n", headers[option].name);

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
  }
  fclose(fp);
  return values;
}

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

Values_Data *order(Header *headers, int option, char **argv) {
  Values_Data *list = getting_all_data(headers, option, argv);
  if (list == NULL) {
    exit(1);
  }
  ColumnType type = headers[option].type;
  sorting(list, type);
  return list;
}

void creatingResults(Header *headers, int option, char **argv) {
  Values_Data *list = order(headers, option, argv);
  if (list == NULL) {
    exit(1);
  }
  ColumnType type = headers[option].type;
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
}
