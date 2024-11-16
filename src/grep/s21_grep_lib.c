#include "s21_grep.h"

int opt_parser(int argc, char *argv[], options *opt) {
  int err = 0;
  for (int i = 1; i < argc; i++) {  // i - для пересчёта аргументов
    char *scan = argv[i];
    if (scan[0] == '-') {
      size_t j = 1;
      while (j < strlen(scan)) {  // j - для пересчёта символов аргумента
        char c = scan[j];
        switch (c) {
          case 'e':
            if ((argv[i - 2] != NULL) && (!strcmp(argv[i - 1], "-e")) &&
                (!strcmp(argv[i - 2], "-e"))) {
              break;  // чтобы не увеличивать счётчик е, если шаблон -e это '-e'
            } else {
              ++opt->e;
              j = strlen(scan);
              break;
            }
          case 'i':
            opt->i = 1;
            break;
          case 'v':
            opt->v = 1;
            break;
          case 'c':
            opt->c = 1;
            break;
          case 'l':
            opt->l = 1;
            break;
          case 'n':
            opt->n = 1;
            break;
          case 'h':
            opt->h = 1;
            break;
          case 's':
            opt->s = 1;
            break;
          default:
            if ((strchr(argv[i - 1], 'e') != NULL) &&
                (strlen(strchr(argv[i - 1], 'e')) == 1)) {
              break;  // чтобы не было ошибки, если шаблон -е начинается с '-'
            } else {
              ++err;
              break;
            }
        }
        ++j;
      }
    }
  }
  return err;
}

void pattern_parser(int argc, char *argv[], options opt, char pattern[],
                    int *exc) {
  if (opt.e == 0) {  // если не задано ни одного шаблона
    for (int i = 1; i < argc; i++) {
      if (strcmp(argv[i], "-") == 0) {
        strcat(pattern, argv[i]);
        exc[0] = i;
        break;
      } else if (argv[i][0] == '-' && strlen(argv[i]) > 1) {
        continue;
      } else {
        strcat(pattern, argv[i]);
        exc[0] = i;
        break;
      }
    }
  } else {  // если задан хотя бы один шаблон -e
    int ecount = 0;
    for (int i = 1; i < argc; i++) {
      if (opt.e - ecount == 0) {
        break;
      }
      if (argv[i][0] == '-') {
        if (strchr(argv[i], 'e') != NULL) {
          if (strlen(strchr(argv[i], 'e')) != 1) {
            if (ecount == 0) {
              strcpy(pattern, strchr(argv[i], 'e') + 1);
              exc[ecount] = i;
              ++ecount;
            } else {
              strcat(pattern, "|");
              strcat(pattern, strchr(argv[i], 'e') + 1);
              exc[ecount] = i;
              ++ecount;
            }
          } else {
            ++i;
            if (ecount == 0) {
              if (argv[i] == NULL) {
                printf("grep: error flag -e\n");
                exit(1);
              } else {
                strcpy(pattern, argv[i]);
                exc[ecount] = i;
                ++ecount;
              }
            } else {
              strcat(pattern, "|");
              strcat(pattern, argv[i]);
              exc[ecount] = i;
              ++ecount;
            }
          }
        }
      }
    }
  }
}

void fNames_parser(int argc, char *argv[], int *exc, char fNames[],
                   int *fCount) {
  int i = 1;
  for (; i < argc; i++) {  // i - для пересчёта аргументов
    unsigned int j = 0;
    int ii = exc[j];
    while (j < sizeof(exc)) {  // перебираем массив исключений
      if (i == ii) {  // если i совпадает с исключением - пропускаем
        ++i;
        j = 0;
      } else {
        ++j;
      }
    }
    if (argv[i][0] == '-' && strlen(argv[i]) > 1) {
      continue;
    } else if (argv[i - 1] != NULL && strcmp(argv[i - 1], "-e") == 0) {
      continue;
    } else if (strcmp(argv[i], "-") == 0) {
      strcat(fNames, " ");
      strcat(fNames, "-");
      ++(*fCount);
    } else {
      strcat(fNames, " ");
      strcat(fNames, argv[i]);
      ++(*fCount);
    }
  }
  if (*fCount == 0) {
    strcat(fNames, " ");
    strcat(fNames, "-");
    ++(*fCount);
  }
}

void grep(char *filename, char *pattern, options opt, int header) {
  int fd;
  if (strcmp(filename, "-") == 0) {
    fd = STDIN_FILENO;
  } else {
    fd = open(filename, O_RDONLY);
  }
  FILE *file = fdopen(fd, "r");
  if (strcmp(filename, "-") == 0) {
    filename = "(standard input)";
  }
  if (file == NULL) {
    if (!opt.s) printf("s21_grep: %s: No such file or directory\n", filename);
  } else {
    const char *error;
    int erroffset;
    int pcreOpt = 0;
    if (opt.i) {
      pcreOpt = PCRE_CASELESS;
    }
    pcre *re = pcre_compile(pattern, pcreOpt, &error, &erroffset, NULL);

    if (re == NULL) {
      printf("PCRE compilation failed at offset %d: %s\n", erroffset, error);
      exit(1);
    } else {
      char *line = NULL;
      size_t len = 0;
      int lineCount = 0;
      int matchCount = 0;
      while ((getline(&line, &len, file)) != -1) {
        lineCount++;
        int ovector[30];
        int rc = pcre_exec(re, NULL, line, strlen(line), 0, 0, ovector, 30);
        if (opt.h) header = 0;
        if (rc < 0) {
          if (opt.v) {
            if (!opt.c && !opt.l) {
              printHeader(filename, header);
              prinNumber(lineCount, opt.n);
              printf("%s", line);
            }
            matchCount++;
          }
        } else {
          if (!opt.v) {
            if (!opt.c && !opt.l) {
              printHeader(filename, header);
              prinNumber(lineCount, opt.n);
              printf("%s", line);
            }
            matchCount++;
          } else {
            if (!opt.c && !opt.l && !opt.v) {
              printHeader(filename, header);
              printf("%s", line);
            }
          }
        }
      }
      if (opt.c) {
        printHeader(filename, header);
        if (opt.l && matchCount > 0) {
          matchCount = 1;
        }
        printf("%d\n", matchCount);
      }
      if (opt.l) {
        if (matchCount > 0) {
          printf("%s\n", filename);
        }
      }
      if (line != NULL) {
        free(line);
        line = NULL;
      }
    }
    pcre_free(re);
  }
  if (file != NULL) {
    fclose(file);
  }
}

void printHeader(char *header, int headerCount) {
  if (headerCount > 1) {
    printf("%s:", header);
  }
}

void prinNumber(int number, int option) {
  if (option) {
    printf("%d:", number);
  }
}
