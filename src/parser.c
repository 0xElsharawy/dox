#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/parser.h"

static Token *current_token(Parser *parser) {
  return &parser->tokens[parser->position];
}

static void advance(Parser *parser) {
  if (parser->position < parser->token_count) {
    parser->position++;
  }
}

static int match(Parser *parser, TokenType type) {
  return current_token(parser)->type == type;
}

static void consume(Parser *parser, TokenType type, const char *err_msg) {
  if (match(parser, type)) {
    advance(parser);
    return;
  }
  fprintf(stderr, "Syntax Error: %s (Got token type %d)\n", err_msg,
          current_token(parser)->type);
  exit(1);
}

void parser_init(Parser *parser, Token *tokens, size_t token_count) {
  parser->tokens = tokens;
  parser->token_count = token_count;
  parser->position = 0;
}

static ASTNode *parse_expression(Parser *parser);
static ASTNode *parse_additive(Parser *parser);
static ASTNode *parse_comparison(Parser *parser);
static ASTNode *parse_term(Parser *parser);
static ASTNode *parse_factor(Parser *parser);
static ASTNode *parse_primary(Parser *parser);
static ASTNode *parse_statement(Parser *parser);
static ASTNode *parse_block(Parser *parser);
static ASTNode *parse_function_decl(Parser *parser);
static ASTNode *parse_unary(Parser *parser);
static ASTNode *parse_variable_decl(Parser *parser);
static ASTNode *parse_if(Parser *parser);

static ASTNode *parse_expression(Parser *parser) {
  return parse_comparison(parser);
}

static ASTNode *parse_primary(Parser *parser) {
  Token *token = current_token(parser);

  if (token->type == TOKEN_NUMBER) {
    int value = atoi(token->value);
    advance(parser);
    return ast_number(value);
  }

  if (token->type == TOKEN_IDENTIFIER) {
    char *name = token->value;
    advance(parser);
    return ast_variable(name);
  }

  if (token->type == TOKEN_LPAREN) {
    advance(parser);
    ASTNode *expr = parse_expression(parser);
    consume(parser, TOKEN_RPAREN, "Expected ')' after expression");
    return expr;
  }

  fprintf(stderr, "Syntax Error: Expected expression\n");
  exit(1);
}

static ASTNode *parse_factor(Parser *parser) { return parse_unary(parser); }

static ASTNode *parse_term(Parser *parser) {
  ASTNode *left = parse_factor(parser);

  while (match(parser, TOKEN_STAR) || match(parser, TOKEN_SLASH)) {
    Token token = *current_token(parser);
    consume(parser, token.type, "Expected '*' or '/' operator");
    ASTNode *right = parse_factor(parser);
    left = ast_binary_op(token, left, right);
  }

  return left;
}

static ASTNode *parse_additive(Parser *parser) {
  ASTNode *left = parse_term(parser);

  while (match(parser, TOKEN_PLUS) || match(parser, TOKEN_MINUS)) {
    Token token = *current_token(parser);
    consume(parser, token.type, "Expected '+' or '-' operator");
    ASTNode *right = parse_term(parser);
    left = ast_binary_op(token, left, right);
  }

  return left;
}

static ASTNode *parse_comparison(Parser *parser) {
  ASTNode *left = parse_additive(parser);

  while (match(parser, TOKEN_EQEQ) || match(parser, TOKEN_NEQ) ||
         match(parser, TOKEN_LT) || match(parser, TOKEN_GT) ||
         match(parser, TOKEN_LTE) || match(parser, TOKEN_GTE)) {
    Token token = *current_token(parser);
    advance(parser);

    ASTNode *right = parse_additive(parser);

    left = ast_binary_op(token, left, right);
  }

  return left;
}

static ASTNode *parse_return(Parser *parser) {
  consume(parser, TOKEN_KW_RETURN, "Expected 'return'");

  ASTNode *expr = parse_expression(parser);

  consume(parser, TOKEN_SEMICOLON, "Expected ';' after return value");
  return ast_return(expr);
}

