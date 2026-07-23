#ifndef STACK_H
#define STACK_H

#include <stdint.h>

#define STACK_SIZE 1024
#define STACK_OVERFLOW 10240

typedef struct {
  uint32_t cap;
  int64_t top;
  int64_t sp;
  uint8_t *data;
} STACK;

STACK *stack_init();
void push_byte(STACK *stack, uint8_t val);
void push_2b(STACK *stack, uint16_t val);
void push_4b(STACK *stack, uint32_t val);
uint8_t pop_byte(STACK *stack);
uint8_t get_byte(STACK *stack, int64_t index);
uint16_t pop_2b(STACK *stack);
uint32_t pop_4b(STACK *stack);
void print_stack(int length, uint8_t *data);
void decrement_top(STACK *stack);

#endif
