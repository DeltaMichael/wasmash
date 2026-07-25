#include "include/asm_lexer.h"
#include "include/instruction.h"
#include "include/list.h"
#include "include/machine.h"
#include "include/stack.h"
#include <stdio.h>
#include <string.h>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
EM_JS(void, js_update_stack, (int length, uint8_t *data), {
  for (var i = 0; i < length; i++) {
    stackData[i] = Module.HEAPU8[data + i];
  }
  updateStackView();
});
#endif

char *read_file(char *path) {
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

int main(int argc, char **argv) {
  LIST *program = NULL;
  LIST *jump_table = NULL;
  if (argc >= 3 && strcmp(argv[1], "-i") == 0) {
    // TODO: Fix it, does not currently work
    ASM_LEXER *lexer = asm_lexer_init(argv[2]);
    asm_lexer_process(lexer);
    program = lexer->instructions;
  } else if (argc == 3 && strcmp(argv[1], "-f") == 0) {
    char *raw_input = read_file(argv[2]);
    ASM_LEXER *lexer = asm_lexer_init(raw_input);
    asm_lexer_process(lexer);
    program = lexer->instructions;
    jump_table = lexer->jump_table;
  } else {
#ifdef __EMSCRIPTEN__
    // TODO: Figure this out
#else
    printf("Usage\n");
    printf("mishmash -f /path/to/program/file\n");
    printf("mishmash -i 'inline instructions'\n");
    return 1;
#endif
  }

  MACHINE *machine = machine_init();
  machine_set_instructions(machine, program);
  machine_set_jump_table(machine, jump_table);
  machine_exec_program(machine);

#ifdef __EMSCRIPTEN__
  js_update_stack(machine->stack->top + 1, stack->data);
#else
//	print_stack(21, machine->stack->data);
#endif
  return 0;
}

