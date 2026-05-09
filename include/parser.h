#ifndef PARSER_H
#define PARSER_H

#include "ast.h"
#include "token.h"

#include <stddef.h>

typedef struct {
  Token *tokens;
  size_t token_count;
  size_t position;
} Parser;

void parser_init(Parser *parser, Token *tokens, size_t token_count);

ASTNode *parser_parse(Parser *parser);

#endif
