#include "s21_cat.h"

int main(int argc, char *argv[]) {
  options opt = {0};  // инициализация и зануление структуры опций
  if (parser(argc, argv, &opt)) {
    printf("error flags\n");
  } else {
    argv += optind;
    fileReader(argv, opt);
  }
  return 0;
}
