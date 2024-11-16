CC = gcc -Wall -Werror -Wextra -g
all: s21_grep

s21_grep: s21_grep.o s21_grep_lib.o
	$(CC) s21_grep.o s21_grep_lib.o  -lpcre -o s21_grep

s21_grep.o: s21_grep.c
	$(CC) -c s21_grep.c -lpcre

s21_grep_lib.o: s21_grep_lib.c
	$(CC) -c s21_grep_lib.c -lpcre

clean:
	rm -rf *.o s21_grep

rebuild: clean all
