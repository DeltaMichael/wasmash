/*
 * Copyright 2011 The Emscripten Authors.  All rights reserved.
 * Emscripten is available under two separate licenses, the MIT license and the
 * University of Illinois/NCSA Open Source License.  Both these licenses can be
 * found in the LICENSE file.
 */

#include <stdio.h>
#include "include/stack.h"
#include "include/instruction.h"
#include "include/list.h"

int main(int argc, char **argv) {
	LIST* list = LIST_INIT(uint8_t, 8);
	for(int i = 0; i < 300; i++) {
		LIST_APPEND(list, uint8_t, i);
	}
	for(int i = 0; i < 300; i++) {
		printf("%d. List number: %d\n", i, LIST_GET(list, uint8_t, i));
	}
	STACK* stack = stack_init();
	uint8_t data[5] = {1, 2, 3, 4, 5};
	exec_instruction(stack, instruction_create(PUSH_8, &data[0]));
	exec_instruction(stack, instruction_create(PUSH_8, &data[1]));
	exec_instruction(stack, instruction_create(PUSH_8, &data[2]));
	exec_instruction(stack, instruction_create(PUSH_8, &data[3]));
	exec_instruction(stack, instruction_create(PUSH_8, &data[4]));
	exec_instruction(stack, instruction_create(ADD_8, NULL));
	exec_instruction(stack, instruction_create(ADD_8, NULL));
	exec_instruction(stack, instruction_create(ADD_8, NULL));
	exec_instruction(stack, instruction_create(ADD_8, NULL));
    for (int i = 0; i < stack->top + 21; i++) {
        printf("%02X ", stack->data[i]);
		if(i != 0 && i % 8 == 0) {
			printf("\n");
		}
    }
    return 0;
}

