#ifndef S21_GREP_H
#define S21_GREP_H

#include <fcntl.h>
#include <pcre.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#define _GNU_SOURCE

typedef struct {
  int e;  // шаблон
  int i;  // игнорирует наличие регистра
  int v;  // выводит строки, в которых образец не встречается
  int c;  // выводит количество строк, в которых обнаружен образец
  int l;  // выводит имена файлов, содержащих образец
  int n;  // выводит строки содержащие образец с номерами
  int h;  // выводит строки содержащие образец без имени файла
  int s;  // не выводит сообщения об ошибках
} options;

int opt_parser(int argc, char *argv[], options *opt);
void pattern_parser(int argc, char *argv[], options opt, char pattern[],
                    int *exc);
void fNames_parser(int argc, char *argv[], int *exc, char fNames[],
                   int *fCount);
void grep(char *filename, char *pattern, options opt, int header);
void printHeader(char *header, int headerCount);
void prinNumber(int number, int option);

#endif  // S21_GREP_H
