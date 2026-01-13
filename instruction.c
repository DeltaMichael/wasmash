#include "include/instruction.h"
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

INSTRUCTION* instruction_create(OPCODE code, uint8_t* data) {
	INSTRUCTION* instr = calloc(1, sizeof(INSTRUCTION));
	instr->opcode = code;
	instr->data = data;
	return instr;
}