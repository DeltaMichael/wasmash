#ifndef MACHINE_H
#define MACHINE_H

#include "stack.h"
#include "list.h"

typedef struct {
    LIST *instructions;
    STACK *stack;
    uint32_t program_counter;
} MACHINE;

MACHINE *machine_init();
void machine_set_instructions(MACHINE *machine, LIST* instructions);
uint8_t machine_exec_next_instruction(MACHINE *machine);
uint8_t machine_exec_program(MACHINE *machine);

#endif