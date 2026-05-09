#include <stdio.h>
#include <string.h>

#include "../include/code_gen.h"

static void gen_function(ASTNode *node, FILE *out);
static void gen_expr(ASTNode *node, FILE *out);
static void gen_stmt(ASTNode *node, FILE *out);

static void gen_function(ASTNode *node, FILE *out) {
  if (strcmp(node->function_decl.name, "main") == 0) {
    fprintf(out, ".intel_syntax noprefix\n");
    fprintf(out, ".global main\n\n");
  }

  fprintf(out, "%s:\n", node->function_decl.name);
  gen_stmt(node->function_decl.body, out);
}

static void gen_stmt(ASTNode *node, FILE *out) {
  switch (node->type) {
  case AST_RETURN:
    gen_expr(node->return_stmt.expr, out);
    fprintf(out, "    ret\n");
    break;
  case AST_BLOCK:
    for (size_t i = 0; i < node->block.statement_count; i++) {
      gen_stmt(node->block.statements[i], out);
    }
    break;
  default:
    break;
  }
}

static void gen_expr(ASTNode *node, FILE *out) {
  switch (node->type) {
  case AST_NUMBER:
    fprintf(out, "    mov rax, %d\n", node->number.value);
    break;
  default:
    break;
  }
}

void gen_code(ASTNode *node, FILE *out) {
  switch (node->type) {
  case AST_FUNCTION_DECL:
    gen_function(node, out);
    break;
  default:
    break;
  }
}
