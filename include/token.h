#ifndef TOKEN_H
#define TOKEN_H

#include <stddef.h>

typedef enum {
  TOKEN_IDENTIFIER,
  TOKEN_NUMBER,

  TOKEN_KW_PRINT,
  TOKEN_KW_INT,
  TOKEN_KW_IF,
  TOKEN_KW_ELSE,
  TOKEN_KW_WHILE,
  TOKEN_KW_RETURN,

  TOKEN_EQEQ,
  TOKEN_NEQ,
  TOKEN_LT,
  TOKEN_GT,
  TOKEN_LTE,
  TOKEN_GTE,

  TOKEN_SEMICOLON,

  TOKEN_LPAREN,
  TOKEN_RPAREN,
  TOKEN_LBRACE,
  TOKEN_RBRACE,

  TOKEN_PLUS,
  TOKEN_MINUS,
  TOKEN_STAR,
  TOKEN_SLASH,
  TOKEN_BANG,
  TOKEN_TILDE,

  TOKEN_ASSIGN,

  TOKEN_EOF,
  TOKEN_INVALID,
} TokenType;

typedef struct {
  TokenType type;
  char *value;

} Token;

const char *token_type_to_string(TokenType type);

#endif
