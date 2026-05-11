#ifndef CODEGEN_H
#define CODEGEN_H

#include <stdio.h>

#include "ast.h"

typedef struct {
  char *name;
  int offset;
} LocalVar;

void gen_code(ASTNode *node, FILE *out);

#endif
