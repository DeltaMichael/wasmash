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
	PRINT_8, // TODO: There should be a way to implement this wiht syscall, cause it's faster
	LTOP_8, // push the value at an address specified by an offset to the stack pointer
	LTOP_8_ABS, // push the value at an absolute address
	LREL_8, // load data on top of the stack to address relative to the stack pointer
	LREL_8_ABS, // load data on top of the stack to absolute address
	JMP_8, // jump to address
	CMP_8, // compare two values
	JZ_8, // jump if comparison result is zero
	JNZ_8, // jump if comparison result is non-zero
	NOP, // no-op
} OPCODE;

typedef struct {
	OPCODE opcode;
	uint8_t* data;
} INSTRUCTION;

INSTRUCTION* instruction_create(OPCODE code, uint8_t* data);

#endif

