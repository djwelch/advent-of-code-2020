#include <assert.h>
#include <errno.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>

int main(void) {
  FILE *file = fopen("./data/day-06", "r");
  if (file == NULL) {
    printf("Unable to open file: %m\n", errno);
    return 1;
  }

  char buffer[32];
  uint32_t questions[32];
  uint32_t questionsbase[32];
  uint32_t questions2[32];
  uint32_t groups = 0;
  uint32_t question_count = 0;
  uint32_t question_count2 = 0;

  for (uint32_t i = 0; i < 32; ++i)
    questions[i] = 0;
  for (uint32_t i = 0; i < 32; ++i)
    questions2[i] = 1;
  for (uint32_t row = 0; fgets(buffer, sizeof(buffer), file) != NULL; ++row) {
    if (buffer[0] == '\n' || buffer[0] == ' ') {
      printf("END\n");
      ++groups;
      for (uint32_t i = 0; i < 32; ++i)
        questions[i] = 0;
      for (uint32_t i = 0; i < 32; ++i) {
        printf("%d: %s\n", i, questions2[i] ? "YES" : "NO");
        if (questions2[i])
          ++question_count2;
        questions2[i] = 1;
      }

      printf("%d group question count\n", question_count2);

    } else {
      if (buffer[0] == '\0')
        break;
      printf("%s\n", buffer);
      for (uint32_t i = 0; i < 32; ++i)
        questionsbase[i] = 0;
      for (char *it = &buffer[0]; *it != '\n' && *it != '\0'; ++it) {
        int32_t question = *it - 'a';
        printf("%c: %d currently %s   %s\n", *it, question,
               questions[question] ? "true" : "false",
               questions2[question] ? "true" : "false");

        assert(question >= 0 && question <= 26);
        if (!questions[question]) {
          ++question_count;
          questions[question] = 1;
        }
        questionsbase[question] = 1;
      }
      for (uint32_t i = 0; i < 32; ++i)
        questions2[i] &= questionsbase[i];
      printf("%d yes\n", question_count);
    }
  }

  printf("there are %d yes answers\n", question_count);
  printf("there are group %d yes answers\n", question_count2);
}

