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
	LIST* list = LIST_INIT(INSTRUCTION*, 256);
	uint8_t data[5] = {1, 2, 3, 4, 5};
	for(uint8_t i = 0; i < 5; i++) {
		LIST_APPEND(list, INSTRUCTION*, instruction_create(PUSH_8, &data[i]));
	}
	for(uint8_t i = 0; i < 4; i++) {
		LIST_APPEND(list, INSTRUCTION*, instruction_create(ADD_8, NULL));
	}
	STACK* stack = stack_init();
	for(int i = 0; i < list->pointer + 1; i++) {
		exec_instruction(stack, LIST_GET(list, INSTRUCTION*, i));
	}
    for (int i = 0; i < stack->top + 21; i++) {
        printf("%02X ", stack->data[i]);
		if(i != 0 && i % 8 == 0) {
			printf("\n");
		}
    }
    return 0;
}

