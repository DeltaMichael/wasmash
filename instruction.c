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

uint8_t exec_instruction(STACK* stack, INSTRUCTION* instr) {
	uint8_t first;
	uint8_t second;
	switch(instr->opcode) {
		case PUSH_8:
			if(instr->data == NULL) {
				printf("Push instruction should have data");
				exit(1);
			}
			push_byte(stack, instr->data[0]);
			break;
		case POP_8:
			pop_byte(stack);
			break;
		case ADD_8:
			push_byte(stack, pop_byte(stack) + pop_byte(stack));
			break;
		case MUL_8:
			push_byte(stack, pop_byte(stack) * pop_byte(stack));
			break;
		case SUB_8:
			first = pop_byte(stack);
			second = pop_byte(stack);
			push_byte(stack, second - first);
			break;
		case DIV_8:
			first = pop_byte(stack);
			second = pop_byte(stack);
			push_byte(stack, second / first);
			break;
		default:
			// TODO: Handle this gracefully
			printf("Unimplemented instruction. Exiting...");
			exit(1);
	}
	return 0;
}

