#ifndef ASM_LEXER_H
#define ASM_LEXER_H

#include "hashmap.h"
#include "list.h"
#include <stdbool.h>
#include <stdint.h>

#define LINE_TERM ';'
#define LABEL_TERM ':'

typedef struct {
  HASHMAP *functions;
} DISPATCH;

typedef struct {
  char *input;
  uint32_t current;
  uint32_t line_count;
  LIST *instructions;
  LIST *jump_table;
  HASHMAP *instr_no_arg;
  HASHMAP *instr_arg;
  HASHMAP *labels;
  HASHMAP *labels_interpolation;
  DISPATCH *dispatcher;
} ASM_LEXER;

typedef void (*INSTR_PROC)(ASM_LEXER *, char *, char *);

DISPATCH *dinit();
void dinsert_instr_proc(DISPATCH *dispatcher, char *key, INSTR_PROC instr_proc);
INSTR_PROC dget_instr_proc(DISPATCH *dispatcher, char *key);

ASM_LEXER *asm_lexer_init(char *input);
void asm_lexer_advance(ASM_LEXER *lexer);
char asm_lexer_current(ASM_LEXER *lexer);
bool asm_lexer_at_end(ASM_LEXER *lexer);
void asm_lexer_skip_whitespace(ASM_LEXER *lexer);
char *asm_lexer_instruction(ASM_LEXER *lexer);
char *asm_lexer_number(ASM_LEXER *lexer);
void asm_lexer_interpolate_labels(ASM_LEXER *lexer);
void asm_lexer_process_instr(ASM_LEXER *lexer, char **instr, char **argument);
void asm_lexer_process(ASM_LEXER *lexer);

void asm_lexer_no_arg_instr(ASM_LEXER *lexer, char *instruction,
                            char *argument);
void asm_lexer_one_arg_instr(ASM_LEXER *lexer, char *instruction,
                             char *argument);
void asm_lexer_jump_instr(ASM_LEXER *lexer, char *instruction, char *argument);

#endif
