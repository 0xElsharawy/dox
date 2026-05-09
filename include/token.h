#ifndef TOKEN_H
#define TOKEN_H

#include <stddef.h>

typedef enum {
  TOKEN_IDENTIFIER,
  TOKEN_NUMBER,

  TOKEN_KW_INT,
  TOKEN_KW_RETURN,

  TOKEN_SEMICOLON,

  TOKEN_LPAREN,
  TOKEN_RPAREN,
  TOKEN_LBRACE,
  TOKEN_RBRACE,

  TOKEN_EOF,
  TOKEN_INVALID,
} TokenType;

typedef struct {
  TokenType type;
  char *value;

} Token;

const char *token_type_to_string(TokenType type);

#endif
