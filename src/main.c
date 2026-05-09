#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/lexer.h"
#include "../include/parser.h"

int main(int argc, char *argv[]) {
  if (argc != 2) {
    fprintf(stderr, "Usage: %s <file>\n", argv[0]);
    return 1;
  }

  size_t len = strlen(argv[1]);

  if (len < 3 || strcmp(argv[1] + len - 2, ".c") != 0) {
    fprintf(stderr, "Error: Input file '%s' must have a '.c' extension\n",
            argv[1]);
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
  ast_print(parser_parse(&parser), 0);

  free(source);
  return 0;
}
