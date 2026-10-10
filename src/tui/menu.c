#include "../core/data.h"
#include <assert.h>
#include <ctype.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * Shows the available columns and asks for one. Returns the zero-based
 * position of the chosen header, or -1 when there is nothing to choose or
 * the input ended.
 */
int menu(Header *headers) {
  size_t total = 0;
  while (headers[total].name != NULL) {
    total++;
  }
  if (total == 0) {
    return -1;
  }

  printf("Select one option to return data:\n\n");
  for (size_t i = 0; i < total; i++) {
    printf("%zu: %s\n", i + 1, headers[i].name);
  }

  char buffer[128];
  while (true) {
    printf("\nChoose an option: ");
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
      return -1;
    }

    char *end;
    long value = strtol(buffer, &end, 10);
    bool has_digits = end != buffer;
    while (isspace((unsigned char)*end)) {
      end++;
    }

    if (has_digits && *end == '\0' && value >= 1 && (size_t)value <= total) {
      return (int)value - 1;
    }
    printf("Invalid option, enter a number between 1 and %zu.\n", total);
  }
}
