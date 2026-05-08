#include "../include/lexer.h"

#include <stdio.h>
#include <stdlib.h>

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

  Token token;

  do {
    token = lexer_next_token(&lexer);

    printf("%s", token_type_to_string(token.type));
    if (token.value)
      printf(" : %s", token.value);

    printf("\n");

  } while (token.type != TOKEN_EOF);

  free(source);
  return 0;
}
