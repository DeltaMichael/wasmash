#include <string.h>

#include "include/asm_lexer.h"
#include "include/char_helper.h"
#include "include/hashmap.h"
#include "include/instruction.h"
#include "include/list.h"

DISPATCH *dinit() {
  DISPATCH *dispatcher = malloc(sizeof(DISPATCH));
  dispatcher->functions = hashmap_init();
  return dispatcher;
}

void dinsert_instr_proc(DISPATCH *dispatcher, char *key,
                        INSTR_PROC instr_proc) {
  INSTR_PROC newval = malloc(sizeof(instr_proc));
  newval = instr_proc;
  hashmap_insert(dispatcher->functions, key, newval);
}

INSTR_PROC dget_instr_proc(DISPATCH *dispatcher, char *key) {
  return (INSTR_PROC)hashmap_get(dispatcher->functions, key);
}

ASM_LEXER *asm_lexer_init(char *input) {
  ASM_LEXER *lexer = malloc(sizeof(ASM_LEXER));
  lexer->current = 0;
  lexer->line_count = 0;
  lexer->input = input;
  lexer->errors = LIST_INIT(LEXER_ERROR *, 128);
  lexer->instructions = LIST_INIT(INSTRUCTION *, 128);
  lexer->jump_table = LIST_INIT(uint32_t, 128);
  lexer->instr_arg = hashmap_init();
  lexer->instr_no_arg = hashmap_init();
  lexer->labels = hashmap_init();
  lexer->labels_interpolation = hashmap_init();
  lexer->dispatcher = dinit();

  hashmap_insert_int(lexer->instr_arg, "push8", PUSH_8);
  hashmap_insert_int(lexer->instr_arg, "prsp8", PRSP_8);
  hashmap_insert_int(lexer->instr_arg, "prt8", PRT_8);
  hashmap_insert_int(lexer->instr_arg, "pabs8", PABS_8);
  hashmap_insert_int(lexer->instr_arg, "lrsp8", LRSP_8);
  hashmap_insert_int(lexer->instr_arg, "labs8", LABS_8);
  hashmap_insert_int(lexer->instr_arg, "jz8", JZ_8);
  hashmap_insert_int(lexer->instr_arg, "jnz8", JNZ_8);
  hashmap_insert_int(lexer->instr_arg, "jmp8", JMP_8);
  hashmap_insert_int(lexer->instr_arg, "call", CALL_8);
  hashmap_insert_int(lexer->instr_arg, "enter8", ENTER_8);

  hashmap_insert_int(lexer->instr_no_arg, "add8", ADD_8);
  hashmap_insert_int(lexer->instr_no_arg, "mul8", MUL_8);
  hashmap_insert_int(lexer->instr_no_arg, "sub8", SUB_8);
  hashmap_insert_int(lexer->instr_no_arg, "div8", DIV_8);
  hashmap_insert_int(lexer->instr_no_arg, "pop8", POP_8);
  hashmap_insert_int(lexer->instr_no_arg, "cmp8", CMP_8);
  hashmap_insert_int(lexer->instr_no_arg, "print8", PRINT_8);
  hashmap_insert_int(lexer->instr_no_arg, "ret8", RET_8);
  hashmap_insert_int(lexer->instr_no_arg, "nop", NOP);

  dinsert_instr_proc(lexer->dispatcher, "push8", asm_lexer_one_arg_instr);
  dinsert_instr_proc(lexer->dispatcher, "prsp8", asm_lexer_one_arg_instr);
  dinsert_instr_proc(lexer->dispatcher, "prt8", asm_lexer_one_arg_instr);
  dinsert_instr_proc(lexer->dispatcher, "pabs8", asm_lexer_one_arg_instr);
  dinsert_instr_proc(lexer->dispatcher, "lrsp8", asm_lexer_one_arg_instr);
  dinsert_instr_proc(lexer->dispatcher, "labs8", asm_lexer_one_arg_instr);
  dinsert_instr_proc(lexer->dispatcher, "enter8", asm_lexer_one_arg_instr);
  dinsert_instr_proc(lexer->dispatcher, "jz8", asm_lexer_jump_instr);
  dinsert_instr_proc(lexer->dispatcher, "jnz8", asm_lexer_jump_instr);
  dinsert_instr_proc(lexer->dispatcher, "jmp8", asm_lexer_jump_instr);
  dinsert_instr_proc(lexer->dispatcher, "call", asm_lexer_jump_instr);

  dinsert_instr_proc(lexer->dispatcher, "add8", asm_lexer_no_arg_instr);
  dinsert_instr_proc(lexer->dispatcher, "mul8", asm_lexer_no_arg_instr);
  dinsert_instr_proc(lexer->dispatcher, "sub8", asm_lexer_no_arg_instr);
  dinsert_instr_proc(lexer->dispatcher, "div8", asm_lexer_no_arg_instr);
  dinsert_instr_proc(lexer->dispatcher, "pop8", asm_lexer_no_arg_instr);
  dinsert_instr_proc(lexer->dispatcher, "cmp8", asm_lexer_no_arg_instr);
  dinsert_instr_proc(lexer->dispatcher, "ret8", asm_lexer_no_arg_instr);
  dinsert_instr_proc(lexer->dispatcher, "print8", asm_lexer_no_arg_instr);
  dinsert_instr_proc(lexer->dispatcher, "nop", asm_lexer_no_arg_instr);

  return lexer;
}

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
  return lexer->input[lexer->current] == '\0';
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
  if (start == lexer->current) {
    return NULL;
  }
  return strndup(lexer->input + start, lexer->current - start);
}