static ASTNode *parse_statement(Parser *parser) {
  if (match(parser, TOKEN_KW_INT)) {
    return parse_variable_decl(parser);
  }

  if (match(parser, TOKEN_KW_IF)) {
    return parse_if(parser);
  }

  if (match(parser, TOKEN_IDENTIFIER)) {
    Token *name = current_token(parser);

    if (parser->position + 1 < parser->token_count &&
        parser->tokens[parser->position + 1].type == TOKEN_ASSIGN) {
      advance(parser);
      advance(parser);
      ASTNode *expr = parse_expression(parser);
      consume(parser, TOKEN_SEMICOLON, "Expected ';' after assignment");
      return ast_assign(name->value, expr);
    }
  }

  if (match(parser, TOKEN_KW_RETURN)) {
    return parse_return(parser);
  }

  fprintf(stderr,
          "Syntax Error: Unexpected statement starting with token type %d\n",
          current_token(parser)->type);
  exit(1);
}

static ASTNode *parse_block(Parser *parser) {
  consume(parser, TOKEN_LBRACE, "Expected '{' to start a block statement");

  size_t capacity = 4;
  size_t count = 0;
  ASTNode **statements = malloc(capacity * sizeof(ASTNode *));

  while (!match(parser, TOKEN_RBRACE) && !match(parser, TOKEN_EOF)) {
    if (count >= capacity) {
      capacity *= 2;
      statements = realloc(statements, capacity * sizeof(ASTNode *));
    }
    statements[count++] = parse_statement(parser);
  }

  consume(parser, TOKEN_RBRACE, "Expected '}' to close a block statement");
  return ast_block(statements, count);
}

static ASTNode *parse_function_decl(Parser *parser) {
  Token *type_token = current_token(parser);
  consume(parser, TOKEN_KW_INT, "Expected function return type");

  Token *name_token = current_token(parser);
  consume(parser, TOKEN_IDENTIFIER, "Expected function name identifier");

  consume(parser, TOKEN_LPAREN, "Expected '(' after function name");
  consume(parser, TOKEN_RPAREN, "Expected ')' after '('");

  ASTNode *body = parse_block(parser);

  return ast_function_decl(name_token->value, type_token->value, body);
}

static ASTNode *parse_unary(Parser *parser) {
  if (match(parser, TOKEN_BANG) || match(parser, TOKEN_MINUS) ||
      match(parser, TOKEN_TILDE)) {
    Token token = *current_token(parser);
    consume(parser, token.type, "Expected unary operator");
    ASTNode *operand = parse_unary(parser);
    return ast_unary_op(token, operand);
  }
  return parse_primary(parser);
}

static ASTNode *parse_variable_decl(Parser *parser) {
  consume(parser, TOKEN_KW_INT, "Expected variable type");

  Token *name_token = current_token(parser);
  consume(parser, TOKEN_IDENTIFIER, "Expected variable name identifier");

  ASTNode *init_expr = NULL;
  if (match(parser, TOKEN_ASSIGN)) {
    advance(parser);
    init_expr = parse_expression(parser);
  }

  consume(parser, TOKEN_SEMICOLON, "Expected ';' after variable declaration");
  return ast_variable_decl(name_token->value, "int", init_expr);
}

static ASTNode *parse_if(Parser *parser) {
  consume(parser, TOKEN_KW_IF, "Expected 'if'");

  consume(parser, TOKEN_LPAREN, "Expected '(' after 'if'");
  ASTNode *condition = parse_expression(parser);
  consume(parser, TOKEN_RPAREN, "Expected ')' after if condition");

  ASTNode *then_branch = parse_block(parser);

  ASTNode *else_branch = NULL;
  if (match(parser, TOKEN_KW_ELSE)) {
    advance(parser);

    if (match(parser, TOKEN_KW_IF)) {
      else_branch = parse_if(parser);
    } else {
      else_branch = parse_block(parser);
    }
  }

  return ast_if(condition, then_branch, else_branch);
}

ASTNode *parser_parse(Parser *parser) { return parse_function_decl(parser); }
