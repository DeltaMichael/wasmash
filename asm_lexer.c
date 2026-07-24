#include <string.h>

#include "include/asm_lexer.h"
#include "include/hashmap.h"
#include "include/instruction.h"
#include "include/list.h"

ASM_LEXER *asm_lexer_init(char *input) {
  ASM_LEXER *lexer = malloc(sizeof(ASM_LEXER));
  lexer->current = 0;
  lexer->line_count = 0;
  lexer->input = input;
  lexer->instructions = LIST_INIT(INSTRUCTION *, 128);
  lexer->jump_table = LIST_INIT(uint32_t, 128);
  lexer->instr_arg = hashmap_init();
  lexer->instr_no_arg = hashmap_init();
  hashmap_insert_int(lexer->instr_arg, "push8", PUSH_8);
  hashmap_insert_int(lexer->instr_arg, "ltop8", LTOP_8);
  hashmap_insert_int(lexer->instr_arg, "ltop8abs", LTOP_8_ABS);
  hashmap_insert_int(lexer->instr_arg, "lrel8", LREL_8);
  hashmap_insert_int(lexer->instr_arg, "labs8", LABS_8);
  hashmap_insert_int(lexer->instr_arg, "jz8", JZ_8);
  hashmap_insert_int(lexer->instr_arg, "jnz8", JNZ_8);
  hashmap_insert_int(lexer->instr_arg, "jmp8", JMP_8);

  hashmap_insert_int(lexer->instr_no_arg, "add8", ADD_8);
  hashmap_insert_int(lexer->instr_no_arg, "mul8", MUL_8);
  hashmap_insert_int(lexer->instr_no_arg, "sub8", SUB_8);
  hashmap_insert_int(lexer->instr_no_arg, "div8", DIV_8);
  hashmap_insert_int(lexer->instr_no_arg, "pop8", POP_8);
  hashmap_insert_int(lexer->instr_no_arg, "cmp8", CMP_8);
  hashmap_insert_int(lexer->instr_no_arg, "print8", PRINT_8);
  hashmap_insert_int(lexer->instr_no_arg, "nop", NOP);

  return lexer;
}

bool is_alpha(char symbol) {
  return (symbol >= 'a' && symbol <= 'z') || (symbol >= 'A' && symbol <= 'Z');
}

bool is_num(char symbol) { return (symbol >= '0' && symbol <= '9'); }

bool is_alnum(char symbol) { return is_alpha(symbol) || is_num(symbol); }

bool is_lineterm(char symbol) { return symbol == ';'; }

bool is_whitespace(char symbol) {
  return symbol == ' ' || symbol == '\t' || symbol == '\r' || symbol == '\n';
}

bool is_newline(char symbol) { return symbol == '\n'; }

bool is_forward_slash(char symbol) { return symbol == '/'; }

void asm_lexer_advance(ASM_LEXER *lexer) { lexer->current++; }

char asm_lexer_current(ASM_LEXER *lexer) {
  return lexer->input[lexer->current];
}

char asm_lexer_peek(ASM_LEXER *lexer) {
  if (asm_lexer_at_end(lexer)) {
    return 0;
  } else {
    return lexer->input[lexer->current + 1];
  }
}

bool asm_lexer_at_end(ASM_LEXER *lexer) {
  return lexer->input[lexer->current] == 0;
}

void asm_lexer_skip_whitespace(ASM_LEXER *lexer) {
  while (!asm_lexer_at_end(lexer) && is_whitespace(asm_lexer_current(lexer))) {
    asm_lexer_advance(lexer);
    if (is_newline(asm_lexer_current(lexer))) {
      LIST_APPEND(lexer->jump_table, uint32_t, lexer->line_count);
    }
  }
}

void asm_lexer_skip_line(ASM_LEXER *lexer) {
  while (!asm_lexer_at_end(lexer) && asm_lexer_current(lexer) != '\n') {
    asm_lexer_advance(lexer);
  }
}

char *asm_lexer_instruction(ASM_LEXER *lexer) {
  int start = lexer->current;
  while (!asm_lexer_at_end(lexer) && is_alnum(asm_lexer_current(lexer))) {
    asm_lexer_advance(lexer);
  }
  return strndup(lexer->input + start, lexer->current - start);
}

char *asm_lexer_number(ASM_LEXER *lexer) {
  int start = lexer->current;
  while (!asm_lexer_at_end(lexer) && is_alnum(asm_lexer_current(lexer))) {
    asm_lexer_advance(lexer);
  }
  return strndup(lexer->input + start, lexer->current - start);
}

void asm_lexer_eat(ASM_LEXER *lexer, char eaten) {
  if (asm_lexer_current(lexer) == eaten) {
    asm_lexer_advance(lexer);
    return;
  }
  printf("Expected: '%c' got '%c'\n", eaten, asm_lexer_current(lexer));
  exit(1);
}

void asm_lexer_process(ASM_LEXER *lexer) {
  while (!asm_lexer_at_end(lexer)) {
    asm_lexer_skip_whitespace(lexer);
    char *instr = NULL;
    char *argument = NULL;
    if (is_alnum(asm_lexer_current(lexer))) {
      // parse instruction
      int value;
      instr = asm_lexer_instruction(lexer);
      asm_lexer_skip_whitespace(lexer);
      // parse arguments if any
      if (hashmap_get_int(lexer->instr_arg, instr, &value) == 0) {
        argument = asm_lexer_number(lexer);
        if (argument == NULL) {
          printf("Expected argument for instruction %s\n", instr);
          exit(1);
        }
      }
      asm_lexer_skip_whitespace(lexer);
      // eat the line term
      asm_lexer_eat(lexer, LINE_TERM);
    } else if (is_forward_slash(asm_lexer_current(lexer)) &&
               is_forward_slash(asm_lexer_peek(lexer))) {
      // it's a comment
      // just skip it and update the jump table
      LIST_APPEND(lexer->jump_table, uint32_t, lexer->line_count);
      asm_lexer_skip_line(lexer);
    }

    if (instr == NULL) { // Comment or empty line, go back to the start
      continue;
    }

    if (argument == NULL) {
      int opcode;
      int err = hashmap_get_int(lexer->instr_no_arg, instr, &opcode);
      if (err == 1) {
        printf("Instruction %s doesn't exist or requires an argument\n", instr);
        exit(1);
      }
      INSTRUCTION *instr = instruction_create(opcode, NULL);
      LIST_APPEND(lexer->instructions, INSTRUCTION *, instr);
    } else {
      int opcode;
      int err = hashmap_get_int(lexer->instr_arg, instr, &opcode);
      if (err == 1) {
        printf("Instruction %s doesn't exist\n", instr);
        exit(1);
      }
      uint8_t *data = malloc(sizeof(uint8_t));
      *data = atoi(argument);
      INSTRUCTION *instr = instruction_create(opcode, data);
      LIST_APPEND(lexer->instructions, INSTRUCTION *, instr);
    }

    LIST_APPEND(lexer->jump_table, uint32_t, lexer->line_count);
    lexer->line_count += 1;
    if (instr != NULL) {
      free(instr);
      instr = NULL;
    }
    if (argument != NULL) {
      free(argument);
      argument = NULL;
    }
  }
}
