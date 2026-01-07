#include "include/machine.h"
#include "include/stack.h"
#include "include/instruction.h"


MACHINE *machine_init() {
    MACHINE *machine = malloc(sizeof(MACHINE));
    machine->stack = stack_init();
    machine->program_counter = 0;
    return machine;
}

void machine_set_instructions(MACHINE *machine, LIST* instructions) {
    machine->instructions = instructions;
}

uint8_t machine_exec_next_instruction(MACHINE* machine) {
    INSTRUCTION *instr = LIST_GET(machine->instructions, INSTRUCTION*, machine->program_counter);
	uint8_t first;
	uint8_t second;
	switch(instr->opcode) {
		case PUSH_8:
			if(instr->data == NULL) {
				printf("Push instruction should have data");
				exit(1);
			}
			push_byte(machine->stack, instr->data[0]);
			break;
		case POP_8:
			pop_byte(machine->stack);
			break;
		case ADD_8:
			first = pop_byte(machine->stack);
			second = pop_byte(machine->stack);
			push_byte(machine->stack,  first + second);
			break;
		case MUL_8:
			push_byte(machine->stack, pop_byte(machine->stack) * pop_byte(machine->stack));
			break;
		case SUB_8:
			first = pop_byte(machine->stack);
			second = pop_byte(machine->stack);
			push_byte(machine->stack, second - first);
			break;
		case DIV_8:
			first = pop_byte(machine->stack);
			second = pop_byte(machine->stack);
			push_byte(machine->stack, second / first);
			break;
        case PRINT_8:
            // TODO: This should be done via syscall
            // Also, eleminate the waste of popping and pushing
            first = pop_byte(machine->stack);
            printf("%02X\n", first);
            push_byte(machine->stack, first);
            break;
		default:
			// TODO: Handle this gracefully with error codes
			printf("Unimplemented instruction. Exiting...");
			exit(1);
	}
    return 0;
}

uint8_t machine_exec_program(MACHINE* machine) {
    while(machine->program_counter <= machine->instructions->pointer) {
        machine_exec_next_instruction(machine);
        machine->program_counter++;
    }
    return 0;
}