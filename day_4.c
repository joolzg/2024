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
static char *XMAS = "XMAS";
static int solution_x[4];
static int solution_y[4];

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

static int search_pattern(int x, int y, const char *string, int xp, int yp) {
  int level = string - XMAS;
  // for( int xp = -1; xp < 2; xp ++)
  {
    // for( int yp = -1; yp < 2; yp++)
    {
      int new_x = x + xp;
      int new_y = y + yp;
      if ((new_x >= 0) && (new_x < input_width) && (new_y >= 0) &&
          (new_y < lines)) {
        // if( xp && yp)
        {
          if (input[new_x + (new_y * input_width)] == *string) {
            solution_x[level] = new_x;
            solution_y[level] = new_y;
            if (*(string + 1)) {
              int ret = search_pattern(new_x, new_y, string + 1, xp, yp);
              return ret;
            } else {
              return 2;
            }
          }
        }
      }
    }
  }

  return 0;
}

static void check_return(int ret) {
  if (ret == 2) {
    // for( int l = 0; l < strlen(XMAS); l++) {
    //     printf( "(%d,%d)", solution_x[l], solution_y[l]);
    // }
    // printf( "\n");
    output_value++;
  }
}

static int end(int error) {
  // printf( "Line %d %d %s\n", input_width, input_length, input);

  for (int x = 0; x < input_width; x++) {
    for (int y = 0; y < lines; y++) {
      if (input[x + (y * input_width)] == *XMAS) {
        int ret;
        solution_x[0] = x;
        solution_y[0] = y;
        ret = search_pattern(x, y, XMAS + 1, -1, -1);
        check_return(ret);
        ret = search_pattern(x, y, XMAS + 1, -1, 0);
        check_return(ret);
        ret = search_pattern(x, y, XMAS + 1, -1, 1);
        check_return(ret);
        ret = search_pattern(x, y, XMAS + 1, 0, -1);
        check_return(ret);
        ret = search_pattern(x, y, XMAS + 1, 0, 0);
        check_return(ret);
        ret = search_pattern(x, y, XMAS + 1, 0, 1);
        check_return(ret);
        ret = search_pattern(x, y, XMAS + 1, 1, -1);
        check_return(ret);
        ret = search_pattern(x, y, XMAS + 1, 1, 0);
        check_return(ret);
        ret = search_pattern(x, y, XMAS + 1, 1, 1);
        check_return(ret);
      }
    }
  }
  // printf( "-->%s\n", input);
  // char *str = input;
  // for( int l = 0; l < lines; l++)
  // {
  //     for( int m = 0; m < input_width; m++)
  //     {
  //         printf( "%c", *str++);
  //     }
  //     printf("\n");
  // }
  printf("Part1 %d\n", output_value);
  output_value = 0;

  for (int x = 1; x < input_width - 1; x++) {
    for (int y = 1; y < lines - 1; y++) {
      if (input[x + (y * input_width)] == 'A') {
        char m1m1 = input[(x - 1) + ((y - 1) * input_width)];
        char p1m1 = input[(x + 1) + ((y - 1) * input_width)];
        char m1p1 = input[(x - 1) + ((y + 1) * input_width)];
        char p1p1 = input[(x + 1) + ((y + 1) * input_width)];

        int ok1 = (m1m1 == 'M') && (p1p1 == 'S');
        int ok2 = (m1m1 == 'S') && (p1p1 == 'M');
        int ok3 = (p1m1 == 'M') && (m1p1 == 'S');
        int ok4 = (p1m1 == 'S') && (m1p1 == 'M');

        output_value += (ok1 || ok2) && (ok3 || ok4);

        // printf( "A %d %d %c %c %c %c %d %d %d %d\n", x, y, m1m1, p1m1, m1p1,
        // p1p1, ok1, ok2, ok3, ok4);
      }
    }
  }
  printf("Part2 %d\n", output_value);

  return 0;
}

void Day_4(const char *filename, int day) {
  printf("DAY4%d %s\n", day, filename);
  read_file(filename, start, line, end);
}
