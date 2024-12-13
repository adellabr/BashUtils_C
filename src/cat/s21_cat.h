#ifndef S21_CAT_H
#define S21_CAT_H
#include <getopt.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

typedef struct {
  bool flag_number_string;
  bool flag_number_nonblank;
  bool flag_end_string;
  bool flag_squeeze_blank;
  bool flag_tab_file;
  bool flag_nonprint;
  bool error;
} Flags;

void print_file(char *file_name, Flags flags);
Flags read_flag(int argc, char *argv[], int *file_index);

#endif