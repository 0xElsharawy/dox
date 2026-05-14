#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/lexer.h"

static void lexer_advance(Lexer *lexer) {
  lexer->i++;
  lexer->c = lexer->src[lexer->i];
}

static int is_identifier_char(char c) { return isalnum(c) || c == '_'; }

static char lexer_peek(Lexer *lexer) {
  if (lexer->src[lexer->i + 1] == '\0') {
    return '\0';
  }
  return lexer->src[lexer->i + 1];
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

  if (strncmp(&lexer->src[lexer->i], "print", 5) == 0 &&
      !is_identifier_char(lexer->src[lexer->i + 5])) {
    token.type = TOKEN_KW_PRINT;
    token.value = "print";
    for (int j = 0; j < 5; ++j) {
      lexer_advance(lexer);
    }
    return token;
  }

  if (strncmp(&lexer->src[lexer->i], "int", 3) == 0 &&
      !is_identifier_char(lexer->src[lexer->i + 3])) {
    token.type = TOKEN_KW_INT;
    token.value = "int";
    for (int j = 0; j < 3; ++j) {
      lexer_advance(lexer);
    }
    return token;
  }

  if (strncmp(&lexer->src[lexer->i], "if", 2) == 0 &&
      !is_identifier_char(lexer->src[lexer->i + 2])) {
    token.type = TOKEN_KW_IF;
    token.value = "if";
    lexer_advance(lexer);
    lexer_advance(lexer);
    return token;
  }

  if (strncmp(&lexer->src[lexer->i], "else", 4) == 0 &&
      !is_identifier_char(lexer->src[lexer->i + 4])) {
    token.type = TOKEN_KW_ELSE;
    token.value = "else";
    for (int j = 0; j < 4; ++j) {
      lexer_advance(lexer);
    }
    return token;
  }

  if (strncmp(&lexer->src[lexer->i], "while", 5) == 0 &&
      !is_identifier_char(lexer->src[lexer->i + 5])) {
    token.type = TOKEN_KW_WHILE;
    token.value = "while";
    for (int j = 0; j < 5; ++j) {
      lexer_advance(lexer);
    }
    return token;
  }

  if (strncmp(&lexer->src[lexer->i], "return", 6) == 0 &&
      !is_identifier_char(lexer->src[lexer->i + 6])) {
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
    while (is_identifier_char(lexer->c)) {
      if (idx >= 64) {
        fprintf(stderr, "Identifier too long\n");
        exit(1);
      }
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

  if (lexer->c == '+' && lexer_peek(lexer) == '=') {
    token.type = TOKEN_PLUSEQ;
    token.value = "+=";
    lexer_advance(lexer);
    lexer_advance(lexer);
    return token;
  }

  if (lexer->c == '-' && lexer_peek(lexer) == '=') {
    token.type = TOKEN_MINUSEQ;
    token.value = "-=";
    lexer_advance(lexer);
    lexer_advance(lexer);
    return token;
  }

  if (lexer->c == '*' && lexer_peek(lexer) == '=') {
    token.type = TOKEN_STAREQ;
    token.value = "*=";
    lexer_advance(lexer);
    lexer_advance(lexer);
    return token;
  }

  if (lexer->c == '/' && lexer_peek(lexer) == '=') {
    token.type = TOKEN_SLASHEQ;
    token.value = "/=";
    lexer_advance(lexer);
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

  if (lexer->c == '=' && lexer_peek(lexer) == '=') {
    token.type = TOKEN_EQEQ;
    token.value = "==";
    lexer_advance(lexer);
    lexer_advance(lexer);
    return token;
  }

  if (lexer->c == '!' && lexer_peek(lexer) == '=') {
    token.type = TOKEN_NEQ;
    token.value = "!=";
    lexer_advance(lexer);
    lexer_advance(lexer);
    return token;
  }

  if (lexer->c == '<' && lexer_peek(lexer) == '=') {
    token.type = TOKEN_LTE;
    token.value = "<=";
    lexer_advance(lexer);
    lexer_advance(lexer);
    return token;
  }

  if (lexer->c == '>' && lexer_peek(lexer) == '=') {
    token.type = TOKEN_GTE;
    token.value = ">=";
    lexer_advance(lexer);
    lexer_advance(lexer);
    return token;
  }

  if (lexer->c == '<') {
    token.type = TOKEN_LT;
    token.value = "<";
    lexer_advance(lexer);
    return token;
  }

  if (lexer->c == '>') {
    token.type = TOKEN_GT;
    token.value = ">";
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
