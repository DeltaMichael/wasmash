#include <stdio.h>
#include <stdlib.h>
#include "include/stack.h"

void testme() {
	printf("I am tested\n");
}

STACK* stack_init() {
	STACK* out = calloc(1, sizeof(STACK));
	out->cap = 1024;
	out->top = -1;
	out->data = calloc(1024, sizeof(uint8_t));
	return out;
}

void push_byte(STACK* stack, uint8_t val) {
	if(stack->top + 1 >= stack->cap) {
		// realloc, unless overflow
	}
	stack->data[stack->top + 1] = val;
	stack->top += 1;
}

void push_2b(STACK* stack, uint16_t val) {
	uint16_t mask = 0xFF00;
	push_byte(stack, (uint8_t)((val & mask) >> 8));
	push_byte(stack, (uint8_t)(val & (mask >> 8)));
}

void push_4b(STACK* stack, uint32_t val) {
	uint32_t mask = 0xFFFF0000;
	push_2b(stack, (uint16_t)((val & mask) >> 16));
	push_2b(stack, (uint16_t)(val & (mask >> 16)));
}

uint8_t pop_byte(STACK* stack) {
	if(stack->top < 0) {
		// TODO: Handle this gracefully
		exit(1);
	}
	uint8_t out = stack->data[stack->top];
	stack->data[stack->top] = 0;
	stack->top--;
	return out;
}

uint16_t pop_2b(STACK* stack) {
	uint8_t byte = pop_byte(stack);
	uint16_t out = pop_byte(stack);
	out = out << 8;
	out = out | (uint16_t)byte;
	return out;
}

uint32_t pop_4b(STACK* stack) {
	uint16_t word = pop_2b(stack);
	uint32_t out = pop_2b(stack);
	out = out << 16;
	out = out | (uint32_t)word;
	return out;
}

