/*
 * Copyright 2011 The Emscripten Authors.  All rights reserved.
 * Emscripten is available under two separate licenses, the MIT license and the
 * University of Illinois/NCSA Open Source License.  Both these licenses can be
 * found in the LICENSE file.
 */

#include <stdio.h>
#include "include/stack.h"

int main(int argc, char **argv) {
	STACK* stack = init();
	push_byte(stack, 0xAA);
	push_2b(stack, 0xAABB);
	push_4b(stack, 0xAABBCCDD);
	printf("%X\n", pop_4b(stack));
	printf("%X\n", pop_2b(stack));
	printf("%X\n", pop_byte(stack));
    for (int i = 0; i < argc; i++) {
        printf("argv[%d] = %s\n", i, argv[i]);
    }
    return 0;
}

