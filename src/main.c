#include <stdio.h>

int main(int argc, char *argv[]) {
  if (argc != 2) {
    fprintf(stderr, "Usage: %s <file>\n", argv[0]);
    return 1;
  }

  const char *filename = argv[1];

  FILE *file = fopen(filename, "r");
  if (!file) {
    perror("fopen");
    return 1;
  }

  char line[1024];

  while (fgets(line, sizeof(line), file)) {
    printf("%s", line);
  }

  fclose(file);
  return 0;
}
