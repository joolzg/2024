#include "file_reader.h"
#include <ctype.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

typedef struct Pages {
  int page1;
  int page2;
} Pages;

typedef struct Updates {
  int *pages;
  int number_of_pages;
  int ok;
} Updates;

static int lines;
static int output_value;
static int input_length;
static Pages *pages;
static int pages_count;
static Updates *updates;
static int update_count;

static int start(int error) {
  lines = 0;
  input_length = 0;

  return 0;
}

static int line(const char *line, int line_length) {
  if (strstr(line, "|")) {
    int page1, page2;
    if (2 == sscanf(line, "%d|%d", &page1, &page2)) {
      pages = realloc(pages, (pages_count + 1) * sizeof(Pages));
      pages[pages_count].page1 = page1;
      pages[pages_count].page2 = page2;
      pages_count++;
    } else {
      printf("Hmm 1:%s\n", line);
    }
  } else {
    updates = realloc(updates, (update_count + 1) * sizeof(Updates));
    updates[update_count].pages = NULL;
    updates[update_count].number_of_pages = 0;
    const char *str = line;
    while (*str) {
      int value;
      if (1 == sscanf(str, "%d", &value)) {
        updates[update_count].pages =
            realloc(updates[update_count].pages,
                    (updates[update_count].number_of_pages + 1) *
                        sizeof(*updates[update_count].pages));
        updates[update_count].pages[updates[update_count].number_of_pages] =
            value;
        updates[update_count].number_of_pages++;
        str += 2;
        if (*str == ',') {
          str++;
        } else if (*str) {
          printf("Hmm2:%s\n", str);
        }
      } else {
        printf("Hmm3:%s\n", str);
      }
    }
    update_count++;
  }
  lines++;

  return 0;
}

static int find_page(int l, int pos, int which_page) {
  //   printf("%4d : ", updates[l].number_of_pages);
  //   for (int m = 0; m < updates[l].number_of_pages; m++) {
  //     if (m == pos + 1) {
  //       printf("_");
  //     }
  //     printf("%d ", updates[l].pages[m]);
  //   }
  //   printf("\n");

  //   for (int p = 0; p < pages_count; p++) {
  //     int cnt = pos + 1;
  //     printf("%3d %3d : ", p, cnt);
  //     while (cnt < updates[l].number_of_pages) {
  //       printf("%3d ", updates[l].pages[cnt]);
  //       cnt++;
  //     }
  //     printf("\n");
  //   }

  int cnt = pos + 1;
  while (cnt < updates[l].number_of_pages) {
    if (updates[l].pages[cnt] == which_page) {
      return cnt;
    }
    cnt++;
  }

  return 0;
}

static int end(int error) {
  for (int l = 0; l < update_count; l++) {
    int found = 0;
    for (int m = 0; !found && (m < updates[l].number_of_pages); m++) {
      for (int p = 0; !found && (p < pages_count); p++) {
        if (updates[l].pages[m] == pages[p].page2) {
          //   printf("%3d %3d %3d : %3d [%3d %3d]\n", l, m, p,
          //   updates[l].pages[m],
          //          pages[p].page1, pages[p].page2);
          if (find_page(l, m, pages[p].page1)) {
            found = 1;
          }
        }
      }
    }
    updates[l].ok = found;
    if (!found) {
      //   for (int ll = 0; ll < updates[l].number_of_pages; ll++) {
      //     printf("%2d ", updates[l].pages[ll]);
      //   }
      //   printf(" ** ");
      int middle = updates[l].number_of_pages / 2;
      //   printf("Found %d %d %d\n", l, middle, updates[l].pages[middle]);
      output_value += updates[l].pages[middle];
    }
  }

  printf("Part1 %d\n", output_value);
  output_value = 0;

  for (int l = 0; l < update_count; l++) {
    if (updates[l].ok) {
      int found;
      do {
        found = 0;
        for (int m = 0; m < updates[l].number_of_pages; m++) {
          for (int p = 0; p < pages_count; p++) {
            if (updates[l].pages[m] == pages[p].page2) {
              int swap = find_page(l, m, pages[p].page1);
              if (swap) {
                found = 1;
                int sw = updates[l].pages[m];
                updates[l].pages[m] = updates[l].pages[swap];
                updates[l].pages[swap] = sw;
              }
            }
          }
        }
      } while (found);
    }
  }
  for (int l = 0; l < update_count; l++) {
    if (updates[l].ok) {
      int middle = updates[l].number_of_pages / 2;
      output_value += updates[l].pages[middle];
    }
  }
  printf("Part2 %d\n", output_value);

  return 0;
}

void Day_5(const char *filename, int day) {
  printf("DAY5%d %s\n", day, filename);
  read_file(filename, start, line, end);
}
