#include <string.h>

#include "include/asm_lexer.h"
#include "include/list.h"
#include "include/instruction.h"

ASM_LEXER* asm_lexer_init(char* input) {
	ASM_LEXER* lexer = malloc(sizeof(ASM_LEXER));
	lexer->current = 0;
	lexer->input = input;
	lexer->instructions = LIST_INIT(INSTRUCTION*, 128);
	return lexer;
}

bool is_alpha(char symbol) {
	return (symbol >= 'a' && symbol <= 'z') || (symbol >= 'A' && symbol <= 'Z');
}

bool is_num(char symbol) {
	return (symbol >= '0' && symbol <= '9');
}

bool is_alnum(char symbol) {
	return is_alpha(symbol) || is_num(symbol);
}

bool is_lineterm(char symbol) {
	return symbol == ';';
}

bool is_whitespace(char symbol) {
	return symbol == ' ' || symbol == '\t' || symbol == '\r' || symbol == '\n';
}

void asm_lexer_advance(ASM_LEXER* lexer) {
	lexer->current++;
}

char asm_lexer_current(ASM_LEXER* lexer) {
	return lexer->input[lexer->current];
}

bool asm_lexer_at_end(ASM_LEXER* lexer) {
	return lexer->input[lexer->current] == 0;
}

void asm_lexer_skip_whitespace(ASM_LEXER* lexer) {
	while(is_whitespace(asm_lexer_current(lexer))) {
		asm_lexer_advance(lexer);
	}
}

char* asm_lexer_instruction(ASM_LEXER* lexer) {
	int start = lexer->current;
	while(!asm_lexer_at_end(lexer) && is_alnum(asm_lexer_current(lexer))) {
		asm_lexer_advance(lexer);
	}
	return strndup(lexer->input + start, lexer->current - start);
}

char* asm_lexer_number(ASM_LEXER* lexer) {
	int start = lexer->current;
	while(!asm_lexer_at_end(lexer) && is_alnum(asm_lexer_current(lexer))) {
		asm_lexer_advance(lexer);
	}
	return strndup(lexer->input + start, lexer->current - start);
}

void asm_lexer_process(ASM_LEXER* lexer) {
	while(!asm_lexer_at_end(lexer)) {
		asm_lexer_skip_whitespace(lexer);
		char* instr = NULL;
		char* argument = NULL;
		while(!asm_lexer_at_end(lexer) && !is_lineterm(asm_lexer_current(lexer))) {
			if(is_alpha(asm_lexer_current(lexer))) {
				instr = asm_lexer_instruction(lexer);
			} else if(is_num(asm_lexer_current(lexer))) {
				argument = asm_lexer_number(lexer);
			} else if(is_whitespace(asm_lexer_current(lexer))) {
				asm_lexer_advance(lexer);
			}
		}
		asm_lexer_advance(lexer); // skip the lineterm
		if(instr == NULL) {
			// TODO: error handling
		}
		if(strcmp("push8", instr) == 0) {
			uint8_t* data = malloc(sizeof(uint8_t));
			if(argument == NULL) {
				// TODO: Error handling
			}
			*data = atoi(argument);
			LIST_APPEND(lexer->instructions, INSTRUCTION*, instruction_create(PUSH_8, data));
		} else if (strcmp("add8", instr) == 0) {
			LIST_APPEND(lexer->instructions, INSTRUCTION*, instruction_create(ADD_8, NULL));
		} else {
			// TODO: Error handling
		}
		if(instr != NULL) {
			free(instr);
			instr = NULL;
		}
		if(argument != NULL) {
			free(argument);
			argument = NULL;
		}
	}
}

