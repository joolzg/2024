#include "file_reader.h"
#include <stdlib.h>

void Day_12(const char *filename);

int main(int argc, char **argv) {
  switch (argc) {
  case 0:
    exit(-1);
  case 1:
    Day_12("day_12_test_data.txt");
    break;

  case 2:
    Day_12(argv[1]);
    break;
  }

  return 0;
}