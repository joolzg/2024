#include "file_reader.h"
#include <ctype.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

typedef struct Pairs {
  int x1;
  int y1;
  int x2;
  int y2;
} Pairs;

static int lines;
static int output_value;
static char *input;
static int input_length;
static int input_width;
static Pairs *pairs;
static int pairs_number;

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

static void dump_map(const char *title, char *map) {
  printf("%-6s\n", title);
  for (int y = 0; y < lines; y++) {
    for (int x = 0; x < input_width; x++) {
      printf("%c", *map++);
    }
    printf("\n");
  }
}

static void add_pair(int x1, int y1, int x2, int y2) {
  for (int l = 0; l < pairs_number; l++) {
    if ((pairs[l].x1 == x1) && (pairs[l].y1 == y1) && (pairs[l].x2 == x2) &&
        (pairs[l].y2 == y2)) {
      printf("Pair already found\n");
      return;
    }
    if ((pairs[l].x1 == x2) && (pairs[l].y1 == y2) && (pairs[l].x2 == x1) &&
        (pairs[l].y2 == y1)) {
      printf("Pair already found\n");
      return;
    }
  }
  pairs = realloc(pairs, (pairs_number + 1) * sizeof(Pairs));
  pairs[pairs_number].x1 = x1;
  pairs[pairs_number].y1 = y1;
  pairs[pairs_number].x2 = x2;
  pairs[pairs_number].y2 = y2;
  pairs_number++;
  printf("Pair added %d %d %d %d\n", x1, y1, x2, y2);
}

static int end(int error) {
  printf("Input : %s\n", input);

  char *map = calloc(1, input_length + 1);
  char *anti = calloc(1, input_length + 1);

  memset(map, '.', input_length);
  memset(anti, '.', input_length);

  int pos = 0;
  while (pos < input_length) {
    int x = pos % input_width;
    int y = pos / input_width;

    if (input[pos] != '.') {
      // printf("Init : %d %d %c\n", x, y, input[pos]);
      int npos = pos;
      while (npos < input_length) {
        int nx = npos % input_width;
        int ny = npos / input_width;
        if ((x != nx) && (y != ny)) {
          if (input[npos] == input[pos]) {
            add_pair(x, y, nx, ny);
          }
        }
        npos++;
      }
      // for (int ny = 0; ny < lines; ny++) {
      //   for (int nx = 0; nx < input_width; nx++) {
      //     if ((x != nx) && (y != ny)) {
      //       if (input[npos] == input[pos]) {
      //         add_pair(x, y, nx, ny);
      //       }
      //     }
      //     npos++;
      //   }
      // }
    }
    pos++;
  }

  Pairs *p = pairs;
  for (int l = 0; l < pairs_number; l++, p++) {
    int x = p->x1;
    int y = p->y1;
    int nx = p->x2;
    int ny = p->y2;
    int dx = nx - x;
    int dy = ny - y;

    printf("Srch : X:%d Y:%d NX:%d NY:%d DX:%d DY:%d\n", x, y, nx, ny, dx, dy);

    int one_dx = x + (dx + dx);
    int one_dy = y + (dy + dy);
    printf("Fix1 : X:%d Y:%d\n", one_dx, one_dy);
    if ((one_dx >= 0) && (one_dx < input_width) && (one_dy >= 0) &&
        (one_dy < lines)) {
      int one = one_dx + (one_dy * input_width);
      output_value += (anti[one] == '.');
      anti[one] = '#';
    }
    int two_dx = nx - (dx + dx);
    int two_dy = ny - (dy + dy);
    printf("Fix2 : X:%d Y:%d\n", two_dx, two_dy);
    if ((two_dx >= 0) && (two_dx < input_width) && (two_dy >= 0) &&
        (two_dy < lines)) {
      int two = two_dx + (two_dy * input_width);
      output_value += (anti[two] == '.');
      anti[two] = '#';
    }
  }

  dump_map("Anti", anti);
  printf("Part1 %d\n", output_value);

  p = pairs;
  for (int l = 0; l < pairs_number; l++, p++) {
    int x = p->x1;
    int y = p->y1;
    int nx = p->x2;
    int ny = p->y2;
    int dx = nx - x;
    int dy = ny - y;

    printf("Srch : X:%d Y:%d NX:%d NY:%d DX:%d DY:%d\n", x, y, nx, ny, dx, dy);

    while (1) {
      int one_dx = x + dx;
      int one_dy = y + dy;
      if ((one_dx >= 0) && (one_dx < input_width) && (one_dy >= 0) &&
          (one_dy < lines)) {
        printf("Fix1 : (%d %d) X:%d Y:%d\n", x, y, one_dx, one_dy);
        int one = one_dx + (one_dy * input_width);
        output_value += (anti[one] == '.');
        anti[one] = '#';
      } else {
        break;
      }
      x += dx;
      y += dy;
    }

    while (1) {
      int two_dx = nx - dx;
      int two_dy = ny - dy;
      if ((two_dx >= 0) && (two_dx < input_width) && (two_dy >= 0) &&
          (two_dy < lines)) {
        printf("Fix2 : (%d %d) X:%d Y:%d\n", nx, ny, two_dx, two_dy);
        int two = two_dx + (two_dy * input_width);
        output_value += (anti[two] == '.');
        anti[two] = '#';
      } else {
        break;
      }
      nx -= dx;
      ny -= dy;
    }
  }

  dump_map("Anti", anti);
  printf("Part2 %d\n", output_value);

  return 0;
}

void Day_8(const char *filename) {
  printf("DAY8 %s\n", filename);
  read_file(filename, start, line, end);
}
