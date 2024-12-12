#include <stdint.h>
#include <stdio.h>

typedef int (*Starting)(int errro);
typedef int (*Line)(const char *line, int line_length);
typedef int (*Ending)(int error);

uint32_t read_file(const char *filename, Starting start, Line line, Ending end);
