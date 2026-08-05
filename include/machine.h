#ifndef MACHINE_H
#define MACHINE_H

#include "list.h"
#include "stack.h"

typedef struct {
  LIST *instructions;
  LIST *jump_table;
  STACK *stack;
  uint32_t program_counter;
  uint8_t cmp_result;
} MACHINE;

MACHINE *machine_init();
void machine_set_instructions(MACHINE *machine, LIST *instructions);
void machine_set_jump_table(MACHINE *machine, LIST *jump_table);
uint8_t machine_exec_next_instruction(MACHINE *machine);
uint8_t machine_exec_program(MACHINE *machine);
uint8_t machine_exec_program_debug(MACHINE *machine);

#endif

