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

char *read_file(char *path)
{
	FILE *f = fopen(path, "rb");
	fseek(f, 0, SEEK_END);
	size_t file_size = ftell(f);
	rewind(f);
	char *out = malloc(file_size + 1);
	fread(out, sizeof(char), file_size, f);
	out[file_size] = 0;
	fclose(f);
	return out;
}

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
	} else if(argc == 3 && strcmp(argv[1],"-f") == 0) {
		char* raw_input = read_file(argv[2]);
		ASM_LEXER* lexer = asm_lexer_init(raw_input);
		asm_lexer_process(lexer);
		program = lexer->instructions;
	} else {
		program = sum_one_to_five();
	}

	// TODO: Read this one line at a time
	// Or some other streaming solution
	// Reading the entire file in memory is quite bad
	STACK* stack = stack_init();
	for(int i = 0; i < program->pointer; i++) { // TODO: test this with emscripten, might have to be program->pointer + 1
		exec_instruction(stack, LIST_GET(program, INSTRUCTION*, i));
	}

#ifdef __EMSCRIPTEN__
	js_update_stack(stack->top + 1, stack->data);
#else
	print_stack(21, stack->data);
#endif
    return 0;
}

