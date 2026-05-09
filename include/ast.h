#ifndef AST_H
#define AST_H

#include "token.h"

#include <stddef.h>

typedef enum {
  AST_NUMBER,
  AST_IDENTIFIER,
  AST_RETURN,
  AST_FUNCTION_CALL,
  AST_FUNCTION_DECL,
  AST_BLOCK,
} ASTNodeType;

typedef struct ASTNode {
  ASTNodeType type;
  const char *detail;

  union {
    struct {
      int value;
    } number;

    struct {
      char *name;
    } identifier;

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
  };
} ASTNode;

ASTNode *ast_number(int value);
ASTNode *ast_identifier(const char *name);
ASTNode *ast_function_decl(const char *name, const char *return_type,
                           ASTNode *body);
ASTNode *ast_block(ASTNode **statements, size_t count);
ASTNode *ast_return(ASTNode *expr);

void ast_print(const ASTNode *node, int depth);

#endif
