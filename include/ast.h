#ifndef AST_H
#define AST_H

#include "token.h"

#include <stddef.h>

typedef enum {
  AST_NUMBER,
  AST_RETURN,
  AST_BINARY_OP,
  AST_UNARY_OP,
  AST_FUNCTION_CALL,
  AST_FUNCTION_DECL,
  AST_BLOCK,
  AST_VARIABLE_DECL,
  AST_VARIABLE,
  AST_ASSIGN,
} ASTNodeType;

typedef struct ASTNode {
  ASTNodeType type;

  union {
    struct {
      int value;
    } number;

    struct {
      struct ASTNode *expr;
    } return_stmt;

    struct {
      char *name;
      struct ASTNode **args;
      size_t arg_count;
    } function_call;

    struct {
      char *name;
      int param_count;
      char **param_types;
      char **param_names;
      char *return_type;
      struct ASTNode *body;
    } function_decl;

    struct {
      struct ASTNode **statements;
      size_t statement_count;
    } block;

    struct {
      Token op;
      struct ASTNode *left;
      struct ASTNode *right;
    } binary_op;

    struct {
      Token op;
      struct ASTNode *operand;
    } unary_op;

    struct {
      char *name;
      char *type;
      struct ASTNode *expr;
    } variable_decl;

    struct {
      char *name;
    } variable;

    struct {
      char *name;
      struct ASTNode *expr;
    } assign;
  };
} ASTNode;

ASTNode *ast_number(int value);
ASTNode *ast_function_decl(const char *name, const char *return_type,
                           ASTNode *body);
ASTNode *ast_block(ASTNode **statements, size_t count);
ASTNode *ast_return(ASTNode *expr);
ASTNode *ast_binary_op(Token op, ASTNode *left, ASTNode *right);
ASTNode *ast_unary_op(Token op, ASTNode *operand);
ASTNode *ast_variable_decl(const char *name, const char *type, ASTNode *expr);
ASTNode *ast_variable(const char *name);
ASTNode *ast_assign(const char *name, ASTNode *expr);

void print_ast(const ASTNode *node, int depth);

#endif
