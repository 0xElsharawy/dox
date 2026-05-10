#include <stdio.h>
#include <string.h>

#include "../include/codegen.h"

static void gen_function(ASTNode *node, FILE *out);
static void gen_expr(ASTNode *node, FILE *out);
static void gen_stmt(ASTNode *node, FILE *out);

static void gen_function(ASTNode *node, FILE *out) {
  fprintf(out, ".global %s\n\n", node->function_decl.name);
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
  case AST_BINARY_OP:
    gen_expr(node->binary_op.left, out);
    fprintf(out, "    push rax\n");
    gen_expr(node->binary_op.right, out);
    fprintf(out, "    mov rbx, rax\n");
    fprintf(out, "    pop rax\n");
    switch (*node->binary_op.op.value) {
    case '+':
      fprintf(out, "    add rax, rbx\n");
      break;
    case '-':
      fprintf(out, "    sub rax, rbx\n");
      break;
    case '*':
      fprintf(out, "    imul rax, rbx\n");
      break;
    case '/':
      fprintf(out, "    cqo\n");
      fprintf(out, "    idiv rbx\n");
      break;
    default:
      break;
    }
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
