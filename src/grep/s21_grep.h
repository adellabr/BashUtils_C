#ifndef S21_GREP_H
#define S21_GREP_H
#define _GNU_SOURCE
#define SIZE 10000

#include <getopt.h>
#include <regex.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  int flag_regex;
  bool flag_e;
  bool flag_i;
  bool flag_invert;
  bool flag_count_line;
  bool flag_match_file;
  bool flag_number_match;
  bool flag_match_noname;
  bool flag_supress_error;
  bool flag_file_pattern;
  bool flag_print_match;
  bool error;
  char patterns[SIZE];
  int file_patt_count;
} Flags;

void read_flag(int argc, char *argv[], Flags *flags, int *error);
void Grep_pattern(int argc, char *argv[], regex_t *preg, Flags flags);
void Find_file(int argc, char *argv[], Flags flags, regex_t *preg);
void Read_line(FILE *file, Flags flags, regex_t *preg, char *filename,
               int count_file);
void print_count_match(int count_match, char *filename, int count_file,
                       Flags flags);
void f_flag_file(Flags *flags, int *error);
void print_match(regex_t *preg, regmatch_t regmatch, char *string);
#endif