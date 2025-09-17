#ifndef STACK_H
#define STACK_H

#include <stdint.h>

#define STACK_SIZE 1024
#define STACK_OVERFLOW 10240

typedef struct {
	uint32_t cap;
	uint32_t top;
	uint8_t* data;
} STACK;

void testme();

STACK* stack_init();
void push_byte(STACK* stack, uint8_t val);
void push_2b(STACK* stack, uint16_t val);
void push_4b(STACK* stack, uint32_t val);
uint8_t pop_byte(STACK* stack);
uint16_t pop_2b(STACK* stack);
uint32_t pop_4b(STACK* stack);
void print_stack(int length, uint8_t* data);
// add (23, 8), (32, 16)

#endif

