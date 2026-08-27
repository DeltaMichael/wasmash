#ifndef CHAR_HELPER
#define CHAR_HELPER

#include <stdbool.h>

bool is_alpha(char symbol);
bool is_num(char symbol);
bool is_alnum(char symbol);
bool is_lineterm(char symbol);
bool is_whitespace(char symbol);
bool is_newline(char symbol);
bool is_forward_slash(char symbol);
bool is_comma(char symbol);

#endif

