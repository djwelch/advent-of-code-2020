#include <assert.h>
#include <errno.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int32_t log = 0;
static int32_t run_program(int32_t program[], int32_t program_length,
                           int32_t *acc) {
  uint32_t exe[1024] = {0};
  int32_t instr = 0;
  int32_t arg = 0;
  for (int32_t p = 0; p != program_length;) {
    if (exe[p])
      break;
    exe[p] = 1;
    instr = program[p];
    arg = (instr & 0xffff);
    if (arg > 0x7fff)
      arg |= 0xffff0000;
    instr = (instr & 0x7fff0000) >> 16;
    // printf("%d %d\n", instr, arg);
    switch (instr) {
    case 'a':
      *acc += arg;
      p += 1;
      break;
    case 'j':
      p += arg;
      assert(p >= 0);
      break;
    case 'n':
      p += 1;
      break;
    case '\0':
      return 0;
    }
    if (p == program_length) {
      printf("terminated at %d\n", p);
      return 1;
    }
  }
  return 0;
}

int main(void) {
  FILE *file = fopen("./data/day-08", "r");
  if (file == NULL) {
    printf("Unable to open file: %m\n", errno);
    return 1;
  }

  char buffer[256];
  int32_t program[1024] = {0};
  int32_t program_length = 0;
  int32_t instr = 0;
  int32_t arg = 0;
  for (uint32_t row = 0; fgets(buffer, 256, file) != NULL; ++row) {
    if (buffer[0] == '\n' || buffer[0] == '\0') break;
    // printf("%s", buffer);
    int32_t state = 0;
    for (char *it = buffer; *it != '\0'; ++it) {
      if (state == 0) {
        instr = *it;
        // printf("%c\n", *it);
        state = 1;
      } else if (state == 1) {
        if (*it == ' ') {
          arg = (int16_t)strtol(it + 1, 0, 10);
          // printf("%d\n", arg);
          break;
        }
      }
    }

    program[program_length] = (instr << 16) | (arg & 0xffff);
    ++program_length;
  }

  int32_t acc1 = 0;
  int32_t t = run_program(program, program_length, &acc1);
  assert(t == 0);

  log = true;
  for (int32_t p = 0; p != program_length; ++p) {
    int32_t oldinstr = program[p];
    int32_t op = ((oldinstr & 0x7fff0000) >> 16) & 0xff;
    // printf("before %c\n", op);
    if (op == 'j') {
      program[p] &= 0x0000ffff;
      program[p] |= ('n' << 16);
    } else if (op == 'n') {
      program[p] &= 0x0000ffff;
      program[p] |= ('j' << 16);
    }
    // printf("after %c\n", (program[p] & 0x7fff0000) >> 16);
    int32_t acc2 = 0;
    if (run_program(program, program_length, &acc2) == 1) {
      printf("part 2: %d\n", acc2);
    }
    program[p] = oldinstr;
    // printf("reset %c\n", (program[p] & 0x7fff0000) >> 16);
  }

  printf("part 1: %d\n", acc1);
}


