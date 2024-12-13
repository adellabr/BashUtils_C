#include "s21_grep.h"

void read_flag(int argc, char *argv[], Flags *flags, int *error) {
  int flag_option = 0;

  while ((flag_option = getopt_long(argc, argv, "e:ivclnhsof:", NULL, 0)) !=
         -1) {
    switch (flag_option) {
      case 'e': {
        // flags.flag_regex |= REG_EXTENDED;
        flags->flag_e = true;
        strcat(flags->patterns, optarg);
        strcat(flags->patterns, "|");
        break;
      }
      case 'i': {
        flags->flag_i = true;
        break;
      }
      case 'v': {
        flags->flag_invert = true;
        break;
      }
      case 'c': {
        flags->flag_count_line = true;
        break;
      }
      case 'l': {
        flags->flag_match_file = true;
        break;
      }
      case 'n': {
        flags->flag_number_match = true;
        break;
      }
      case 'h': {
        flags->flag_match_noname = true;
        break;
      }
      case 's': {
        flags->flag_supress_error = true;
        break;
      }
      case 'f': {
        flags->flag_file_pattern = true;
        f_flag_file(flags, error);
        break;
      }
      case 'o': {
        flags->flag_print_match = true;
        break;
      }
      default: {
        flags->error = true;
        break;
      }
    }
  }
}

int main(int argc, char *argv[]) {
  int error = 0;
  regex_t preg_storage;
  regex_t *preg = &preg_storage;
  Flags flags = {0,     false, false, false, false, false, false,
                 false, false, false, false, false, {0},   0};
  read_flag(argc, argv, &flags, &error);
  if (!error && !flags.error && argc > 1 && argc > optind) {
    Grep_pattern(argc, argv, preg, flags);
    Find_file(argc, argv, flags, preg);
    regfree(preg);
  }
}

void Grep_pattern(int argc, char *argv[], regex_t *preg, Flags flags) {
  if (flags.flag_e || flags.flag_file_pattern) {
    if (flags.patterns[strlen(flags.patterns) - 1] == '|')
      flags.patterns[strlen(flags.patterns) - 1] = '\0';
    regcomp(preg, flags.patterns,
            flags.flag_i ? REG_EXTENDED | REG_ICASE : REG_EXTENDED);
  } else if (!flags.flag_file_pattern && argc > 1 && argc > optind) {
    char *pattern = 0;
    pattern = argv[optind];
    regcomp(preg, pattern,
            flags.flag_i ? REG_EXTENDED | REG_ICASE : REG_EXTENDED);
    optind++;
  }
}

void f_flag_file(Flags *flags, int *error) {
  FILE *file = fopen(optarg, "r");
  if (file != NULL) {
    char *line = 0;
    size_t len = 0;
    while (getline(&line, &len, file) > 0) {
      strcat(flags->patterns, line);
      if (flags->patterns[strlen(flags->patterns) - 1] == '\n')
        flags->patterns[strlen(flags->patterns) - 1] = '|';
    }
    free(line);
    fclose(file);
  } else {
    fprintf(stderr, "grep: %s: No such file or directory\n", optarg);
    *error = 1;
  }
}

void Find_file(int argc, char *argv[], Flags flags, regex_t *preg) {
  char **ind_file = &argv[optind];
  int count_file = 0;
  for (; ind_file != &argv[argc]; ind_file++) {
    if (**ind_file == '-') continue;
    count_file++;
  }
  for (int i = optind; i < argc; i++) {
    // if (argv[i][0] == '-') continue;
    FILE *file = fopen(argv[i], "r");
    if (file != NULL) {
      Read_line(file, flags, preg, argv[i], count_file);
      fclose(file);
    } else if (!flags.flag_supress_error)
      fprintf(stderr, "grep: %s: No such file or directory\n", argv[i]);
  }
}

void Read_line(FILE *file, Flags flags, regex_t *preg, char *filename,
               int count_file) {
  regmatch_t regmatch;
  char *string = 0;
  size_t len = 0;
  int count_line = 0;
  int count_match = 0;
  while (getline(&string, &len, file) > 0) {
    count_line++;
    int match = !regexec(preg, string, 1, &regmatch, 0);
    if (flags.flag_invert) match = !match;
    if (match) count_match++;
    if (match && !flags.flag_count_line && !flags.flag_match_file) {
      if (count_file > 1 && !flags.flag_match_noname) printf("%s:", filename);
      if (flags.flag_number_match) printf("%d:", count_line);
      if (flags.flag_print_match && !flags.flag_invert)
        print_match(preg, regmatch, string);
      else {
        printf("%s", string);
        if (string[strlen(string) - 1] != '\n') printf("\n");
      }
    }
  }
  free(string);
  if (flags.flag_count_line)
    print_count_match(count_match, filename, count_file, flags);
  if (flags.flag_match_file && count_match > 0) printf("%s\n", filename);
}

void print_count_match(int count_match, char *filename, int count_file,
                       Flags flags) {
  if (flags.flag_match_file) {
    if (count_match >= 1)
      count_match = 1;
    else
      count_match = 0;
  }
  if (count_file > 1 && !flags.flag_match_noname) printf("%s:", filename);
  if (count_file > 1) printf("%d\n", count_match);
  if (count_file == 1) printf("%d\n", count_match);
}

void print_match(regex_t *preg, regmatch_t regmatch, char *string) {
  printf("%.*s\n", (int)(regmatch.rm_eo - regmatch.rm_so),
         string + regmatch.rm_so);
  char *temp = string + regmatch.rm_eo;
  while (!regexec(preg, temp, 1, &regmatch, 0)) {
    printf("%.*s\n", (int)(regmatch.rm_eo - regmatch.rm_so),
           temp + regmatch.rm_so);
    temp += regmatch.rm_eo;
  }
}
