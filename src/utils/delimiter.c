#include "delimiter.h"
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

char **str_split(const char *s, char delim) {
  if (s == NULL)
    return NULL;

  size_t count = 1;
  bool in_quotes = false;

  for (const char *p = s; *p; p++) {
    if (*p == '"') {
      if (in_quotes && p[1] == '"') {
        p++;
      } else {
        in_quotes = !in_quotes;
      }
    } else if (*p == delim && !in_quotes) {
      count++;
    }
  }

  if (in_quotes)
    return NULL;

  char **result = malloc((count + 1) * sizeof(char *));
  if (!result)
    return NULL;

  size_t idx = 0;
  const char *p = s;

  while (idx < count) {
    size_t len = 0;
    size_t capacity = strlen(p) + 1;

    char *field = malloc(capacity);
    if (!field)
      goto cleanup;

    in_quotes = false;

    while (*p) {
      if (*p == '"') {
        if (in_quotes && p[1] == '"') {
          field[len++] = '"';
          p += 2;
        } else {
          in_quotes = !in_quotes;
          p++;
        }
      } else if (*p == delim && !in_quotes) {
        p++;
        break;
      } else {
        field[len++] = *p++;
      }
    }

    field[len] = '\0';

    if (idx == count - 1) {
      while (len > 0 && (field[len - 1] == '\n' || field[len - 1] == '\r')) {
        field[--len] = '\0';
      }
    }

    result[idx++] = field;
  }

  result[idx] = NULL;
  return result;

cleanup:
  for (size_t i = 0; i < idx; i++)
    free(result[i]);

  free(result);
  return NULL;
}
