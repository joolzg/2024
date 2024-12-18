#include "file_reader.h"
#include <ctype.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define MAX(X, Y) ((X) > (Y) ? (X) : (Y))
#define MAX_LOOPS 100

static int lines;
static int output_value;
static char *input;
static int input_length;
static int input_width;

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

static int walk(char *in, int cx, int cy) {
  int output_value;
  int same_value;
  int max_same_value;
  int x = -1;
  int y = -1;

  for (int l = 0; l < input_length; l++) {
    if (in[l] == '^') {
      x = l % input_width;
      y = l / input_width;
      break;
    }
  }

  if ((cx != -1) && (cy != -1)) {
    if ((cx != x) || (cy != y)) {
      in[cx + (cy * input_width)] = '#';
    }
  }

  int xd = 0;
  int yd = -1;
  output_value = 1;
  same_value = 0;
  max_same_value = 0;
  while ((x >= 0) && (x < input_width) && (y >= 0) && (y < lines)) {
    if (in[x + (y * input_width)] == 'X') {
      same_value++;
      max_same_value = MAX(max_same_value, same_value);
      if (same_value == MAX_LOOPS) {
        // printf("Final : %3d %3d %3d %3d %s\n", cx, cy, max_same_value,
        //        same_value, in);
        return 0;
      }
    } else {
      same_value = 0;
    }

    in[x + (y * input_width)] = 'X';

    // printf("Input : %3d %3d %3d %3d %s\n", x, y, xd, yd, in);
    int nx = x + xd;
    int ny = y + yd;
    if ((nx >= 0) && (nx < input_width) && (ny >= 0) && (ny < lines)) {
      if (in[nx + (ny * input_width)] == '#') {
        if (!xd && yd == -1) {
          xd = 1;
          yd = 0;
        } else if (xd == 1 && !yd) {
          xd = 0;
          yd = 1;
        } else if (!xd && yd == 1) {
          xd = -1;
          yd = 0;
        } else if (xd == -1 && !yd) {
          xd = 0;
          yd = -1;
        } else
          break;
      } else {
        switch (in[nx + (ny * input_width)]) {
        case 'X':
          break;

        default:
          output_value++;
          break;
        }
        x = nx;
        y = ny;
      }
    } else {
      break;
    }
  }

  // printf("Final : %3d %3d %3d %3d %s\n", cx, cy, max_same_value, same_value,
  // in);

  return output_value;
}
static int end(int error) {
  char *in = calloc(1, input_length + 1);

  memcpy(in, input, input_length);
  output_value = walk(in, -1, -1);
  printf("Part1 %d\n", output_value);

  output_value = 0;
  for (int l = 0; l < input_length; l++) {
    if (input[l] == '.') {
      memcpy(in, input, input_length);
      int ret = walk(in, l % input_width, l / input_width);
      output_value += !ret;
    }
  }
  printf("Part2 %d\n", output_value);
  free(in);

  return 0;
}

void Day_6(const char *filename, int day) {
  printf("DAY6%d %s\n", day, filename);
  read_file(filename, start, line, end);
}
