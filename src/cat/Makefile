CC = gcc -Wall -Werror -Wextra

all: s21_cat

s21_cat: s21_cat.o s21_cat_lib.o
	$(CC) s21_cat.o s21_cat_lib.o -o s21_cat

s21_cat.o: s21_cat.c
	$(CC) -c s21_cat.c

s21_cat_lib.o: s21_cat_lib.c
	$(CC) -c s21_cat_lib.c

clean:
	rm -rf *.o s21_cat

rebuild:
	make clean
	make
