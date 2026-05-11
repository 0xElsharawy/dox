#include "../include/token.h"

const char *token_type_to_string(TokenType type) {
  switch (type) {
  case TOKEN_IDENTIFIER:
    return "TOKEN_IDENTIFIER";
  case TOKEN_NUMBER:
    return "TOKEN_NUMBER";

  case TOKEN_KW_INT:
    return "TOKEN_KW_INT";
  case TOKEN_KW_IF:
    return "TOKEN_KW_IF";
  case TOKEN_KW_ELSE:
    return "TOKEN_KW_ELSE";
  case TOKEN_KW_RETURN:
    return "TOKEN_KW_RETURN";

  case TOKEN_EQEQ:
    return "TOKEN_EQEQ";
  case TOKEN_NEQ:
    return "TOKEN_NEQ";
  case TOKEN_LT:
    return "TOKEN_LT";
  case TOKEN_GT:
    return "TOKEN_GT";
  case TOKEN_LTE:
    return "TOKEN_LTE";
  case TOKEN_GTE:
    return "TOKEN_GTE";

  case TOKEN_SEMICOLON:
    return "TOKEN_SEMICOLON";

  case TOKEN_LPAREN:
    return "TOKEN_LPAREN";
  case TOKEN_RPAREN:
    return "TOKEN_RPAREN";
  case TOKEN_LBRACE:
    return "TOKEN_LBRACE";
  case TOKEN_RBRACE:
    return "TOKEN_RBRACE";

  case TOKEN_PLUS:
    return "TOKEN_PLUS";
  case TOKEN_MINUS:
    return "TOKEN_MINUS";
  case TOKEN_STAR:
    return "TOKEN_STAR";
  case TOKEN_SLASH:
    return "TOKEN_SLASH";
  case TOKEN_BANG:
    return "TOKEN_BANG";
  case TOKEN_TILDE:
    return "TOKEN_TILDE";

  case TOKEN_ASSIGN:
    return "TOKEN_ASSIGN";

  case TOKEN_EOF:
    return "TOKEN_EOF";

  default:
    return "TOKEN_INVALID";
  }
}
