#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/ast.h"

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
  case AST_NUMBER: {
    printf("AST_NUMBER: %d\n", node->number.value);
    break;
  }

  case AST_IF: {
    printf("AST_IF\n");
    print_ast(node->if_stmt.condition, depth + 1);
    print_ast(node->if_stmt.then_branch, depth + 1);
    if (node->if_stmt.else_branch) {
      print_ast(node->if_stmt.else_branch, depth + 1);
    }
    break;
  }

  case AST_ELSE: {
    printf("AST_ELSE\n");
    print_ast(node->else_stmt.condition, depth + 1);
    print_ast(node->else_stmt.then_branch, depth + 1);
    break;
  }

  case AST_VARIABLE: {
    printf("AST_VARIABLE: %s\n", node->variable.name);
    break;
  }
  case AST_VARIABLE_DECL: {
    printf("AST_VARIABLE_DECL: %s -> %s\n", node->variable_decl.name,
           node->variable_decl.type);
    print_ast(node->variable_decl.expr, depth + 1);
    break;
  }
  case AST_ASSIGN: {
    printf("AST_ASSIGN: %s\n", node->assign.name);
    print_ast(node->assign.expr, depth + 1);
    break;
  }

  case AST_RETURN: {
    printf("AST_RETURN\n");
    print_ast(node->return_stmt.expr, depth + 1);
    break;
  }

  case AST_BINARY_OP: {
    printf("AST_BINARY_OP: %s\n", node->binary_op.op.value);
    print_ast(node->binary_op.left, depth + 1);
    print_ast(node->binary_op.right, depth + 1);
    break;
  }

  case AST_UNARY_OP: {
    printf("AST_UNARY_OP: %s\n", node->unary_op.op.value);
    print_ast(node->unary_op.operand, depth + 1);
    break;
  }

  case AST_FUNCTION_CALL: {
    printf("AST_FUNCTION_CALL: %s (%zu args)\n", node->function_call.name,
           node->function_call.arg_count);
    for (size_t i = 0; i < node->function_call.arg_count; i++) {
      print_ast(node->function_call.args[i], depth + 1);
    }
    break;
  }

  case AST_FUNCTION_DECL: {
    printf("AST_FUNCTION_DECL: %s -> %s\n", node->function_decl.name,
           node->function_decl.return_type);
    print_ast(node->function_decl.body, depth + 1);
    break;
  }

  case AST_BLOCK: {
    printf("AST_BLOCK (%zu statements)\n", node->block.statement_count);
    for (size_t i = 0; i < node->block.statement_count; i++) {
      print_ast(node->block.statements[i], depth + 1);
    }
    break;
  }

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

ASTNode *ast_if(ASTNode *condition, ASTNode *then_branch,
                ASTNode *else_branch) {
  ASTNode *node = malloc(sizeof(ASTNode));
  node->type = AST_IF;
  node->if_stmt.condition = condition;
  node->if_stmt.then_branch = then_branch;
  node->if_stmt.else_branch = else_branch;
  return node;
}

ASTNode *ast_else(ASTNode *condition, ASTNode *then_branch) {
  ASTNode *node = malloc(sizeof(ASTNode));
  node->type = AST_ELSE;
  node->else_stmt.condition = condition;
  node->else_stmt.then_branch = then_branch;
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

ASTNode *ast_binary_op(Token op, ASTNode *left, ASTNode *right) {
  ASTNode *node = malloc(sizeof(ASTNode));
  node->type = AST_BINARY_OP;
  node->binary_op.op = op;
  node->binary_op.left = left;
  node->binary_op.right = right;
  return node;
}

ASTNode *ast_unary_op(Token op, ASTNode *operand) {
  ASTNode *node = malloc(sizeof(ASTNode));
  node->type = AST_UNARY_OP;
  node->unary_op.op = op;
  node->unary_op.operand = operand;
  return node;
}

ASTNode *ast_variable_decl(const char *name, const char *type, ASTNode *expr) {
  ASTNode *node = malloc(sizeof(ASTNode));
  node->type = AST_VARIABLE_DECL;
  node->variable_decl.name = strdup(name);
  node->variable_decl.type = strdup(type);
  node->variable_decl.expr = expr;
  return node;
}

ASTNode *ast_variable(const char *name) {
  ASTNode *node = malloc(sizeof(ASTNode));
  node->type = AST_VARIABLE;
  node->variable.name = strdup(name);
  return node;
}

ASTNode *ast_assign(const char *name, ASTNode *expr) {
  ASTNode *node = malloc(sizeof(ASTNode));
  node->type = AST_ASSIGN;
  node->assign.name = strdup(name);
  node->assign.expr = expr;
  return node;
}
