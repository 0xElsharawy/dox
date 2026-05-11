#include "../include/lexer.h"
#include <ctype.h>
#include <string.h>

static void lexer_advance(Lexer *lexer) {
  lexer->i++;
  lexer->c = lexer->src[lexer->i];
}

void lexer_init(Lexer *lexer, const char *src) {
  lexer->src = src;
  lexer->i = 0;
  lexer->c = src[0];
}

Token lexer_next_token(Lexer *lexer) {
  Token token;

  while (lexer->c == ' ' || lexer->c == '\t' || lexer->c == '\n') {
    lexer_advance(lexer);
  }

  if (lexer->c == '\0') {
    token.type = TOKEN_EOF;
    token.value = NULL;
    return token;
  }

  if (strncmp(&lexer->src[lexer->i], "int", 3) == 0) {
    token.type = TOKEN_KW_INT;
    token.value = "int";
    for (int j = 0; j < 3; ++j) {
      lexer_advance(lexer);
    }
    return token;
  }

  if (strncmp(&lexer->src[lexer->i], "return", 6) == 0) {
    token.type = TOKEN_KW_RETURN;
    token.value = "return";
    for (int j = 0; j < 6; ++j) {
      lexer_advance(lexer);
    }
    return token;
  }

  if (isalpha(lexer->c) || lexer->c == '_') {
    token.type = TOKEN_IDENTIFIER;
    char buffer[64];
    int idx = 0;
    while (isalnum(lexer->c) || lexer->c == '_') {
      buffer[idx++] = lexer->c;
      lexer_advance(lexer);
    }
    buffer[idx] = '\0';
    token.value = strdup(buffer);
    return token;
  }

  if (lexer->c >= '0' && lexer->c <= '9') {
    token.type = TOKEN_NUMBER;
    char buffer[32];
    int idx = 0;
    while (lexer->c >= '0' && lexer->c <= '9') {
      buffer[idx++] = lexer->c;
      lexer_advance(lexer);
    }
    buffer[idx] = '\0';
    token.value = strdup(buffer);
    return token;
  }

  if (lexer->c == '(') {
    token.type = TOKEN_LPAREN;
    token.value = "(";
    lexer_advance(lexer);
    return token;
  }

  if (lexer->c == ')') {
    token.type = TOKEN_RPAREN;
    token.value = ")";
    lexer_advance(lexer);
    return token;
  }

  if (lexer->c == '{') {
    token.type = TOKEN_LBRACE;
    token.value = "{";
    lexer_advance(lexer);
    return token;
  }

  if (lexer->c == '}') {
    token.type = TOKEN_RBRACE;
    token.value = "}";
    lexer_advance(lexer);
    return token;
  }

  if (lexer->c == '[') {
    token.type = TOKEN_LBRACK;
    token.value = "[";
    lexer_advance(lexer);
    return token;
  }

  if (lexer->c == ']') {
    token.type = TOKEN_RBRACK;
    token.value = "]";
    lexer_advance(lexer);
    return token;
  }

  if (lexer->c == '+') {
    token.type = TOKEN_PLUS;
    token.value = "+";
    lexer_advance(lexer);
    return token;
  }

  if (lexer->c == '-') {
    token.type = TOKEN_MINUS;
    token.value = "-";
    lexer_advance(lexer);
    return token;
  }

  if (lexer->c == '*') {
    token.type = TOKEN_STAR;
    token.value = "*";
    lexer_advance(lexer);
    return token;
  }

  if (lexer->c == '/') {
    token.type = TOKEN_SLASH;
    token.value = "/";
    lexer_advance(lexer);
    return token;
  }

  if (lexer->c == '!') {
    token.type = TOKEN_BANG;
    token.value = "!";
    lexer_advance(lexer);
    return token;
  }

  if (lexer->c == '~') {
    token.type = TOKEN_TILDE;
    token.value = "~";
    lexer_advance(lexer);
    return token;
  }

  if (lexer->c == '=') {
    token.type = TOKEN_ASSIGN;
    token.value = "=";
    lexer_advance(lexer);
    return token;
  }

  if (lexer->c == ';') {
    token.type = TOKEN_SEMICOLON;
    token.value = ";";
    lexer_advance(lexer);
    return token;
  }

  token.type = TOKEN_INVALID;
  token.value = NULL;
  lexer_advance(lexer);

  return token;
}
