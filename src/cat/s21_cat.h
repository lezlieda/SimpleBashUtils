#ifndef S21_CAT_H
#define S21_CAT_H

#include <ctype.h>
#include <fcntl.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

typedef struct {
  int b;  // нумерует непустые строки
  int e;  // запускает флаг v, а также добавляет $ в конце строк; -E то же, но
          // без -v
  int n;  // нумерует все строки
  int s;  // сжимает пустые строки
  int t;  // запускает флаг v, а также отображает табы как ^I; -T то же, но без
          // -v
  int v;  // отображает все непечатные символы, кроме табов
} options;

int parser(int argc, char *argv[], options *opt);
void fileReader(char *argv[], options opt);
void cat(FILE *f, options opt);

#endif  // S21_CAT_H
