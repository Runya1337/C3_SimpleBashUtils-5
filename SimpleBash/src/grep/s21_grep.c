#include "s21_grep.h"

int main(int argc, char *argv[]) {
  char patterns[MAX_PATTERNS][MAX_LINE_LENGTH];
  char *filenames[MAX_PATTERNS];
  int pattern_count = 0;
  Flags flags = {false, false, false, false, false, false, false};

  int name_count =
      main_check_flag(argc, argv, patterns, &pattern_count, filenames, &flags);

  reader(filenames, name_count, patterns, pattern_count, flags);
  return 0;
}

int main_check_flag(int argc, char *argv[], char patterns[][MAX_LINE_LENGTH],
                    int *pattern_count, char *filenames[], Flags *flags) {
  if (argc < 3) {
    printf(
        "Usage: %s [-i] [-v] [-c] [-e] [-l] [-n] [-h] [-s] <pattern> <file>\n",
        argv[0]);
    exit(-1);
  }

  check_flags(argc, argv, patterns, pattern_count, flags);

  if (*pattern_count == 0 && optind < argc) {
    strcpy(patterns[*pattern_count], argv[optind]);
    (*pattern_count)++;
    optind++;
  }

  int name_count = argc - optind;

  for (int i = 0; optind < argc; i++) {
    filenames[i] = argv[optind++];
  }

  return name_count;
}

void check_flags(int argc, char *argv[], char patterns[][MAX_LINE_LENGTH],
                 int *pattern_count, Flags *flags) {
  int option_index = 0;
  int c;

  while ((c = getopt_long(argc, argv, "ive:clnhs", long_options,
                          &option_index)) != -1) {
    switch (c) {
      case 'i':
        flags->i_flag = true;
        break;
      case 'v':
        flags->v_flag = true;
        break;
      case 'c':
        flags->c_flag = true;
        break;
      case 'e':
        strcpy(patterns[(*pattern_count)++], optarg);
        break;
      case 'l':
        flags->l_flag = true;
        break;
      case 'n':
        flags->n_flag = true;
        break;
      case 'h':
        flags->h_flag = true;
        break;
      case 's':
        flags->s_flag = true;
        break;
      default:
        printf(
            "Usage: %s [-i] [-v] [-c] [-l] [-n] [-h] [-s] [-e pattern] "
            "<file>\n",
            argv[0]);
        exit(-1);
    }
  }
}

void reader(char *filenames[], int name_count, char patterns[][MAX_LINE_LENGTH],
            int pattern_count, Flags flags) {
  for (int j = 0; j < name_count; j++) {
    char line[MAX_LINE_LENGTH];
    int count = 0;
    int match = 0;
    int line_number = 0;
    FILE *file = NULL;
    if (filenames[j] != NULL) {
      file = fopen(filenames[j], "r");
      if (file == NULL) {
        if (flags.s_flag == false) {
          printf("grep: %s: No such file or directory\n", filenames[j]);
        }
        continue;
      }
    }

    while (fgets(line, MAX_LINE_LENGTH, file) != NULL) {
      line_number++;
      if (line[strlen(line) - 1] != '\n') {
        strcat(line, "\n");
      }
      for (int i = 0; i < pattern_count; i++) {
        int found = find_pattern(line, patterns[i], flags.i_flag);
        int is_match = is_search_match(flags.v_flag, found);

        if (is_match == 1) {
          match = 1;
        }

        if (is_match == 1 && flags.c_flag) {
          count++;
          continue;
        }

        if (flags.l_flag) continue;

        if (is_match == 1) {
          if (flags.h_flag) {
            if (flags.n_flag)
              printf("%d:%s", line_number, line);
            else
              printf("%s", line);
          } else if (name_count < 2) {
            if (flags.n_flag)
              printf("%d:%s", line_number, line);
            else
              printf("%s", line);
          } else {
            if (flags.n_flag)
              printf("%s:%d:%s", filenames[j], line_number, line);
            else
              printf("%s:%s", filenames[j], line);
          }
          break;
        }
      }
    }

    if (flags.c_flag) {
      if (name_count > 1) {
        printf("%s:%d\n", filenames[j], count);
      } else {
        printf("%d\n", count);
      }
    }

    if (flags.l_flag && match == 1) {
      printf("%s\n", filenames[j]);
    }

    if (file != stdin) {
      fclose(file);
    }
  }
}

int find_pattern(char *line, char *pattern, bool i_flag) {
  regex_t regex;

  int regcomp_result;
  int res_nomatch;

  if (i_flag == true) {
    regcomp_result = regcomp(&regex, pattern, REG_ICASE);
  } else {
    regcomp_result = regcomp(&regex, pattern, REG_EXTENDED);
  }

  if (regcomp_result == 1) {
    return 0;
  }

  int regexec_result = regexec(&regex, line, 0, NULL, 0);
  regfree(&regex);

  if (regexec_result != 1) {
    res_nomatch = 1;
  } else if (regexec_result == REG_NOMATCH) {
    res_nomatch = 0;
  } else {
    res_nomatch = 0;
  }
  return res_nomatch;
}

int is_search_match(bool v_flag, int found) {
  if (v_flag) {
    found = !found;
  }
  return found;
}