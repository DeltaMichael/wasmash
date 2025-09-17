#ifndef ASM_LEXER_H
#define ASM_LEXER_H

#include <stdint.h>
#include <stdbool.h>
#include "list.h"

typedef struct {
	char* input;
	uint32_t current;
	LIST* instructions;
} ASM_LEXER;

ASM_LEXER* asm_lexer_init(char* input);
bool is_alpha(char symbol);
bool is_num(char symbol);
bool is_alnum(char symbol);
bool is_lineterm(char symbol);
bool is_whitespace(char symbol);

void asm_lexer_advance(ASM_LEXER* lexer);
char asm_lexer_current(ASM_LEXER* lexer);
bool asm_lexer_at_end(ASM_LEXER* lexer);
void asm_lexer_skip_whitespace(ASM_LEXER* lexer);
char* asm_lexer_instruction(ASM_LEXER* lexer);
char* asm_lexer_number(ASM_LEXER* lexer);
void asm_lexer_process(ASM_LEXER* lexer);

#endif

