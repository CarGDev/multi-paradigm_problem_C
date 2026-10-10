#include "data.h"
#include "../utils/delimiter.h"
#include "../utils/validate_data.h"
#include <assert.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

  if (getline(&prev_headers, &headers_capacity, fp) == -1 ||
      getline(&line, &line_capacity, fp) == -1) {
    // empty file, or a header row without any data row
    free(prev_headers);
    free(line);
    fclose(fp);
    return NULL;
  }

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

  size_t line_count = 0;
  while (tokens_lines[line_count] != NULL) {
    line_count++;
  }

  for (size_t i = 0; tokens[i] != NULL; i++) {
    ColumnType valid_input =
        i < line_count ? validate_data(tokens_lines[i]) : TYPE_INVALID;

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
  for (size_t i = 0; i < line_count; i++) {
    free(tokens_lines[i]);
  }
  free(tokens_lines);
  free(line);
  free(prev_headers);

  fclose(fp);
  return headers;
}

void free_headers(Header *headers) {
  if (headers == NULL) {
    return;
  }
  for (size_t i = 0; headers[i].name != NULL; i++) {
    free(headers[i].name);
  }
  free(headers);
}
