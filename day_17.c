#include "file_reader.h"
#include <ctype.h>
#include <math.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define MIN(X, Y) ((X) < (Y) ? (X) : (Y))
#define MAX(X, Y) ((X) > (Y) ? (X) : (Y))
#define SGN(X) ((X) < 0 ? -1 : 1)

typedef struct CPU {
  uint64_t reg_A;
  uint64_t reg_B;
  uint64_t reg_C;
  int output_size;
  char output[24];
} CPU;

static int lines;
static uint64_t output_value;
static char *input;
static int input_length;
static CPU cpu;

static int start(int error) {
  lines = 0;
  input = NULL;
  input_length = 0;

  return 0;
}

static int line(const char *line, int line_length) {
  if (!memcmp(line, "Register A: ", strlen("Register A: "))) {
    const char *p = line + strlen("Register A: ");
    cpu.reg_A = 0;
    while (*p) {
      cpu.reg_A *= 10;
      cpu.reg_A += (*p++) - '0';
    }
  } else if (!memcmp(line, "Register B: ", strlen("Register B: "))) {
    const char *p = line + strlen("Register B: ");
    cpu.reg_B = 0;
    while (*p) {
      cpu.reg_B *= 10;
      cpu.reg_B += (*p++) - '0';
    }
  } else if (!memcmp(line, "Register C: ", strlen("Register C: "))) {
    const char *p = line + strlen("Register C: ");
    cpu.reg_C = 0;
    while (*p) {
      cpu.reg_C *= 10;
      cpu.reg_C += (*p++) - '0';
    }
  } else if (!memcmp(line, "Program: ", strlen("Program: "))) {
    line_length -= strlen("Program: ");
    line += strlen("Program: ");
    input = realloc(input, input_length + line_length + 1);
    memcpy(input + input_length, line, line_length);
    input_length += line_length;
    input[input_length] = 0;
    lines++;
  }
  return 0;
}

uint8_t instructions[256];
int number_of_instructions;

static void process_cpu_fast() {
  cpu.output_size = 0;
  do {
    cpu.reg_B = cpu.reg_A % 8;
    cpu.reg_B ^= 3;
    cpu.reg_C = cpu.reg_A / (1 << cpu.reg_B);
    cpu.reg_A /= 8;
    cpu.reg_B ^= cpu.reg_C;
    cpu.reg_B ^= 5;
    cpu.output[cpu.output_size] = 48 + (cpu.reg_B % 8);
    cpu.output_size++;
    cpu.output[cpu.output_size] = 0;
  } while (cpu.reg_A);
}

static void process_cpu() {
  int pos = 0;
  cpu.output_size = 0;
  while (pos < number_of_instructions) {
    int skip = 2;
    uint64_t opcode = instructions[pos];
    uint64_t operand = instructions[pos + 1];
    if (1) {
      uint64_t combo;
      switch (operand) {
      case 0:
      case 1:
      case 2:
      case 3:
        combo = operand;
        break;
      case 4: // Reg A
        combo = cpu.reg_A;
        break;
      case 5: // Reg B
        combo = cpu.reg_B;
        break;
      case 6: // Reg C
        combo = cpu.reg_C;
        break;
      case 7:
      default:
        combo = 0;
        break;
      }
      switch (opcode) {
      case 0: { // Division int A
        uint64_t mod = 1 << combo;
        cpu.reg_A /= mod;
      } break;
      case 1: // bitwise XOR
        cpu.reg_B ^= operand;
        break;
      case 2: // Modulo B
        cpu.reg_B = combo % 8;
        break;
      case 3: // jnz
        if (cpu.reg_A) {
          pos = operand * 2;
          skip = 0;
        }
        break;
      case 4: // bitwise XOR B C
        cpu.reg_B ^= cpu.reg_C;
        break;
      case 5: // OUT
        cpu.output[cpu.output_size] = 48 + (combo % 8);
        cpu.output_size++;
        cpu.output[cpu.output_size] = 0;
        if (cpu.output_size == sizeof(cpu.output)) {
          return;
        }
        break;
      case 6: { // Division into B
        uint64_t mod = 1 << combo;
        cpu.reg_B = cpu.reg_A / mod;
      } break;
      case 7: { // Division into C
        uint64_t mod = 1 << combo;
        cpu.reg_C = cpu.reg_A / mod;
      } break;
      }
    } else {
      break;
    }
    pos += skip;
  }
}

