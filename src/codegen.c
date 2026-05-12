#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/codegen.h"

LocalVar locals[256];
int local_count;
int local_offset;
int label_count;
int return_label;

static int find_local(const char *name) {
  for (int i = 0; i < local_count; i++) {
    if (strcmp(locals[i].name, name) == 0) {
      return locals[i].offset;
    }
  }
  fprintf(stderr, "Undefined variable: %s\n", name);
  exit(1);
}

static void gen_function(ASTNode *node, FILE *out);
static void gen_expr(ASTNode *node, FILE *out);
static void gen_stmt(ASTNode *node, FILE *out);

static void gen_function(ASTNode *node, FILE *out) {
  local_count = 0;
  local_offset = 0;

  return_label = label_count++;

  fprintf(out, ".global %s\n\n", node->function_decl.name);
  fprintf(out, "%s:\n", node->function_decl.name);

  fprintf(out, "    push rbp\n");
  fprintf(out, "    mov rbp, rsp\n");
  fprintf(out, "    sub rsp, 256\n");

  gen_stmt(node->function_decl.body, out);

  fprintf(out, ".L%d:\n", return_label);
  fprintf(out, "    mov rsp, rbp\n");
  fprintf(out, "    pop rbp\n");
  fprintf(out, "    ret\n");
}

static void gen_stmt(ASTNode *node, FILE *out) {
  switch (node->type) {
  case AST_VARIABLE_DECL: {
    local_offset += 8;
    locals[local_count].name = node->variable_decl.name;
    locals[local_count].offset = local_offset;
    local_count++;
    if (node->variable_decl.expr) {
      gen_expr(node->variable_decl.expr, out);
      fprintf(out, "    mov [rbp-%d], rax\n", local_offset);
    }
    break;
  }

  case AST_ASSIGN: {
    gen_expr(node->assign.expr, out);
    int offset = find_local(node->assign.name);
    fprintf(out, "    mov [rbp-%d], rax\n", offset);
    break;
  }

  case AST_PRINT: {
    gen_expr(node->print_stmt.expr, out);
    fprintf(out, "    mov rdi, rax\n");
    fprintf(out, "    call print_int\n");
    break;
  }

  case AST_IF: {
    gen_expr(node->if_stmt.condition, out);
    fprintf(out, "    cmp rax, 0\n");

    if (node->if_stmt.else_branch) {
      int else_label = label_count++;
      int end_label = label_count++;

      fprintf(out, "    je .L%d\n", else_label);

      gen_stmt(node->if_stmt.then_branch, out);
      fprintf(out, "    jmp .L%d\n", end_label);

      fprintf(out, ".L%d:\n", else_label);
      gen_stmt(node->if_stmt.else_branch, out);

      fprintf(out, ".L%d:\n", end_label);
    } else {
      int end_label = label_count++;

      fprintf(out, "    je .L%d\n", end_label);

      gen_stmt(node->if_stmt.then_branch, out);

      fprintf(out, ".L%d:\n", end_label);
    }

    break;
  }

  case AST_WHILE: {
    int start_label = label_count++;
    int end_label = label_count++;

    fprintf(out, ".L%d:\n", start_label);

    gen_expr(node->while_stmt.condition, out);
    fprintf(out, "    cmp rax, 0\n");
    fprintf(out, "    je .L%d\n", end_label);

    gen_stmt(node->while_stmt.body, out);

    fprintf(out, "    jmp .L%d\n", start_label);

    fprintf(out, ".L%d:\n", end_label);
    break;
  }

  case AST_RETURN: {
    gen_expr(node->return_stmt.expr, out);
    fprintf(out, "    jmp .L%d\n", return_label);
    break;
  }

  case AST_BLOCK: {
    for (size_t i = 0; i < node->block.statement_count; i++) {
      gen_stmt(node->block.statements[i], out);
    }
    break;
  }

  default:
    break;
  }
}

static void gen_expr(ASTNode *node, FILE *out) {
  switch (node->type) {
  case AST_NUMBER: {
    fprintf(out, "    mov rax, %d\n", node->number.value);
    break;
  }
  case AST_VARIABLE: {
    int offset = find_local(node->variable.name);
    fprintf(out, "    mov rax, [rbp-%d]\n", offset);
    break;
  }
  case AST_UNARY_OP: {
    gen_expr(node->unary_op.operand, out);
    switch (*node->unary_op.op.value) {
    case '-': {
      fprintf(out, "    neg rax\n");
      break;
    }
    case '!': {
      fprintf(out, "    cmp rax, 0\n");
      fprintf(out, "    sete al\n");
      fprintf(out, "    movzx rax, al\n");
      break;
    }
    case '~': {
      fprintf(out, "    not rax\n");
      break;
    }
    default:
      break;
    }
    break;
  }
  case AST_BINARY_OP: {
    gen_expr(node->binary_op.left, out);
    fprintf(out, "    push rax\n");

    gen_expr(node->binary_op.right, out);
    fprintf(out, "    mov rbx, rax\n");

    fprintf(out, "    pop rax\n");

    switch (node->binary_op.op.type) {
    case TOKEN_PLUS:
      fprintf(out, "    add rax, rbx\n");
      break;

    case TOKEN_MINUS:
      fprintf(out, "    sub rax, rbx\n");
      break;

    case TOKEN_STAR:
      fprintf(out, "    imul rax, rbx\n");
      break;

    case TOKEN_SLASH:
      fprintf(out, "    cqo\n");
      fprintf(out, "    idiv rbx\n");
      break;

    case TOKEN_EQEQ:
      fprintf(out, "    cmp rax, rbx\n");
      fprintf(out, "    sete al\n");
      fprintf(out, "    movzx rax, al\n");
      break;

    case TOKEN_NEQ:
      fprintf(out, "    cmp rax, rbx\n");
      fprintf(out, "    setne al\n");
      fprintf(out, "    movzx rax, al\n");
      break;

    case TOKEN_LT:
      fprintf(out, "    cmp rax, rbx\n");
      fprintf(out, "    setl al\n");
      fprintf(out, "    movzx rax, al\n");
      break;

    case TOKEN_GT:
      fprintf(out, "    cmp rax, rbx\n");
      fprintf(out, "    setg al\n");
      fprintf(out, "    movzx rax, al\n");
      break;

    case TOKEN_LTE:
      fprintf(out, "    cmp rax, rbx\n");
      fprintf(out, "    setle al\n");
      fprintf(out, "    movzx rax, al\n");
      break;

    case TOKEN_GTE:
      fprintf(out, "    cmp rax, rbx\n");
      fprintf(out, "    setge al\n");
      fprintf(out, "    movzx rax, al\n");
      break;

    default:
      break;
    }

    break;
  }
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
