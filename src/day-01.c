#include <ctype.h>
#include <errno.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

static bool is_space_or_end(char ch) { return ch == '\0' || isspace(ch); }

static size_t read_numbers(size_t numbers_size,
                           int64_t numbers[static numbers_size], FILE *file) {

  int32_t buffer_size = 1024;
  char buffer[buffer_size];
  size_t numbers_read = 0;
  while (fgets(buffer, buffer_size, file) != NULL) {
    if (numbers_read == numbers_size) {
      printf("Buffer overflow parsing numbers\n");
      return 0;
    }
    char *buffer_end = NULL;
    errno = 0;
    numbers[numbers_read] = strtol(buffer, &buffer_end, 10);
    if (errno != 0) {
      printf("%m\n", errno);
      return 0;
    }
    if (buffer_end == buffer && is_space_or_end(buffer_end[0]))
      continue;
    if (!is_space_or_end(buffer_end[0])) {
      printf("Unable to parse numbers\n");
      return 0;
    }
    ++numbers_read;
  }
  return numbers_read;
}

int main() {
  FILE *file = fopen("./data/day-01", "r");
  if (file == NULL) {
    printf("Unable to open file: %m\n", errno);
    return 1;
  }

  size_t numbers_size = 200;
  int64_t *numbers = malloc(sizeof(int64_t) * numbers_size);
  size_t numbers_read = read_numbers(numbers_size, numbers, file);

  for (size_t i = 0; i != numbers_read; ++i) {
    int64_t n1 = numbers[i];
    if (n1 > 2020)
      continue;
    for (size_t j = 0; j != numbers_read; ++j) {
      int64_t n2 = numbers[j];
      if ((n1 + n2) > 2020)
        continue;
      if (n1 + n2 == 2020) {
        printf("%li * %li = %li\n", n1, n2, n1 * n2);
      }
      for (size_t k = 0; k != numbers_read; ++k) {
        int64_t n3 = numbers[k];
        if (n1 + n2 + n3 == 2020) {
          printf("%li * %li * %li = %li\n", n1, n2, n3, n1 * n2 * n3);
        }
      }
    }
  }

  free(numbers);
  fclose(file);
  return 0;
}
