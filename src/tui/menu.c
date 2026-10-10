#include "../core/data.h"
#include <assert.h>
#include <ctype.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int menu(Header *headers) {
  printf("Select one option to return data:\n\n");
  bool ask = true;
  int option;
  while (ask) {
    int j = 0;
    for (size_t i = 0; headers[i].name != NULL; i++) {
      printf("%zu: %s\n", i + 1, headers[i].name);
      j++;
    }
    printf("\nChoose an option: ");
    int value;
    scanf("%d", &value);
    if (value <= j && j > 0) {
      option = value - 1;
      ask = false;
    }
  }
  return option;
}
