#include "file_reader.h"
#include <stdlib.h>
#include <string.h>

extern void Day_1(const char *filename, int day);
extern void Day_2(const char *filename, int day);
extern void Day_3(const char *filename, int day);
extern void Day_4(const char *filename, int day);
extern void Day_5(const char *filename, int day);
extern void Day_6(const char *filename, int day);
extern void Day_7(const char *filename, int day);
extern void Day_8(const char *filename, int day);
extern void Day_9(const char *filename, int day);
extern void Day_10(const char *filename, int day);
extern void Day_11(const char *filename, int day);
extern void Day_12(const char *filename, int day);
extern void Day_13(const char *filename, int day);
extern void Day_14(const char *filename, int day);
extern void Day_15(const char *filename, int day);
extern void Day_15(const char *filename, int day);
extern void Day_16(const char *filename, int day);
extern void Day_17(const char *filename, int day);
extern void Day_18(const char *filename, int day);
extern void Day_19(const char *filename, int day);

typedef void (*DAYS)(const char *filename, int day);

int day = 17;
DAYS days[] = {NULL,   Day_1,  Day_2,  Day_3,  Day_4,  Day_5,  Day_6,
               Day_7,  Day_8,  Day_9,  Day_10, Day_11, Day_12, Day_13,
               Day_14, Day_15, Day_16, Day_17, Day_18, Day_19};

int main(int argc, char **argv) {
  char data_name[128];

  switch (argc) {
  case 0:
    exit(-1);
  case 1:
    sprintf(data_name, "day_%d_test_data.txt", day);
    break;

  case 2:
    strcpy(data_name, argv[1]);
    break;
  }
  days[day](data_name, day);

  return 0;
}