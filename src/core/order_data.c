#include "order_data.h"
#include "../utils/delimiter.h"
#include "../utils/validate_data.h"
#include "data.h"
#include <_stdio.h>
#include <_string.h>
#include <assert.h>
#include <ctype.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/_types/_ssize_t.h>
#include <unistd.h>

#define CHAR_SIZE 4096

Values_Data *order(Header *headers, int option, char **argv) {
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
  if (type == TYPE_INT) {
    printf("User has choosen: %ld\n", values->values.i[option]);
  } else {
    printf("User has choosen: %f\n", values->values.d[option]);
  }
  return values;
}
