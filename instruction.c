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

INSTRUCTION* instruction_create_with_value(OPCODE code, uint8_t data) {
	INSTRUCTION* instr = calloc(1, sizeof(INSTRUCTION));
	instr->opcode = code;
	instr->data = malloc(sizeof(uint8_t));
	*instr->data = data;
	return instr;
}

