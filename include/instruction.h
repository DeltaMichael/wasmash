#ifndef INSTRUCTION_H
#define INSTRUCTION_H
#include "stack.h"
#include <stdint.h>

typedef enum {
  ADD_8,  // pop two values off the stack, add them and push the result
  SUB_8,  // pop two values off the stack and subtract them, e.g. subtracted =
          // top, subtractee = top - 1 and push the result
  MUL_8,  // pop two values of the stack, multiply them and push the result
  DIV_8,  // divide the values on top of the stack, e.g. divider = top, divident
          // = top - 1
  MOD_8,  // modulo the values on top of the stack, e.g. divider = top, divident
          // = top - 1
  PUSH_8, // push a value
  POP_8,  // pop a value
  PRINT_8, // print the value on top of the stack

  PRSP_8,  // push the value at an address specified by an offset to the stack
           // pointer on top of the stack
  PRT_8,   // push the value on top at an address specified by an offset to the
           // top, e.g. prt8 1 addresses top - 1
  PABS_8,  // push the value at an absolute address on top of the stack
  LRSP_8,  // load data on top of the stack to address relative to the stack
           // pointer
  LABS_8,  // load data on top of the stack to absolute address
  JMP_8,   // jump to line
  CMP_8,   // compare two values
  JZ_8,    // jump if comparison result is zero, e.g. a != b will give a zero
           // result
  JNZ_8,   // jump if comparison result is non-zero, e.g. a == b will give a
           // non-zero result
  CALL_8,  // call a function at label
  ENTER_8, // enter new stackframe with arguments
  RET_8,   // return the value on top of the stack and push it on top of the
           // caller stack frame
  NOP,     // no-op
} OPCODE;

typedef struct {
  char* name;
  OPCODE opcode;
  uint8_t *data;
} INSTRUCTION;

INSTRUCTION *instruction_create(const char* name, OPCODE code, uint8_t *data);
INSTRUCTION *instruction_create_with_value(const char* name, OPCODE code, uint8_t data);

#endif

