#include "file_reader.h"
#include <ctype.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static int lines;
static int output_value;
static char *input;
static int input_length;

static int start(int error) {
  lines = 0;
  input = NULL;
  input_length = 0;

  return 0;
}

static int line(const char *line, int line_length) {
  input = realloc(input, input_length + line_length + 1);
  memcpy(input + input_length, line, line_length);
  input_length += line_length;
  input[input_length] = 0;
  lines++;

  return 0;
}

static int end(int error) {
  char *pos = strstr(input, "mul(");
  while (pos) {
    int values[2] = {0, 0};
    int value_count = 0;
    char *str = pos + 4;
    while (isdigit(*str) || (*str == ',')) {
      // printf( "Values:[%d %d], ValueCount:%d String:%s\n", values[0],
      // values[1], value_count, str);
      if (isdigit(*str)) {
        if (value_count < 2) {
          values[value_count] *= 10;
          values[value_count] += (*str - '0');
        }
      } else {
        value_count++;
      }
      str++;
    }
    if ((*str == ')') && (value_count == 1)) {
      output_value += (values[0] * values[1]);
    }
    pos = strstr(pos + 1, "mul(");
  }
  printf("Part1 %d\n", output_value);

  int enabled = 1;
  output_value = 0;
  pos = input;
  while (*pos) {
    if ((strlen(pos) > 4) && !memcmp(pos, "mul(", 4)) {
      int values[2] = {0, 0};
      int value_count = 0;
      char *str = pos + 4;
      while (isdigit(*str) || (*str == ',')) {
        if (isdigit(*str)) {
          if (value_count < 2) {
            values[value_count] *= 10;
            values[value_count] += (*str - '0');
          }
        } else {
          value_count++;
        }
        str++;
      }
      if ((*str == ')') && (value_count == 1)) {
        if (enabled) {
          output_value += (values[0] * values[1]);
        }
      }
      pos = str;
    } else if ((strlen(pos) > 7) && !memcmp(pos, "don't()", 7)) {
      pos += 5;
      enabled = 0;
    } else if ((strlen(pos) > 4) && !memcmp(pos, "do()", 4)) {
      pos += 2;
      enabled = 1;
    } else {
      pos++;
    }
  }
  printf("Part2 %d\n", output_value);

  return 0;
}

void Day_3(const char *filename) {
  printf("DAY3 %s\n", filename);
  read_file(filename, start, line, end);
}
