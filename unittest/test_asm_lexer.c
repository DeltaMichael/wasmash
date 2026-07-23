#include <assert.h>
#include "../include/list.h"
#include "../include/asm_lexer.h"
#include "../include/instruction.h"

void test_parse_instructions() {
	char* input = "push8 1;\r\npush8 2;\r\nadd8;\r\nprint8;\r\n";
	ASM_LEXER* lexer = asm_lexer_init(input);

	INSTRUCTION* expected[4] = { instruction_create_with_value(PUSH_8, 1),
		instruction_create_with_value(PUSH_8, 2),
		instruction_create(ADD_8, NULL),
		instruction_create(PRINT_8, NULL),
	};

	asm_lexer_process(lexer);
	assert(lexer->instructions->size == 4);
	for(int i = 0; i < lexer->instructions->size; i++) {
		INSTRUCTION* instruction = LIST_GET(lexer->instructions, INSTRUCTION*, i);
		assert(instruction->opcode == expected[i]->opcode);
		assert(*instruction->data == *expected[i]->data);
	}
}

int main(int argc, char** args) {
	test_parse_instructions();
}

