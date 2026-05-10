#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/codegen.h"
#include "../include/lexer.h"
#include "../include/parser.h"

int main(int argc, char *argv[]) {
  if (argc != 2) {
    fprintf(stderr, "Usage: %s <file>\n", argv[0]);
    return 1;
  }

  FILE *file = fopen(argv[1], "r");
  if (!file) {
    perror("fopen");
    return 1;
  }

  fseek(file, 0, SEEK_END);
  long size = ftell(file);
  rewind(file);

  char *source = malloc(size + 1);
  fread(source, 1, size, file);
  source[size] = '\0';

  fclose(file);

  Lexer lexer;
  lexer_init(&lexer, source);

  Token token, tokens[1024];
  size_t token_count = 0;

  do {
    token = lexer_next_token(&lexer);
    tokens[token_count++] = token;
  } while (token.type != TOKEN_EOF);

  Parser parser;
  parser_init(&parser, tokens, token_count);

  ASTNode *ast = parser_parse(&parser);

  print_ast(ast, 0);

  FILE *out = fopen("out.s", "w");

  if (out == NULL) {
    perror("fopen");
    free(source);
    return 1;
  }

  fprintf(out, ".intel_syntax noprefix\n");

  gen_code(ast, out);

  free(source);
  return 0;
}
