#include "file_reader.h"
#include <ctype.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static int lines;
static int output_value;
static char *input;
static int input_length;
static int input_width;
static char *trail_heads;
static int trails_found;

static int start(int error) {
  lines = 0;
  input = NULL;
  input_length = 0;

  return 0;
}

static int line(const char *line, int line_length) {
  input_width = line_length;
  input = realloc(input, input_length + line_length + 1);
  memcpy(input + input_length, line, line_length);
  input_length += line_length;
  input[input_length] = 0;
  lines++;

  return 0;
}

static int walk(int pos) {
  int x = pos % input_width;
  int y = pos / input_width;

  if (input[pos] == '9') {
    trails_found++;
    trail_heads[pos] = 1;
    return 1;
  }

  if (y > 0) {
    if ((input[pos] + 1) == input[pos - input_width]) {
      walk(pos - input_width);
    }
  }
  if (y < (lines - 1)) {
    if ((input[pos] + 1) == input[pos + input_width]) {
      walk(pos + input_width);
    }
  }
  if (x > 0) {
    if ((input[pos] + 1) == input[pos - 1]) {
      walk(pos - 1);
    }
  }
  if (x < (input_width - 1)) {
    if ((input[pos] + 1) == input[pos + 1]) {
      walk(pos + 1);
    }
  }

  return 0;
}

static int end(int error) {
  trail_heads = calloc(1, input_length + 1);

  for (int pos = 0; pos < input_length; pos++) {
    if (input[pos] == '0') {
      memset(trail_heads, 0, input_length);
      if (walk(pos)) {
      }
      for (int l = 0; l < input_length; l++) {
        output_value += trail_heads[l];
      }
    }
  }
  printf("Part1 %d\n", output_value);
  output_value = 0;

  printf("Part2 %d\n", trails_found);

  return 0;
}

void Day_10(const char *filename, int day) {
  printf("DAY10%d %s\n", day, filename);
  read_file(filename, start, line, end);
}