void asm_lexer_eat(ASM_LEXER *lexer, char eaten) {
  if (asm_lexer_current(lexer) == eaten) {
    asm_lexer_advance(lexer);
    return;
  }

  asm_lexer_report_error(lexer, "Expected %c but got '%c'\n", eaten,
                         asm_lexer_current(lexer));
  asm_lexer_skip_to_terminator(lexer);
}

void asm_lexer_skip_to_terminator(ASM_LEXER *lexer) {
  while (!asm_lexer_at_end(lexer)) {
    asm_lexer_advance(lexer);
    if (is_lineterm(asm_lexer_current(lexer))) {
      asm_lexer_advance(lexer);
      return;
    }
    if (is_newline(asm_lexer_current(lexer))) {
      return;
    }
  }
}

void asm_lexer_report_error(ASM_LEXER *lexer, const char *fmt, ...) {
  va_list args1, args2;
  va_start(args1, fmt);
  va_copy(args2, args1);
  LEXER_ERROR *err = malloc(sizeof(LEXER_ERROR));
  err->line = lexer->jump_table->size + 1;
  int length = vsnprintf(NULL, 0, fmt, args1);
  err->message = malloc(sizeof(char) * (length + 1));
  vsnprintf(err->message, length + 1, fmt, args2);
  LIST_APPEND(lexer->errors, LEXER_ERROR *, err);
  va_end(args1);
  va_end(args2);
}

void asm_lexer_no_arg_instr(ASM_LEXER *lexer, char *instruction,
                            char *argument) {
  int opcode;
  hashmap_get_int(lexer->instr_no_arg, instruction, &opcode);
  INSTRUCTION *instr = instruction_create(instruction, opcode, NULL);
  LIST_APPEND(lexer->instructions, INSTRUCTION *, instr);
}

void asm_lexer_one_arg_instr(ASM_LEXER *lexer, char *instruction,
                             char *argument) {
  int opcode;
  hashmap_get_int(lexer->instr_arg, instruction, &opcode);
  uint8_t *data = malloc(sizeof(uint8_t));
  *data = atoi(argument);
  INSTRUCTION *instr = instruction_create(instruction, opcode, data);
  LIST_APPEND(lexer->instructions, INSTRUCTION *, instr);
}

void asm_lexer_jump_instr(ASM_LEXER *lexer, char *instruction, char *argument) {
  int opcode;
  hashmap_get_int(lexer->instr_arg, instruction, &opcode);
  uint8_t *data = malloc(sizeof(uint8_t));
  char *endptr;
  *data = strtol(argument, &endptr, 10);
  if (*endptr != '\0') { // it's a label
    int line;
    int err = hashmap_get_int(lexer->labels, argument, &line);
    if (err == 0) { // we've seen it before
      *data = (uint8_t)line;
    } else { // save for interpolation
      LIST *interpolations;
      int err = hashmap_get_list(lexer->labels_interpolation, argument,
                                 &interpolations);
      if (err != 0) { // list does not exist, initialize
        interpolations = LIST_INIT(INSTRUCTION *, 16);
        hashmap_insert_list(lexer->labels_interpolation, argument,
                            interpolations);
      }
      INSTRUCTION *instr = instruction_create(instruction, opcode, data);
      LIST_APPEND(interpolations, INSTRUCTION *, instr);
      LIST_APPEND(lexer->instructions, INSTRUCTION *, instr);
      return;
    }
  }
  INSTRUCTION *instr = instruction_create(instruction, opcode, data);
  LIST_APPEND(lexer->instructions, INSTRUCTION *, instr);
}

