#include <assert.h>
#include <errno.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int64_t find_invalid_number(int64_t encrypted[],
                                   int32_t encrypted_length) {
  for (int32_t i = 25; i != encrypted_length; ++i) {
    int64_t target = encrypted[i];
    int32_t found = 0;
    for (int32_t j = i - 25; (i - 1) != j; ++j) {
      if (target < encrypted[j])
        continue;
      int64_t t = target - encrypted[j];
      assert(t > 0);
      for (int32_t k = j + 1; i != k; ++k) {
        if (encrypted[k] == t) {
          found = 1;
          break;
        }
      }
      if (found) {
        break;
      }
    }
    if (!found) {
      return target;
    }
  }
  return 0;
}

static int64_t find_encryption_weakness(int64_t encrypted[],
                                        int32_t encrypted_length,
                                        int64_t target) {
  for (int32_t buffer_length = 2; buffer_length != encrypted_length - 1;
       ++buffer_length) {
    for (int32_t i = 0; i != (encrypted_length - buffer_length); ++i) {
      int64_t sum = 0;
      int64_t min = INT64_MAX, max = 0;
      for (int32_t j = 0; j != buffer_length; ++j) {
        int64_t n = encrypted[i + j];
        if (min > n)
          min = n;
        if (max < n)
          max = n;
        sum += n;
      }
      if (sum == target) {
        return min + max;
      }
    }
  }
  return 0;
}

int main(void) {
  FILE *file = fopen("./data/day-09", "r");
  if (file == NULL) {
    printf("Unable to open file: %m\n", errno);
    return 1;
  }

  char buffer[256];
  int64_t encrypted[1000] = {0};
  int32_t encrypted_length = 0;
  for (; fgets(buffer, 256, file) != NULL; ++encrypted_length) {
    if (buffer[0] == '\n' || buffer[0] == '\0') break;
    int64_t n = strtol(buffer, 0, 10);
    if (errno) {
      printf("Unable to parse number: %m\n", errno);
      return 1;
    }

    encrypted[encrypted_length] = n;
  }

  int64_t n = find_invalid_number(encrypted, encrypted_length);
  int64_t w = find_encryption_weakness(encrypted, encrypted_length, n);
  printf("Invalid number is %ld\n", n);
  printf("Encryption weakness %ld\n", w);
}


