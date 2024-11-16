#ifndef S21_CAT_LIB_C
#define S21_CAT_LIB_C

#include "s21_cat.h"

int parser(int argc, char *argv[], options *opt) {
  int err = 0;
  int rez = 0;
  int option_index;
  const struct option long_options[] = {
      // структура для соответствия длинных параметров коротким
      {"number-nonblank", 0, 0, 'b'},
      {"number", 0, 0, 'n'},
      {"squeeze-blank", 0, 0, 's'},
      {NULL, 0, NULL, 0}};
  while ((rez = getopt_long(argc, argv, "+benstvET", long_options,
                            &option_index)) != -1) {
    switch (rez) {  // если список коротких опций "benstvET", опции будут
                    // считываться из любого места как в linux
      case 'b':
        opt->b = 1;
        break;
      case 'e':
        opt->v = 1;
        opt->e = 1;
        break;
      case 'n':
        opt->n = 1;
        break;
      case 's':
        opt->s = 1;
        break;
      case 't':
        opt->v = 1;
        opt->t = 1;
        break;
      case 'v':
        opt->v = 1;
        break;
      case 'T':
        opt->t = 1;
        break;
      case 'E':
        opt->e = 1;
        break;
      default:
        err = 1;  // если встретятся 'кривые' флаги
        break;
    }
  }
  return err;
}

void fileReader(char *argv[], options opt) {
  int i = 0;
  char *path;
  FILE *f;
  while ((path = argv[i]) != NULL || i == 0) {
    int fd;
    if (path == NULL || strcmp(path, "-") == 0) {
      fd = STDIN_FILENO;
    } else {
      fd = open(path, O_RDONLY);
    }
    if (fd < 0) {
      printf("s21_cat: %s: No such file or directory\n", argv[i]);
    } else {
      if (fd == STDIN_FILENO) {
        cat(stdin, opt);
      } else {
        f = fdopen(fd, "r");
        cat(f, opt);
        fclose(f);
      }
    }
    ++i;
  }
}

void cat(FILE *f, options opt) {
  int ch, gobble, line, prev;
  line = gobble = 0;
  for (prev = '\n'; (ch = getc(f)) != EOF; prev = ch) {
    if (prev == '\n') {
      if (opt.s) {  // если два перевода строки подряд, то пропускаем второй
        if (ch == '\n') {
          if (gobble) {
            continue;
          }
          gobble = 1;
        } else {
          gobble = 0;
        }
      }
      if (opt.n && (!opt.b || ch != '\n')) {  // если есть флаг -n, то выводим
                                              // номер строки
        fprintf(stdout, "%6d\t", ++line);
      } else if (opt.b && ch != '\n') {  // если есть флаг -b, то выводим номер
                                         // строки, кроме пустых строк
        fprintf(stdout, "%6d\t", ++line);
      }
    }
    if (ch == '\n') {
      if (opt.e && putchar('$') == EOF)
        break;  // putchar возвращает EOF при неудаче
    } else if (ch == '\t') {
      if (opt.t) {  // если есть флаг -t, то заменяем табуляцию на ^I
        if (putchar('^') == EOF || putchar('I') == EOF) break;
        continue;
      }
    } else if (opt.v) {  // если есть флаг -v, то заменяем непечатаемые символы
      if (!isascii(ch) && !isprint(ch)) {  // на 'M-' и код символа
        if (putchar('M') == EOF || putchar('-') == EOF) break;
        ch = toascii(ch);
      }
      if (iscntrl(ch)) {  // '\177' - 'DEL', отображается как '^?';
                          // ch + 64 - повышает регистр символа
        if (putchar('^') == EOF || putchar(ch == '\177' ? '?' : ch + 64) == EOF)
          break;
        continue;
      }
    }
    if (putchar(ch) == EOF) break;
  }
}

#endif  // S21_CAT_LIB_C
