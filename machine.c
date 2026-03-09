#include "include/machine.h"
#include "include/stack.h"
#include "include/instruction.h"


MACHINE *machine_init() {
    MACHINE *machine = malloc(sizeof(MACHINE));
    machine->stack = stack_init();
    machine->program_counter = 0;
	machine->cmp_result = 2;
    return machine;
}

void machine_set_instructions(MACHINE *machine, LIST* instructions) {
    machine->instructions = instructions;
}

void machine_set_jump_table(MACHINE *machine, LIST* jump_table) {
    machine->jump_table = jump_table;
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
		case LTOP_8:
			if(instr->data == NULL) {
				printf("Push instruction should have data");
				exit(1);
			}
			if(machine->stack->top < 0) {
				// TODO: Decide if we can address garbage or not
			}
			first = machine->stack->data[machine->stack->sp + instr->data[0]]; // TODO: Pull the whole offset, not just the first 8 bytes
			push_byte(machine->stack, first);
			break;
		case LTOP_8_ABS:
			break;
		case LREL_8:
			if(instr->data == NULL) {
				printf("Push instruction should have data");
				exit(1);
			}
			if(machine->stack->top < 0) {
				// TODO: Decide if we can address garbage or not
			}
			first = pop_byte(machine->stack);
			machine->stack->data[machine->stack->sp + instr->data[0]] = first; // TODO: Pull the whole offset, not just the first 8 bytes
			break;
		case LREL_8_ABS:
			break;
		case JMP_8:
			if(instr->data == NULL) {
				printf("JMP instruction should have data");
				exit(1);
			}
			machine->program_counter = instr->data[0] - 2;
    		uint32_t position = LIST_GET(machine->jump_table, uint32_t, instr->data[0] - 1);
			machine->program_counter = position - 1;
			break;
		case JZ_8:
			if (machine->cmp_result == 0) {
				if(instr->data == NULL) {
					printf("JMP instruction should have data");
					exit(1);
				}
    			uint32_t position = LIST_GET(machine->jump_table, uint32_t, instr->data[0] - 1);
				machine->program_counter = position - 1;
			}
			break;
		case JNZ_8:
			if (machine->cmp_result == 1) {
				if(instr->data == NULL) {
					printf("JMP instruction should have data");
					exit(1);
				}
    			uint32_t position = LIST_GET(machine->jump_table, uint32_t, instr->data[0] - 1);
				machine->program_counter = position - 1;
			}
			break;
		case CMP_8:
            first = pop_byte(machine->stack);
            second = pop_byte(machine->stack);
			machine->cmp_result = (first == second);
			push_byte(machine->stack, second);
			push_byte(machine->stack, first);
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
    while(machine->program_counter < machine->instructions->size && machine->program_counter >= 0) {
        machine_exec_next_instruction(machine);
        machine->program_counter++;
    }
    return 0;
}