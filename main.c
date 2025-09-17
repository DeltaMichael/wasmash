#include <stdio.h>
#include <string.h>
#include "include/stack.h"
#include "include/instruction.h"
#include "include/list.h"
#include "include/asm_lexer.h"

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
EM_JS(void, js_update_stack, (int length, uint8_t* data), {
    for (var i = 0; i < length; i++) {
        stackData[i] = Module.HEAPU8[data + i];
    }
    updateStackView();
});
#endif

LIST* sum_one_to_five() {
	LIST* list = LIST_INIT(INSTRUCTION*, 256);
	uint8_t data[5] = {1, 2, 3, 4, 5};
	for(uint8_t i = 0; i < 5; i++) {
		LIST_APPEND(list, INSTRUCTION*, instruction_create(PUSH_8, &data[i]));
	}
	for(uint8_t i = 0; i < 4; i++) {
		LIST_APPEND(list, INSTRUCTION*, instruction_create(ADD_8, NULL));
	}
	return list;
}

int main(int argc, char **argv) {
	LIST* program = NULL;
	if(argc == 3 && strcmp(argv[1],"-i") == 0) {
		ASM_LEXER* lexer = asm_lexer_init(argv[2]);
		asm_lexer_process(lexer);
		program = lexer->instructions;
	} else {
		program = sum_one_to_five();
	}

	STACK* stack = stack_init();
	for(int i = 0; i < program->pointer + 1; i++) {
		exec_instruction(stack, LIST_GET(program, INSTRUCTION*, i));
	}

#ifdef __EMSCRIPTEN__
	js_update_stack(stack->top + 1, stack->data);
#else
	print_stack(21, stack->data);
#endif
    return 0;
}

