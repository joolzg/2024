#include "file_reader.h"
#include <string.h>

uint32_t read_file(const char *filename, Starting start, Line line,
                   Ending end) {
  uint32_t line_count = 0;
  FILE *f = fopen(filename, "r");
  if (f) {
    char new_line[513];

    start(0);
    while (fgets(new_line, sizeof(new_line) - 1, f)) {
      new_line[sizeof(new_line) - 1] = 0;
      int length = strlen(new_line);
      if (new_line[length - 1] == 10) {
        new_line[length - 1] = 0;
        length--;
      }
      if (length) {
        line(new_line, length);
        line_count++;
      }
    }
    fclose(f);
  } else {
    end(-1);
    return -1;
  }
  end(0);

  return line_count;
}
