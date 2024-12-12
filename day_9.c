#include "file_reader.h"
#include <ctype.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

typedef short DiskValue_t;

static int lines;
static uint64_t output_value;
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

static int end(int error) {
  uint32_t disk_size = 0;
  uint32_t index_points_used = 0;
  uint32_t index_points_free = 0;

  char *str = input;
  while (*str) {
    disk_size += (*str++ - '0');
    index_points_used++;
    if (*str) {
      disk_size += (*str++ - '0');
      index_points_free++;
    }
  }
  printf("Index %d %d\n", index_points_used, index_points_free);

  DiskValue_t *disk = calloc(sizeof(*disk), disk_size + 1);
  DiskValue_t *disk_2 = calloc(sizeof(*disk_2), disk_size + 1);
  uint32_t *index_used = calloc(sizeof(*index_used), index_points_used);
  uint32_t *index_used_size = calloc(sizeof(*index_used), index_points_used);
  uint32_t *index_free = calloc(sizeof(*index_free), index_points_free);
  uint32_t *index_free_size = calloc(sizeof(*index_free), index_points_free);

  uint32_t file_id = 0;
  uint32_t *iused = index_used;
  uint32_t *iused_size = index_used_size;
  uint32_t *ifree = index_free;
  uint32_t *ifree_size = index_free_size;
  DiskValue_t *dis = disk;
  str = input;
  file_id = 1;
  while (*str) {
    *iused++ = (dis - disk);
    *iused_size++ = *str - '0';
    for (int l = '0'; l < *str; l++) {
      *dis++ = file_id;
    }
    file_id++;
    str++;
    if (*str) {
      *ifree++ = (dis - disk);
      *ifree_size++ = *str - '0';
      for (int l = '0'; l < *str; l++) {
        *dis++ = 0;
      }
      str++;
    }
  }
  memcpy(disk_2, disk, sizeof(*disk_2) * disk_size);
  printf("DiskSize = %d, DiskEndId %d\n", disk_size, file_id);

  // for (int l = 0; l < disk_size; l++) {
  //   printf("%3d ", disk[l]);
  // }
  // printf("\n");
  // for (int l = 0; l < index_points_used; l++) {
  //   printf("%3d ", index_used[l]);
  // }
  // printf("\n");
  // for (int l = 0; l < index_points_free; l++) {
  //   printf("%3d ", index_free[l]);
  // }
  // printf("\n");

  int index_used_pos = index_points_used - 1;
  DiskValue_t *free = disk + index_free[0];
  while (1) {
    DiskValue_t *dis = disk + index_used[index_used_pos--];
    // printf("%d %ld\n", index_used_pos, dis - disk);
    while (*dis) {
      while (*free) {
        free++;
      }
      *free++ = *dis;
      *dis++ = 0;
    }
    // printf("%ld %p %p\n", free - disk, dis, free);
    int l = free - disk;
    for (; l < disk_size; l++) {
      if (disk[l]) {
        break;
      }
    }
    if (l == disk_size)
      break;
  }

  // for (int l = 0; l < disk_size; l++) {
  //   if (!disk[l])
  //     break;
  //   printf("%d", disk[l] - 1);
  // }
  // printf("\n");

  int l = 0;
  for (; l < disk_size; l++) {
    if (!disk[l])
      break;
    output_value += (l * (disk[l] - 1));
  }
  printf("New disk size %d\n", l);

  printf("Part1 %ld\n", output_value);
  output_value = 0;

  index_used_pos = index_points_used - 1;
  while (index_used_pos) {
    int file_size = index_used_size[index_used_pos];
    uint32_t file_pos = index_used[index_used_pos];
    DiskValue_t *dis = disk_2 + file_pos;
    // printf("Fs %d - ", file_size);
    for (int l = 0; l < index_points_free; l++) {
      if (index_free_size[l] >= file_size) {
        printf("%d %d\n", index_free[l], file_pos);
        // if (index_used[index_used_pos] != index_free[l]) {
        if (index_free[l] < file_pos) {
          memcpy(disk_2 + index_free[l], dis, sizeof(*dis) * file_size);
          memset(dis, 0, sizeof(*dis) * file_size);
          index_free_size[l] -= file_size;
          index_free[l] += file_size;
        }
        break;
      }
    }
    // printf("\n");
    index_used_pos--;
  }

  if (0) {
    for (int l = 0; l < disk_size; l++) {
      if (disk_2[l])
        printf("%d", disk_2[l] - 1);
      else
        printf(".");
    }
    printf("\n");
    for (int l = 0; l < index_points_used; l++) {
      printf("%3d ", index_used[l]);
    }
    printf("\n");
    for (int l = 0; l < index_points_free; l++) {
      printf("%3d ", index_free[l]);
    }
    printf("\n");
  }

  l = 0;
  for (; l < disk_size; l++) {
    if (disk_2[l])
      output_value += (l * (disk_2[l] - 1));
  }
  printf("Part2 %ld\n", output_value);

  return 0;
}

void Day_9(const char *filename) {
  printf("DAY9 %s\n", filename);
  read_file(filename, start, line, end);
}
