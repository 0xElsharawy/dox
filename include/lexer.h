#ifndef LEXER_H
#define LEXER_H

#include "token.h"

typedef struct {
  const char *src;
  int i;
  char c;

} Lexer;

void lexer_init(Lexer *lexer, const char *src);

Token lexer_next_token(Lexer *lexer);

#endif