void asm_lexer_process_instr(ASM_LEXER *lexer, char **instr, char **argument) {
  asm_lexer_skip_whitespace(lexer);
  if (is_alnum(asm_lexer_current(lexer))) {
    // parse instruction
    int value;
    *instr = asm_lexer_instruction(lexer);
    asm_lexer_skip_whitespace(lexer);
    // parse arguments if any
    *argument = asm_lexer_number(lexer);
    if (hashmap_get_int(lexer->instr_arg, *instr, &value) == 0) {
      if (*argument == NULL) {
        asm_lexer_report_error(lexer, "Expected argument for instruction %s\n",
                               *instr);
      }
    } else if (hashmap_get_int(lexer->instr_no_arg, *instr, &value) == 0) {
      if (*argument != NULL) {
        asm_lexer_report_error(
            lexer, "Instruction %s does not require an argument\n", *instr);
      }
    }

    asm_lexer_skip_whitespace(lexer);

    if (asm_lexer_current(lexer) == LABEL_TERM) {
      hashmap_insert_int(lexer->labels, *instr, lexer->jump_table->size + 1);

      *instr = NULL;
      asm_lexer_eat(lexer, LABEL_TERM);
      LIST_APPEND(lexer->jump_table, uint32_t, lexer->line_count);
      asm_lexer_skip_line(lexer);
    } else {
      // eat the line term
      asm_lexer_eat(lexer, LINE_TERM);
    }
  } else if (is_forward_slash(asm_lexer_current(lexer)) &&
             is_forward_slash(asm_lexer_peek(lexer))) {
    // it's a comment
    // just skip it and update the jump table
    LIST_APPEND(lexer->jump_table, uint32_t, lexer->line_count);
    asm_lexer_skip_line(lexer);
  } else if (asm_lexer_at_end(lexer)) {
    return;
  } else {
    asm_lexer_report_error(lexer, "Unexpected character %c\n",
                           asm_lexer_current(lexer));
    asm_lexer_skip_to_terminator(lexer);
  }
}

void asm_lexer_interpolate_labels(ASM_LEXER *lexer) {
  for (int i = 0; i < lexer->labels_interpolation->capacity; i++) {
    H_ENTRY *entry = lexer->labels_interpolation->data + i;
    if (entry->active) {
      LIST *interpolations;
      int err = hashmap_get_list(lexer->labels_interpolation, entry->key,
                                 &interpolations);
      if (err) {
        // TODO: error handling
        printf("No interpolations for label %s\n", entry->key);
        continue;
      }
      for (int j = 0; j < interpolations->size; j++) {
        INSTRUCTION *instr = LIST_GET(interpolations, INSTRUCTION *, j);
        int line_number;
        int err = hashmap_get_int(lexer->labels, entry->key, &line_number);
        if (err) {
          // TODO: error handling
          printf("Label %s does not exist\n", entry->key);
          continue;
        }
        *instr->data = (uint8_t)line_number;
      }
    }
  }
}

int asm_lexer_get_start_line(ASM_LEXER *lexer) {
  int line_number;
  int err = hashmap_get_int(lexer->labels, "start", &line_number);
  if (err == 0) {
    return line_number;
  }
  return 0;
}

void asm_lexer_process(ASM_LEXER *lexer) {
  while (!asm_lexer_at_end(lexer)) {
    char *instr = NULL;
    char *argument = NULL;
    asm_lexer_process_instr(lexer, &instr, &argument);

    // Comment, empty line or label
    if (instr == NULL) {
      continue;
    }

    // Get the dispatch function for the instruction
    INSTR_PROC proc = dget_instr_proc(lexer->dispatcher, instr);
    if (proc == NULL) {
      asm_lexer_report_error(lexer, "Unknown instruction '%s'\n", instr);
    }
    if (lexer->errors->size == 0) {
      proc(lexer, instr, argument);
    }

    // Update the jump table, increment the line
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
  asm_lexer_interpolate_labels(lexer);
}

