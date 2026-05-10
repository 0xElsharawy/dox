#ifndef CODEGEN_H
#define CODEGEN_H

#include <stdio.h>

#include "ast.h"

void gen_code(ASTNode *node, FILE *out);

#endif
