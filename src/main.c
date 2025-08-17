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
  bool number, nonempty, end_line;
  unsigned lineno = 0;
  char buffer[MAX_LINE_LENGTH];

  number = nonempty = end_line = false;

  while (argc-- > 0 && (*++argv)[0] == '-') {
    switch (c = *++argv[0]) {
    case 'n':
      number = true;
      break;
    case 'b':
      nonempty = true;
      break;
    case 'e':
      end_line = true;
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

    if (number || nonempty) {
      while (fgets(buffer, MAX_LINE_LENGTH, fp) != NULL) {
        bool is_nonempty = strlen(buffer) > 1;
        if (!nonempty || is_nonempty) {
          if (end_line) {
            int i;
            for (i = 0; buffer[i] != '\n'; ++i)
              ;
            buffer[i++] = '$';
            buffer[i++] = '\n';
            buffer[i] = '\0';
          }
          lineno++;
          printf("%3.d: %s", lineno, buffer);
        }
      }
    } else if (end_line) {
      int i = 0;
      while ((c = getc(fp)) != EOF) {
        if (c == '\n') {
          buffer[i++] = '$';
          buffer[i++] = c;
        } else {
          buffer[i++] = c;
        }
      }
      buffer[i] = '\0';
      printf("%s", buffer);
    } else {
      while ((c = fgetc(fp)) != EOF)
        putchar(c);
    }
    fclose(fp);
  }
  return 0;
}
