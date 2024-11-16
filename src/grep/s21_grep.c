#include "s21_grep.h"

int main(int argc, char *argv[]) {
  options opt = {0};
  int err = opt_parser(argc, argv, &opt);
  if (err) {
    printf("\tError flags!\n");
  } else {
    int eCount = 1;
    int fCount = 0;
    if (opt.e != 0) eCount = opt.e;
    char pattern[8192] = "";  // строка для хранения шаблонов поиска
    int exc[eCount + 1];  // массив для хранения номеров шаблонов для их
                          // исключения из имён файлов
    char fileNames[8192] = "";  // строка для хранения имен файлов
    pattern_parser(argc, argv, opt, pattern, exc);
    fNames_parser(argc, argv, exc, fileNames, &fCount);
    char *path = strtok(fileNames, " ");
    while (path != NULL) {
      grep(path, pattern, opt, fCount);
      path = strtok(NULL, " ");
    }
  }
  return 0;
}
