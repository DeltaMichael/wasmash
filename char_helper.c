#include "include/char_helper.h"

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

bool is_comma(char symbol) { return symbol == ','; }

