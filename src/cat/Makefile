CC = gcc
FLAG = -Wall -Werror -Wextra

all: s21_cat

s21_cat: s21_cat.o
	$(CC) $(FLAG) s21_cat.o -o s21_cat

s21_cat.o: s21_cat.c
	$(CC) $(FLAG) -c s21_cat.c

clean:
	rm -f *.o
	rm -f s21_cat

rebuild: clean all