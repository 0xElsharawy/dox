#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/ast.h"

static const char *node_type_to_string(ASTNodeType type) {
  switch (type) {
  case AST_NUMBER:
    return "Number";
  case AST_RETURN:
    return "Return";
  case AST_FUNCTION_CALL:
    return "FunctionCall";
  case AST_FUNCTION_DECL:
    return "FunctionDecl";
  case AST_IDENTIFIER:
    return "Identifier";
  case AST_BLOCK:
    return "Block";
  default:
    return "Unknown";
  }
}

static void print_indent(int depth) {
  for (int i = 0; i < depth; i++) {
    printf("  ");
  }
}

void print_ast(const ASTNode *node, int depth) {
  if (node == NULL) {
    print_indent(depth);
    printf("(NULL)\n");
    return;
  }

  print_indent(depth);

  switch (node->type) {
  case AST_NUMBER:
    printf("AST_NUMBER: %d\n", node->number.value);
    break;

  case AST_IDENTIFIER:
    printf("AST_IDENTIFIER: %s\n", node->identifier.name);
    break;

  case AST_RETURN:
    printf("AST_RETURN\n");
    print_ast(node->return_stmt.expr, depth + 1);
    break;

  case AST_FUNCTION_CALL:
    printf("AST_FUNCTION_CALL: %s (%zu args)\n", node->function_call.name,
           node->function_call.arg_count);
    for (size_t i = 0; i < node->function_call.arg_count; i++) {
      print_ast(node->function_call.args[i], depth + 1);
    }
    break;

  case AST_FUNCTION_DECL:
    printf("AST_FUNCTION_DECL: %s -> %s\n", node->function_decl.name,
           node->function_decl.return_type);
    print_ast(node->function_decl.body, depth + 1);
    break;

  case AST_BLOCK:
    printf("AST_BLOCK (%zu statements)\n", node->block.statement_count);
    for (size_t i = 0; i < node->block.statement_count; i++) {
      print_ast(node->block.statements[i], depth + 1);
    }
    break;

  default:
    printf("UNKNOWN_NODE_TYPE\n");
    break;
  }
}

ASTNode *ast_number(int value) {
  ASTNode *node = malloc(sizeof(ASTNode));
  node->type = AST_NUMBER;
  node->number.value = value;
  return node;
}

ASTNode *ast_identifier(const char *name) {
  ASTNode *node = malloc(sizeof(ASTNode));
  node->type = AST_IDENTIFIER;
  node->identifier.name = strdup(name);
  return node;
}

ASTNode *ast_function_decl(const char *name, const char *return_type,
                           ASTNode *body) {
  ASTNode *node = malloc(sizeof(ASTNode));
  node->type = AST_FUNCTION_DECL;
  node->function_decl.name = strdup(name);
  node->function_decl.return_type = strdup(return_type);
  node->function_decl.body = body;
  return node;
}

ASTNode *ast_block(ASTNode **statements, size_t count) {
  ASTNode *node = malloc(sizeof(ASTNode));
  node->type = AST_BLOCK;
  node->block.statements = statements;
  node->block.statement_count = count;
  return node;
}

ASTNode *ast_return(ASTNode *expr) {
  ASTNode *node = malloc(sizeof(ASTNode));
  node->type = AST_RETURN;
  node->return_stmt.expr = expr;
  return node;
}
