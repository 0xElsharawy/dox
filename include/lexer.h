#ifndef LEXER_H
#define LEXER_H

#include "token.h"

typedef struct {
  const char *input;
  int i;
  char c;

} Lexer;

void lexer_init(Lexer *lexer, const char *input);

Token lexer_next_token(Lexer *lexer);

#endif
