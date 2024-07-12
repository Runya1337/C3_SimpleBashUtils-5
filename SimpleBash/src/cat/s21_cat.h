#ifndef S21_CAT_H_
#define S21_CAT_H_

#include <getopt.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  bool n_flag;
  bool b_flag;
  bool t_flag;
  bool s_flag;
  bool v_flag;
  bool e_flag;
} Flags;

static struct option long_options[] = {{"number-nonblank", no_argument, 0, 'b'},
                                       {"number", no_argument, 0, 'n'},
                                       {"squeeze-blank", no_argument, 0, 's'},
                                       {0, 0, 0, 0}};

#endif