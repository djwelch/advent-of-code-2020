#include <assert.h>
#include <ctype.h>
#include <errno.h>
#include <inttypes.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define buffer_size 1024
_Static_assert(buffer_size <= INT_MAX, "Buffer too big");

int main() {
  FILE *file = fopen("./data/day-04", "r");
  if (file == NULL) {
    printf("Unable to open file: %m\n", errno);
    return 1;
  }
  char *buffer = malloc(sizeof(char) * buffer_size);
  uint8_t key_index = 0, key_size = 32;
  char key[key_size];
  uint8_t value_index = 0, value_size = 32;
  char value[value_size];
  uint32_t passports_with_required_keys = 0;
  uint32_t valid_passports = 0;
  uint32_t present_keys = 0;
  uint32_t valid_values = 0;

  size_t bytes_read;
  int8_t state = 0;
  while ((bytes_read = fread(buffer, sizeof(char), buffer_size, file)) != 0) {
    for (char *it = &buffer[0]; it != &buffer[bytes_read]; ++it) {
      char ch = *it;
      switch (state) {
      case 0:
      case 1:
        if (isspace(ch)) {
          assert(state == 0);
          if (present_keys == 7) {
            ++passports_with_required_keys;
            if (valid_values == 7) {
              ++valid_passports;
            }
          }
          printf("got %d keys in passport with %d valid values\n", present_keys,
                 valid_values);
          present_keys = 0;
          valid_values = 0;
        } else if (ch == ':') {
          assert(state == 1);
          state = 2;
          key[key_index] = '\0';
          // printf("%s\n", key);
          if (strcmp(key, "cid") != 0) {
            ++present_keys;
          }
          key_index = 0;
        } else if (isalpha(ch)) {
          state = 1;
          key[key_index++] = ch;
          assert(key_index != key_size);
        }
        break;
      case 2:
        if (isspace(ch)) {
          state = 0;
          value[value_index] = '\0';
          // printf("%s\n", value);

          errno = 0;
          if (strcmp(key, "byr") == 0) {
            char *value_end = &value[value_index];
            long byr = strtol(value, &value_end, 10);
            if (1920 <= byr && byr <= 2002 && errno == 0) {
              ++valid_values;
            } else {
              printf("byr:%s invalid\n", value);
            }
          }

          if (strcmp(key, "iyr") == 0) {
            char *value_end = &value[value_index];
            long iyr = strtol(value, &value_end, 10);
            if (2010 <= iyr && iyr <= 2020 && errno == 0) {
              ++valid_values;
            } else {
              printf("iyr:%s invalid\n", value);
            }
          }

          if (strcmp(key, "eyr") == 0) {
            char *value_end = &value[value_index];
            long eyr = strtol(value, &value_end, 10);
            if (2020 <= eyr && eyr <= 2030 && errno == 0) {
              ++valid_values;
            } else {
              printf("eyr:%s invalid\n", value);
            }
          }

          if (strcmp(key, "hgt") == 0) {
            char *value_end = &value[value_index] - 2;
            long hgt = strtol(value, &value_end, 10);
            if (strcmp(value_end, "cm") == 0) {
              if (150 <= hgt && hgt <= 193 && errno == 0) {
                ++valid_values;
              } else {
                printf("hgt:%s invalid\n", value);
              }
            } else if (strcmp(value_end, "in") == 0) {
              if (59 <= hgt && hgt <= 76 && errno == 0) {
                ++valid_values;
              } else {
                printf("hgt:%s invalid\n", value);
              }
            } else {
              printf("hgt:%s invalid\n", value);
            }
          }

          if (strcmp(key, "hcl") == 0) {
            char *value_end = &value[value_index];
            if (value[0] == '#') {
              strtol(value + 1, &value_end, 16);
              if (value_end == &value[value_index]) {
                ++valid_values;
                printf("hcl:%s valid\n", value);
              } else {
                printf("hcl:%s invalid\n", value);
              }
            } else {
              printf("hcl:%s invalid\n", value);
            }
          }

          if (strcmp(key, "ecl") == 0) {
            if (strcmp(value, "amb") == 0 || strcmp(value, "blu") == 0 ||
                strcmp(value, "brn") == 0 || strcmp(value, "gry") == 0 ||
                strcmp(value, "grn") == 0 || strcmp(value, "hzl") == 0 ||
                strcmp(value, "oth") == 0) {
              ++valid_values;
            } else {
              printf("ecl:%s invalid\n", value);
            }
          }

          if (strcmp(key, "pid") == 0) {
            bool alldigits = true;
            uint8_t digitcount = 0;
            for (char *x = value; x != &value[value_index]; ++x) {
              alldigits = isdigit(*x);
              if (alldigits) {
                ++digitcount;
              } else {
                break;
              }
            }
            if (alldigits && digitcount == 9) {
              ++valid_values;
            } else {
              printf("pid:%s invalid\n", value);
            }
          }

          value_index = 0;
        } else {
          value[value_index++] = ch;
          assert(value_index != value_size);
        }
        break;
      }
    }
  }

  printf("%d passports with all required keys\n", passports_with_required_keys);
  printf("%d valid passports\n", valid_passports);

  fclose(file);
  free(buffer);
}
