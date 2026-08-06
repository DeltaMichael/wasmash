#ifndef INSTRUCTION_H
#define INSTRUCTION_H
#include "stack.h"
#include <stdint.h>

typedef enum {
  ADD_8,
  SUB_8,
  MUL_8,
  DIV_8,
  PUSH_8,
  POP_8,
  PRINT_8, // TODO: There should be a way to implement this with syscall, cause
           // it's faster
  PRSP_8,  // push the value at an address specified by an offset to the stack pointer on top of the stack
  PABS_8, // push the value at an absolute address on top of the stack
  LRSP_8,     // load data on top of the stack to address relative to the stack
              // pointer
  LABS_8,     // load data on top of the stack to absolute address
  JMP_8,      // jump to line
  CMP_8,      // compare two values
  JZ_8,       // jump if comparison result is zero
  JNZ_8,      // jump if comparison result is non-zero
  CALL_8,     // call a function at label
  RET_8,	  // return the value on top of the stack
  NOP,        // no-op
} OPCODE;

typedef struct {
  char* name;
  OPCODE opcode;
  uint8_t *data;
} INSTRUCTION;

INSTRUCTION *instruction_create(const char* name, OPCODE code, uint8_t *data);
INSTRUCTION *instruction_create_with_value(const char* name, OPCODE code, uint8_t data);

#endif

