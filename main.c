#include "include/asm_lexer.h"
#include "include/list.h"
#include "include/machine.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
EM_JS(void, js_update_stack, (int length, uint8_t *data), {
  clearStackData();
  for (var i = 0; i < length; i++) {
    stackData.push(Module.HEAPU8[data + i]);
  }
  updateStackView();
});

LIST *jump_table = NULL;
uint32_t start;
ASM_LEXER *lexer = NULL;
MACHINE *machine = NULL;

EMSCRIPTEN_KEEPALIVE void load_program(const char *program) {
  lexer = asm_lexer_init(program);
  asm_lexer_process(lexer);
  if (lexer->errors->size > 0) {
    for (int i = 0; i < lexer->errors->size; i++) {
      LEXER_ERROR *error = LIST_GET(lexer->errors, LEXER_ERROR *, i);
      printf("ERR L%d: %s\n", error->line, error->message);
    }
    exit(1);
  }
  jump_table = lexer->jump_table;
  start = asm_lexer_get_start_line(lexer);

  machine = machine_init();
  machine_set_instructions(machine, lexer->instructions);
  machine_set_jump_table(machine, jump_table);
  uint32_t start_position = LIST_GET(machine->jump_table, uint32_t, start);
  machine->program_counter = start_position;
}

EMSCRIPTEN_KEEPALIVE void run_program(const char *program) {
  load_program(program);
  while (machine->program_counter < machine->instructions->size &&
         machine->program_counter >= 0) {
    machine_exec_next_instruction(machine);
    machine->program_counter++;
  }
  js_update_stack(machine->stack->top + 1, machine->stack->data);
}

EMSCRIPTEN_KEEPALIVE void exec_instruction() {
  if (machine->program_counter < machine->instructions->size &&
      machine->program_counter >= 0) {
    machine_exec_next_instruction(machine);
    machine->program_counter++;
    js_update_stack(machine->stack->top + 1, machine->stack->data);
  }
}

int main(int argc, char **argv) { return 0; }

#else
void usage() {
    printf("Usage:\nmishmash -f /path/to/program/file\nmishmash -i 'inline instructions'\n");
}

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
  optreset = 1;
  int ch;
  int ffile, finline, fdebug = 0;
  char* file_path = NULL;
  char *inline_instructions = NULL;
  while ((ch = getopt(argc, argv, "i:f:d")) != -1) { switch(ch) {
		case 'i':
			finline = 1;
			inline_instructions = strdup(optarg);
			break;
		case 'f':
			ffile = 1;
			file_path = strdup(optarg);
			break;
		case 'd':
			fdebug = 1;
			break;
		default:
			usage();
			break;
	}
  }

  LIST *program = NULL;
  LIST *jump_table = NULL;
  uint32_t start;
  ASM_LEXER *lexer = NULL;

  if (finline && inline_instructions) {
    lexer = asm_lexer_init(inline_instructions);
  } else if (ffile && file_path) {
    char *raw_input = read_file(file_path);
    lexer = asm_lexer_init(raw_input);
  } else {
    usage();
    return 1;
  }

  asm_lexer_process(lexer);
  program = lexer->instructions;
  if (lexer->errors->size > 0) {
    for (int i = 0; i < lexer->errors->size; i++) {
      LEXER_ERROR *error = LIST_GET(lexer->errors, LEXER_ERROR *, i);
      printf("ERR L%d: %s", error->line, error->message);
    }
    exit(1);
  }
  jump_table = lexer->jump_table;
  start = asm_lexer_get_start_line(lexer);

  MACHINE *machine = machine_init();
  machine_set_instructions(machine, program);
  machine_set_jump_table(machine, jump_table);

  uint32_t start_position = LIST_GET(machine->jump_table, uint32_t, start);
  machine->program_counter = start_position;
  if (fdebug) {
    machine_exec_program_debug(machine);
  } else {
    machine_exec_program(machine);
  }

  return 0;
}
#endif