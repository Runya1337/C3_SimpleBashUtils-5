#include "s21_cat.h"

int check_flags(int argc, char *argv[], Flags *flags);
void catch_files(int argc, char *argv[], Flags flags);
void reader(Flags flags, FILE *file);
int is_printable(int c);
void v_flag(Flags flags, FILE *file, int *c);

int main(int argc, char *argv[]) {
  Flags flags = {false, false, false, false, false, false};
  if (check_flags(argc, argv, &flags) == 1) {
    catch_files(argc, argv, flags);
  } else {
    printf("%s\n", "usage: cat [-belnstuv] [file ...]");
  }
}

int check_flags(int argc, char *argv[], Flags *flags) {
  char current_opt = 0;
  int option_index;
  int flag;
  while (current_opt != -1) {
    current_opt =
        getopt_long(argc, argv, "+benstvTE", long_options, &option_index);
    switch (current_opt) {
      case 'b':
        flags->b_flag = true;
        break;
      case 'n':
        flags->n_flag = true;
        break;
      case 't':
        flags->t_flag = true;
        flags->v_flag = true;
        break;
      case 's':
        flags->s_flag = true;
        break;
      case 'v':
        flags->v_flag = true;
        break;
      case 'e':
        flags->e_flag = true;
        flags->v_flag = true;
        break;
    }
  }
  if (current_opt == -1) {
    flag = 1;
  } else {
    flag = 0;
  }
  return flag;
}

void catch_files(int argc, char *argv[], Flags flags) {
  for (; optind < argc; optind++) {
    FILE *file = fopen(argv[optind], "r");
    if (file == NULL) {
      fprintf(stderr, "cat: %s: No such file or directory", argv[optind]);
    } else {
      reader(flags, file);
    }
  }
}

void reader(Flags flags, FILE *file) {
  int line_number = 1;
  int c;
  bool new_line = true;
  int pre_preview_symbvol = '\0';
  int preview_symbvol = '\0';
  while ((c = fgetc(file)) != EOF) {
    if (flags.s_flag) {
      if ((pre_preview_symbvol == '\n' || pre_preview_symbvol == '\0') &&
          preview_symbvol == '\n' && c == '\n') {
        continue;
      }
    }
    if (flags.b_flag) {
      if (new_line == true && c != '\n') {
        printf("%6d\t", line_number);
        line_number++;
        new_line = false;
      }
      if (c == '\n') {
        new_line = true;
      }
    } else if (flags.n_flag) {
      if (new_line == true) {
        printf("%6d\t", line_number);
        line_number++;
        new_line = false;
      }
      if (c == '\n') {
        new_line = true;
      }
    }
    if (flags.e_flag && c == '\n') {
      putchar('$');
    }
    if (flags.t_flag && c == '\t') {
      printf("^I");
      continue;
    }
    if (flags.v_flag) {
      v_flag(flags, file, &c);
    }
    pre_preview_symbvol = preview_symbvol;
    preview_symbvol = c;
    putchar(c);
  }
}

void v_flag(Flags flags, FILE *file, int *c) {
  if (is_printable(*c) == 0 && *c != '\n' && *c != '\t') {
    if (*c < 32) {
      printf("^");
      *c = *c + 64;
    } else if (*c == 127) {
      printf("^");
      *c = '?';
    } else if (*c > 128 && *c < 160) {
      printf("M-^");
      *c = *c - 64;
    }
  }
}

int is_printable(int c) {
  int flag = 1;
  if (c <= 32 || c >= 127) {
    flag = 0;
  }
  return flag;
}