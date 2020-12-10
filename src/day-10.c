#include <assert.h>
#include <errno.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int64_t find_max_jolts(int64_t jolts[], int32_t jolts_length) {
  int64_t j = 0;
  for (int32_t i = 0; i != jolts_length; ++i) {
    if (j < jolts[i])
      j = jolts[i];
  }
  return j;
}

static void find_differences(int64_t jolts[], int32_t jolts_length, int64_t j,
                             int64_t *j1, int64_t *j3, int64_t *chain) {
  int64_t _j1 = 0;
  int64_t _j3 = 0;
  for (int32_t i = 0; i != jolts_length; ++i) {
    if (j == (jolts[i] + 1)) {
      _j1 = jolts[i];
      break;
    }
    //    if (j == (jolts[i] + 2))
    //      _j2 = jolts[i];
    if (j == (jolts[i] + 3))
      _j3 = jolts[i];
  }
  if (_j1) {
    *j1 = *j1 + 1;
    *chain = _j1;
    find_differences(jolts, jolts_length, _j1, j1, j3, chain + 1);
  } else if (_j3) {
    *j3 = *j3 + 1;
    *chain = _j3;
    find_differences(jolts, jolts_length, _j3, j1, j3, chain + 1);
  }

  if (j == 1) {
    printf("Got to 1\n");
    *j1 = *j1 + 1;
    *chain = 0;
  }
}

int main(void) {
  FILE *file = fopen("./data/day-10.test", "r");
  if (file == NULL) {
    printf("Unable to open file: %m\n", errno);
    return 1;
  }

  char buffer[256];
  int64_t jolts[1000] = {0};
  int32_t jolts_length = 0;
  for (; fgets(buffer, 256, file) != NULL; ++jolts_length) {
    if (buffer[0] == '\n' || buffer[0] == '\0') break;
    int64_t n = strtol(buffer, 0, 10);
    if (errno) {
      printf("Unable to parse number: %m\n", errno);
      return 1;
    }

    jolts[jolts_length] = n;
  }

  int64_t maxjolts = find_max_jolts(jolts, jolts_length);
  printf("max jolts %ld\n", maxjolts);
  int64_t j1 = 0, j3 = 0;
  int64_t chain[2450] = {0};
  chain[0] = maxjolts + 3;
  find_differences(jolts, jolts_length, maxjolts + 3, &j1, &j3, &chain[1]);
  printf("adapter chain %ld * %ld = %ld\n", j1, j3, j1 * j3);

  int64_t totalways = 1;

  for (int32_t i = 0; i != 2450;) {
    int64_t jolt = chain[i];
    printf("%d: %ld\n", i, jolt);
    ++i;
    if (jolt == 0)
      break;
    int64_t ways = 0;
    while ((jolt - chain[i]) <= 3) {
      printf("%d: %ld - %ld (%ld ways) (%ld)\n", i, jolt, chain[i], ways,
             totalways);
      if (chain[i] == 0)
        break;
      ways += 1;
      ++i;
    }
    while (--ways > 0)
      totalways *= 2;
  }

  printf("%ld total ways\n", totalways);
}


