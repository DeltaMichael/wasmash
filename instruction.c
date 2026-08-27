#include "include/instruction.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

INSTRUCTION *instruction_create(const char* name, OPCODE code, int size, uint8_t *data) {
  INSTRUCTION *instr = calloc(1, sizeof(INSTRUCTION));
  instr->name = strdup(name);
  instr->opcode = code;
  instr->data_size = size;
  instr->data = data;
  return instr;
}

INSTRUCTION *instruction_create_with_value(const char* name, OPCODE code, uint8_t data) {
  INSTRUCTION *instr = calloc(1, sizeof(INSTRUCTION));
  instr->name = strdup(name);
  instr->opcode = code;
  instr->data = malloc(sizeof(uint8_t));
  *instr->data = data;
  return instr;
}

