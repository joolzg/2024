#include "file_reader.h"
#include <assert.h>
#include <ctype.h>
#include <math.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

typedef uint64_t Stone_t;
#define ALLOC_HASH (4096 * 1)

typedef struct HashMap {
  Stone_t cnt;
  Stone_t value;
} HashMap;

static int lines;
static uint64_t output_value;
static char *input;
static int input_length;
static int input_width;

static HashMap hashmap[ALLOC_HASH];
static HashMap hashmap2[ALLOC_HASH];

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

static HashMap *hash_insert(Stone_t val, HashMap *hashmap) {
  uint32_t pos = val;

  while (1) {
    pos %= ALLOC_HASH;
    if (!hashmap[pos].cnt) {
      hashmap[pos].value = val;
      hashmap[pos].cnt = 1;
      return &hashmap[pos];
    } else if (hashmap[pos].cnt && (hashmap[pos].value == val)) {
      hashmap[pos].cnt++;
      return &hashmap[pos];
    } else {
      pos++;
      pos *= 69069;
    }
  }
}

static int end(int error) {
  uint64_t number_of_stones = 0;
  int pos = 0;
  int quit = 0;

  while (pos < strlen(input) && !quit) {
    quit = 1;
    while (input[pos] == 32) {
      pos++;
      quit = 0;
    }
    uint64_t val = 0;
    while (isdigit(input[pos])) {
      val = (val * 10) + (input[pos] - '0');
      pos++;
      quit = 0;
    }
    if (!quit) {
      hash_insert(val, &hashmap[0]);
      number_of_stones++;
    }
  }

  for (int rounds = 0; rounds < 75; rounds++) {
    memset(hashmap2, 0, sizeof(hashmap2));
    for (int l = 0; l < ALLOC_HASH; l++) {
      if (hashmap[l].cnt) {
        Stone_t stone = hashmap[l].value;
        if (stone == 0) {
          hash_insert(1, &hashmap2[0])->cnt += hashmap[l].cnt - 1;
        } else {
          Stone_t cnt = 0;
          Stone_t val = 1;
          while (val <= stone) {
            cnt++;
            val *= 10;
          }
          if (!(cnt & 1)) {
            Stone_t val0 = pow(10, cnt / 2);
            Stone_t val1 = stone / val0;
            hash_insert(val1, &hashmap2[0])->cnt += hashmap[l].cnt - 1;
            Stone_t val2 = stone % val0;
            hash_insert(val2, &hashmap2[0])->cnt += hashmap[l].cnt - 1;
          } else {
            hash_insert(stone * 2024, &hashmap2[0])->cnt += hashmap[l].cnt - 1;
          }
        }
      }
    }
    memcpy(hashmap, hashmap2, sizeof(hashmap));
    output_value = 0;
    int in_use = 0;
    for (int l = 0; l < ALLOC_HASH; l++) {
      output_value += hashmap[l].cnt;
      in_use += !!hashmap[l].cnt;
    }
    if (rounds == 24 || rounds >= 74) {
      printf("Day %d : %ld %d\n", rounds + 1, output_value, in_use);
    }
  }

  return 0;
}

void Day_11(const char *filename) {
  printf("DAY11 %s\n", filename);
  read_file(filename, start, line, end);
}
