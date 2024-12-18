#include "file_reader.h"
#include <ctype.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static int lines;
static int output_value_1;
static int output_value_2;

static int start(int error) {
  lines = 0;

  return 0;
}

static int is_block_valid(int *values, int cnt) {
  int pos = 0;
  int val = values[pos++];
  int direction = values[pos] - val;
  int quit = 1;
  for (; pos < cnt;) {
    int delta = values[pos] - val;
    if (!delta || labs(delta) > 3) {
      quit = 0;
    } else {
      int value = delta * direction;
      // printf( "Pos:%d Val:%d Delta:%d Direction:%d Val:%d\n", pos,
      // values[pos], delta, direction, val);
      if (value < 0) {
        quit = 0;
      }
    }
    val = values[pos++];
  }

  return quit;
}

static int line(const char *line, int line_length) {
  int values[8];
  int cnt = 0;
  int pos = 0;
  int quit = 0;

  while (pos < strlen(line) && !quit) {
    quit = 1;
    while (line[pos] == 32) {
      pos++;
      quit = 0;
    }
    int val = 0;
    while (isdigit(line[pos])) {
      val = (val * 10) + (line[pos] - '0');
      pos++;
      quit = 0;
    }
    if (!quit) {
      values[cnt++] = val;
    }
  }
  // for( int i = 0; i < cnt; i++)
  // {
  //     printf( "%d ", values[i]);
  // }
  // printf( " cnt:%d\n", cnt);
  quit = is_block_valid(values, cnt);
  // printf( "OK %d\n", quit);
  output_value_1 += quit;
  lines++;

  output_value_2 += quit;
  if (!quit) {
    // for( int i = 0; i < cnt; i++)
    // {
    //     printf( "%d ", values[i]);
    // }
    // printf( " cnt:%d\n", cnt);
    for (int i = 0; !quit && (i < cnt); i++) {
      int backup[8];
      int pos = 0;

      for (int j = 0; j < cnt; j++) {
        if (i != j) {
          backup[pos++] = values[j];
        }
      }
      // for( int i = 0; i < pos; i++)
      // {
      //     printf( "%d ", backup[i]);
      // }
      // printf( " cnt:%d\n", pos);
      quit = is_block_valid(backup, pos);
      output_value_2 += quit;
    }
  }

  return 0;
}

static int end(int error) {
  printf("Part1 %d\n", output_value_1);

  printf("Part2 %d\n", output_value_2);

  return 0;
}

void Day_2(const char *filename, int day) {
  printf("DAY2%d %s\n", day, filename);
  read_file(filename, start, line, end);
}
