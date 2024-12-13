CC = gcc
FLAG = -Wall -Werror -Wextra

all: s21_grep

s21_grep: s21_grep.o
	$(CC) $(FLAG) s21_grep.o -o s21_grep

s21_grep.o: s21_grep.c
	$(CC) $(FLAG) -c s21_grep.c

clean:
	rm -f *.o
	rm -f s21_grep

rebuild: clean all
