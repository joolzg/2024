#include "file_reader.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static int lines;
static int output_value;
static long *column_left;
static long *column_right;

static int start(int error) {
  lines = 0;

  return 0;
}

static int line(const char *line, int line_length) {
  lines++;

  return 0;
}

static int end(int error) {
  // printf( "lines %d\n", lines);

  return 0;
}

static int start_real(int error) {
  column_left = calloc(lines, sizeof(*column_left));
  column_right = calloc(lines, sizeof(*column_right));
  lines = 0;

  return 0;
}

static int line_real(const char *line, int line_length) {
  long left, right;

  sscanf(line, "%ld %ld", &left, &right);
  // printf( "%d %ld %ld\n", lines, left, right);
  column_left[lines] = left;
  column_right[lines] = right;
  lines++;

  return 0;
}

static int sort_by_number(const void *left_, const void *right_) {
  long *left = (long *)left_;
  long *right = (long *)right_;

  return *left - *right;
}

static int end_real(int error) {
  qsort(column_left, lines, sizeof(*column_left), sort_by_number);
  qsort(column_right, lines, sizeof(*column_right), sort_by_number);

  for (int i = 0; i < lines; i++) {
    long left = column_left[i];
    long right = column_right[i];

    // printf( "%d %ld %ld\n", i, left, right);
    output_value += labs(left - right);
  }
  printf("Part1 %d\n", output_value);

  output_value = 0;
  int i = 0;
  int j = 0;
  uint32_t next_number = column_left[0];
  while (i < lines) {
    while ((i < lines) && (next_number == column_left[i])) {
      i++;
      j = 0;
      while ((j < lines) && (column_right[j] <= next_number)) {
        output_value += next_number * (column_right[j] == next_number);
        j++;
      }
    }
    // printf("NextNumber %d %d %d %d\n", i, next_number, j, output_value);
    next_number = column_left[i];
  }
  printf("Part2 %d\n", output_value);

  free(column_left);
  free(column_right);

  return 0;
}

void Day_1(const char *filename) {
  printf("DAY1 %s\n", filename);
  read_file(filename, start, line, end);
  read_file(filename, start_real, line_real, end_real);
}
