#include "s21_cat.h"

int main(int argc, char *argv[]) {
  int file_index = 0;
  Flags flags = read_flag(argc, argv, &file_index);
  if (!flags.error && optind < argc) {
    for (int i = file_index; i < argc; i++) print_file(argv[i], flags);
  }

  return 0;
}

Flags read_flag(int argc, char *argv[], int *file_index) {
  struct option long_options[] = {{"number-nonblank", no_argument, 0, 'b'},
                                  {"number", no_argument, 0, 'n'},
                                  {"squeeze-blank", no_argument, 0, 's'},
                                  {NULL, 0, NULL, 0}};

  int flag_option;
  Flags flags = {false, false, false, false, false, false, false};

  while ((flag_option =
              getopt_long(argc, argv, "benstvTE", long_options, NULL)) != -1) {
    switch (flag_option) {
      case 'b': {
        flags.flag_number_nonblank = true;
        break;
      }
      case 'e': {
        flags.flag_end_string = true;
        flags.flag_nonprint = true;
        break;
      }
      case 'n': {
        flags.flag_number_string = true;
        break;
      }
      case 's': {
        flags.flag_squeeze_blank = true;
        break;
      }
      case 'v': {
        flags.flag_nonprint = true;
        break;
      }
      case 't': {
        flags.flag_tab_file = true;
        flags.flag_nonprint = true;
        break;
      }
      case 'T': {
        flags.flag_tab_file = true;
        break;
      }
      case 'E': {
        flags.flag_end_string = true;
        break;
      }
      default: {
        flags.error = true;
        break;
      }
    }
  }
  *file_index = optind;
  return flags;
}

void print_file(char *file_name, Flags flags) {
  FILE *file = fopen(file_name, "r");
  int prev = '\n';
  int prev2 = '0';
  int n = 1;
  int b = 1;
  if (file != NULL) {
    int c = fgetc(file);
    while (c != EOF) {
      if (flags.flag_squeeze_blank && c == '\n' && prev == '\n' &&
          prev2 == '\n') {
        prev2 = prev;
        prev = c;
        c = fgetc(file);
        continue;
      }
      if (flags.flag_number_nonblank && c != '\n' && prev == '\n') {
        printf("%6d\t", b);
        b++;
      }
      if (flags.flag_number_string && !flags.flag_number_nonblank &&
          prev == '\n') {
        printf("%6d\t", n);
        n++;
      }
      if (flags.flag_end_string && c == '\n') printf("$");
      if (flags.flag_tab_file && c == '\t') {
        printf("^");
        c += 64;
      }
      if (flags.flag_nonprint) {
        if (c > 127 && c < 160) printf("M-^");
        if ((c < 32 && c != '\n' && c != '\t') || c == 127) printf("^");
        if ((c < 32 || (c > 126 && c < 160)) && c != '\n' && c != '\t')
          c = c > 126 ? c - 128 + 64 : c + 64;
      }

      printf("%c", c);
      prev2 = prev;
      prev = c;
      c = fgetc(file);
    }
    fclose(file);
    // printf("\n");
  }
}