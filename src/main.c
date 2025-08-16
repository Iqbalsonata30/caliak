#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 255
#define handle_error(msg)                                                      \
  do {                                                                         \
    perror(msg);                                                               \
    exit(1);                                                                   \
  } while (0)

int main(int argc, char **argv) {
  const char *prog = argv[0];
  int c;
  bool number = 0;
  unsigned lineno = 0;
  char buffer[MAX_LINE_LENGTH];

  while (argc-- > 0 && (*++argv)[0] == '-') {
    switch (c = *++argv[0]) {
    case 'n':
      number = true;
      break;
    default:
      printf("%s: illegal option %c\n", prog, c);
      break;
    }
  }

  for (; argc > 0; argc--) {
    FILE *fp = fopen(*argv++, "r");
    if (fp == NULL)
      handle_error("fopen");

    if (number) {
      while (fgets(buffer, MAX_LINE_LENGTH, fp) != NULL) {
        lineno++;
        printf("%3.d: %s", lineno, buffer);
      }
    } else {
      while ((c = fgetc(fp)) != EOF)
        fputc(c, stdout);
    }
    fclose(fp);
  }

  return 0;
}
