#include <assert.h>
#include <ctype.h>
#include <errno.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

static int parse_file(char *buffer, char *buffer_end, int64_t *min,
                      int64_t *max, char *ch, char **password) {
  char *it_end = buffer_end;
  char *it = buffer;
  errno = 0;

  *min = strtol(it, &it_end, 10);
  it = it_end + 1;
  it_end = buffer_end;
  if (errno != 0)
    return 1;
  if (*min == 0)
    return 1;

  *max = strtol(it, &it_end, 10);
  it = it_end + 1;
  it_end = buffer_end;
  if (errno != 0)
    return 1;
  if (*max == 0)
    return 1;
  if (*min >= *max)
    return 1;

  *ch = *it;
  if (!isalpha(*ch))
    return 1;

  ++it;
  if (*it != ':')
    return 1;
  ++it;
  if (*it != ' ')
    return 1;

  *password = it + 1;
  return 0;
}

int main() {
  FILE *file = fopen("./data/day-02", "r");
  if (file == NULL) {
    printf("Unable to open file: %m\n", errno);
    return 1;
  }

  int32_t valid_policy1 = 0;
  int32_t valid_policy2 = 0;
  int32_t buffer_size = 1024;
  char buffer[buffer_size];
  char *buffer_end = buffer + buffer_size;
  while (fgets(buffer, buffer_size, file) != NULL) {
    int64_t min, max;
    char ch;
    char *password = NULL;
    int ret = parse_file(buffer, buffer_end, &min, &max, &ch, &password);
    if (ret) {
      printf("Unable to parse file\n");
      break;
    }
    uint8_t count = 0;
    bool policy2 = false;
    uint8_t i = 1;
    for (char *it = password; it != buffer_end; ++it, ++i) {
      if (*it == '\0' || *it == '\n')
        break;
      if (*it == ch) {
        ++count;
        policy2 = policy2 != ((i == min) || (i == max));
      }
    }

    if (min <= count && count <= max) {
      ++valid_policy1;
    }

    if (policy2) {
      ++valid_policy2;
    }
  }

  printf("%d valid passwords (policy 1)\n", valid_policy1);
  printf("%d valid passwords (policy 2)\n", valid_policy2);

  fclose(file);
  return 0;
}
