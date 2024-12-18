#include "file_reader.h"
#include <assert.h>
#include <ctype.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

typedef struct Number {
  uint64_t total;
  uint64_t *values;
  int values_count;
} Numbers;

static int lines;
static uint64_t output_value;
static Numbers *numbers;
static int numbers_count;
static int show = 0;

static int start(int error) {
  lines = 0;
  numbers = NULL;
  numbers_count = 0;

  return 0;
}

static int line(const char *line, int line_length) {
  while (*line) {
    numbers = realloc(numbers, (numbers_count + 1) * sizeof(Numbers));
    numbers[numbers_count].total = 0;
    numbers[numbers_count].values = NULL;
    numbers[numbers_count].values_count = 0;
    Numbers *num = &numbers[numbers_count];
    numbers_count++;

    while (isdigit(*line)) {
      num->total *= 10UL;
      num->total += (*line - '0');
      line++;
    }
    if (*line == ':') {
      line++;
      while (isspace(*line)) {
        line++;
      }
      while (isdigit(*line)) {
        num->values = realloc(num->values,
                              (num->values_count + 1) * sizeof(num->values[0]));
        num->values[num->values_count] = 0;
        uint64_t last;
        while (isdigit(*line)) {
          last = num->values[num->values_count];
          num->values[num->values_count] *= 10UL;
          num->values[num->values_count] += (*line - '0');
          assert(num->values[num->values_count] >= last);
          line++;
        }
        while (isspace(*line)) {
          line++;
        }
        num->values_count++;
      }
    }
  }
  lines++;

  return 0;
}

static uint64_t process(int number_of_operators) {
  uint64_t output_value = 0;
  Numbers *num = numbers;
  // numbers_count = 1;
  uint8_t *o = calloc(1, 64);
  for (int l = 0; l < numbers_count; l++) {
    memset(o, 0, num->values_count + 1);
    while (!o[num->values_count - 1]) {
      uint64_t total = num->values[0];

      if (show) {
        for (int m = 0; m < num->values_count + 1; m++) {
          printf("%d", o[m]);
        }
        printf(" ");
      }

      for (int m = 1; m < num->values_count; m++) {
        uint32_t t = total;
        switch (o[m - 1]) {
        case 0:
          if (show)
            printf("+(%8ld %3ld) ", total, num->values[m]);
          total += num->values[m];
          break;

        case 1:
          if (show)
            printf("*(%8ld %3ld) ", total, num->values[m]);
          total *= num->values[m];
          break;

        case 2:
          uint64_t mult = 10;
          while (mult <= num->values[m]) {
            mult *= 10;
          }
          if (show)
            printf("|(%8ld %3ld) ", total, num->values[m]);
          total *= mult;
          total += num->values[m];
          break;
        }
        assert(total >= t);
      }
      if (total == num->total) {
        // printf(" *** %ld\n", total);
        output_value += total;
        break;
      }
      if (show)
        printf("%ld\n", total);
      int x = 0;
      while (1) {
        if (++o[x] == number_of_operators) {
          o[x] = 0;
          x++;
        } else {
          break;
        }
      }
    }
    num++;
  }
  free(o);

  return output_value;
}

static int end(int error) {
  // for (int l = 0; l < numbers_count; l++) {
  //   printf("%ld: ", numbers[l].total);
  //   for (int m = 0; m < numbers[l].values_count; m++) {
  //     printf("%ld ", numbers[l].values[m]);
  //   }
  //   printf("\n");
  // }

  // Numbers *num = numbers;
  // for (int l = 0; l < numbers_count; l++) {
  //   int ops = 1 << (num->values_count - 1);
  //   int current_ops = 0;
  //   while (ops) {
  //     uint64_t total = num->values[0];
  //     int mask_ops = 1;
  //     for (int m = 1; m < num->values_count; m++) {
  //       if (current_ops & mask_ops) {
  //         total *= num->values[m];
  //       } else {
  //         total += num->values[m];
  //       }
  //       if (total == num->total) {
  //       }
  //       mask_ops <<= 1;
  //     }
  //     if (total == num->total) {
  //       output_value += total;
  //       break;
  //     }
  //     current_ops++;
  //     ops--;
  //   }
  //   num++;
  // }
  // output_value = process(2);
  // printf("Part1 %ld\n", output_value);

  output_value = process(3);
  printf("Part2 %ld\n", output_value);

  for (int l = 0; l < numbers_count; l++) {
    free(numbers[l].values);
  }
  free(numbers);

  return 0;
}

void Day_7(const char *filename, int day) {
  printf("DAY7%d %s\n", day, filename);
  read_file(filename, start, line, end);
}
