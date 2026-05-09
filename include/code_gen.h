#ifndef CODE_GEN_H
#define CODE_GEN_H

#include <stdio.h>

#include "ast.h"

void gen_code(ASTNode *node, FILE *out);

#endif