// #define ARRAY_SIZE 2048
// uint64_t next_values[ARRAY_SIZE];
// int next_values_count;
// uint64_t new_values[ARRAY_SIZE];
// int new_values_count;

static int end(int error) {
  char *wanted = calloc(1, 64);
  int wanted_size = 0;

  char *insts = input;
  while (*insts) {
    uint64_t ins, oper;
    sscanf(insts, "%ld,%ld", &ins, &oper);
    instructions[number_of_instructions++] = ins;
    instructions[number_of_instructions++] = oper;
    wanted[wanted_size++] = ins + '0';
    wanted[wanted_size++] = oper + '0';
    insts += 3;
    if (*insts == ',')
      insts++;
  }

  process_cpu();
  printf("Part1 - ");
  for (int l = 0; l < cpu.output_size; l++) {
    printf("%c", cpu.output[l]);
    if (l < (cpu.output_size - 1))
      printf(",");
  }
  printf("\n");
  output_value = 0;

  uint8_t octets[] = {7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7};

  if (1) {
    uint64_t reg_A = 0;
    for (int n = 0; n < wanted_size; n++) {
      reg_A <<= 3;
      reg_A += octets[n];
    }

    // CPU fast_cpu;
    // cpu.reg_A = reg_A;
    // cpu.reg_B = 0;
    // cpu.reg_C = 0;
    // process_cpu_fast();
    // fast_cpu = cpu;

    cpu.reg_A = reg_A;
    cpu.reg_B = 0;
    cpu.reg_C = 0;
    process_cpu();

    // if ((fast_cpu.reg_A != cpu.reg_A) || (fast_cpu.reg_B != cpu.reg_B) ||
    //     (fast_cpu.reg_C != cpu.reg_C) ||
    //     (fast_cpu.output_size != cpu.output_size)) {
    //   return 0;
    // }
    cpu.reg_A = reg_A;
  }

  output_value = INT64_MAX;

  int new_values_count = 0;
  uint64_t new_values[128][2];

  new_values[new_values_count][0] = cpu.reg_A;
  new_values[new_values_count][1] = 0;
  new_values_count++;
  while (new_values_count) {
    new_values_count--;
    uint64_t reg_A = new_values[new_values_count][0];
    int which_digit = new_values[new_values_count][1];
    int m = wanted_size;
    for (int n = 0; n < wanted_size; n++) {
      octets[--m] = reg_A % 8;
      reg_A >>= 3;
    }
    while (1) {
      reg_A = 0;
      for (int n = 0; n < wanted_size; n++) {
        reg_A <<= 3;
        reg_A += octets[n];
      }
      cpu.reg_A = reg_A;
      cpu.reg_B = 0;
      cpu.reg_C = 0;
      process_cpu();
      if (!memcmp(cpu.output, wanted, cpu.output_size)) {
        reg_A = 0;
        for (int n = 0; n < wanted_size; n++) {
          reg_A <<= 3;
          reg_A += octets[n];
        }
        cpu.reg_A = reg_A;
        cpu.reg_B = 0;
        cpu.reg_C = 0;
        process_cpu();
        cpu.reg_A = reg_A;
        printf("CPU A %ld\n", reg_A);
        output_value = MIN(output_value, reg_A);
      } else if (memcmp(cpu.output + wanted_size - (which_digit + 1),
                        wanted + wanted_size - (which_digit + 1),
                        (which_digit + 1))) {
      } else {
        new_values[new_values_count][0] = reg_A;
        new_values[new_values_count][1] = which_digit + 1;
        new_values_count++;
      }
      octets[which_digit]--;
      if (octets[which_digit] == 255) {
        break;
      }
    }
  }

  printf("Part2 %ld (236580836040301)\n", output_value);

  return 0;
}

void Day_17(const char *filename, int day) {
  printf("DAY%d %s\n", day, filename);
  read_file(filename, start, line, end);
}
