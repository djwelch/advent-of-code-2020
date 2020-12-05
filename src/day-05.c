#include <assert.h>
#include <errno.h>
#include <inttypes.h>
#include <stdio.h>

#define BYTE_TO_BINARY_PATTERN "%c%c%c%c%c%c%c%c"
#define BYTE_TO_BINARY(byte)                                                   \
  (byte & 0x80 ? '1' : '0'), (byte & 0x40 ? '1' : '0'),                        \
      (byte & 0x20 ? '1' : '0'), (byte & 0x10 ? '1' : '0'),                    \
      (byte & 0x08 ? '1' : '0'), (byte & 0x04 ? '1' : '0'),                    \
      (byte & 0x02 ? '1' : '0'), (byte & 0x01 ? '1' : '0')

int main(void) {
  FILE *file = fopen("./data/day-05", "r");
  if (file == NULL) {
    printf("Unable to open file: %m\n", errno);
    return 1;
  }

  char buffer[32];
  uint8_t row, col;
  uint16_t highest = 0;
  uint8_t seats[1024];
  for (uint32_t row = 0; fgets(buffer, sizeof(buffer), file) != NULL; ++row) {
    if (buffer[0] != 'F' && buffer[0] != 'B')
      break;
    uint8_t h = 127, l = 0;
    uint8_t mask = 128;
    for (int8_t i = 0; i < 7; ++i) {
      mask >>= 1;
      if (buffer[i] == 'F') {
        h -= mask;
      } else {
        l += mask;
      }
    }
    assert(h == l);
    row = h;

    h = 7, l = 0;
    mask = 8;
    for (int8_t i = 7; i < 10; ++i) {
      mask >>= 1;
      if (buffer[i] == 'L') {
        h -= mask;
      } else {
        l += mask;
      }
    }
    assert(h == l);
    col = h;

    seats[row * 8 + col] = 1;
    if (highest < row * 8 + col) {
      highest = row * 8 + col;
    }
  }
  printf("highest seat is %d\n", highest);

  for (int16_t s = 1; s < highest; ++s) {
    if (seats[s - 1] && seats[s + 1] && !seats[s]) {
      printf("myseat is %d\n", s);
    }
  }
}
