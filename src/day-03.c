#include <assert.h>
#include <errno.h>
#include <inttypes.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define buffer_size 256
_Static_assert(buffer_size <= INT_MAX, "Buffer too big");

int main() {
  FILE *file = fopen("./data/day-03", "r");
  if (file == NULL) {
    printf("Unable to open file: %m\n", errno);
    return 1;
  }
  char *buffer = malloc(sizeof(char) * buffer_size);

  uint64_t trees11 = 0, trees31 = 0, trees51 = 0, trees71 = 0;
  uint64_t trees12 = 0;
  for (uint32_t row = 0; fgets(buffer, buffer_size, file) != NULL; ++row) {
    uint64_t line_length = strlen(buffer);
    trees11 += buffer[(row * 1) % (line_length - 1)] == '#' ? 1 : 0;
    trees31 += buffer[(row * 3) % (line_length - 1)] == '#' ? 1 : 0;
    trees51 += buffer[(row * 5) % (line_length - 1)] == '#' ? 1 : 0;
    trees71 += buffer[(row * 7) % (line_length - 1)] == '#' ? 1 : 0;
    if ((row % 2) == 0) {
      trees12 += buffer[(row * 1) % (line_length - 1)] == '#' ? 1 : 0;
    }
  }
  uint64_t trees = trees11 * trees31 * trees51 * trees71 * trees12;
  printf("there are %li trees on the (3,1) diagonal\n", trees31);
  printf("there are %li trees on the multi diagonal\n", trees);
  fclose(file);
  free(buffer);
}
