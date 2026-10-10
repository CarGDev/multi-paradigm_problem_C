#include "data.h"
#include "../utils/delimiter.h"
#include "../utils/validate_data.h"
#include <_stdio.h>
#include <assert.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/_types/_ssize_t.h>
#include <unistd.h>

Header *csv_headers(int argc, char **argv) {
  if (argc != 2) {
    fprintf(stderr, "Only one argument is allowed");
    exit(1);
  }
  FILE *fp = fopen(argv[1], "r");
  if (fp == NULL) {
    fprintf(stderr, "statictics: Invalid file.\n");
    exit(1);
  }
  char *line = NULL;
  char *prev_headers = NULL;
  Header *headers = NULL;

  size_t line_capacity = 0;
  size_t headers_capacity = 0;

  getline(&prev_headers, &headers_capacity, fp);
  getline(&line, &line_capacity, fp);

  char **tokens = str_split(prev_headers, ',');
  if (!tokens) {
    fprintf(stderr, "statistics: out of memory\n");
    exit(1);
  }
  size_t count = 0;

  while (tokens[count] != NULL) {
    count++;
  }

  headers = malloc((count + 1) * sizeof(*headers));

  if (headers == NULL) {
    fprintf(stderr, "statistics: out of memory\n");
    exit(1);
  }

  char **tokens_lines = str_split(line, ',');
  if (!tokens_lines) {
    fprintf(stderr, "statistics: out of memory\n");
    exit(1);
  }

  size_t j = 0;

  for (size_t i = 0; tokens[i] != NULL; i++) {
    ColumnType valid_input = validate_data(tokens_lines[i]);

    if (valid_input != TYPE_INVALID) {
      headers[j].name = strdup(tokens[i]);
      headers[j].index = i;
      headers[j].type = valid_input;
      j++;
    }

    free(tokens[i]);
  }

  headers[j].name = NULL;
  headers[j].index = 0;

  free(tokens);
  free(line);
  free(prev_headers);

  fclose(fp);
  return headers;
}
