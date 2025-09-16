#ifndef INSTRUCTION_H
#define INSTRUCTION_H
#include <stdint.h>
#include "stack.h"

typedef enum {
	ADD_8,
	SUB_8,
	MUL_8,
	DIV_8,
	PUSH_8,
	POP_8,
} OPCODE;

typedef struct {
	OPCODE opcode;
	uint8_t* data;
} INSTRUCTION;

INSTRUCTION* instruction_create(OPCODE code, uint8_t* data);
uint8_t exec_instruction(STACK* stack, INSTRUCTION* instr);

#endif

