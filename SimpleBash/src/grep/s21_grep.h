#include <getopt.h>
#include <regex.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_LINE_LENGTH 1024
#define MAX_PATTERNS 1024

typedef struct {
  bool i_flag;
  bool v_flag;
  bool c_flag;
  bool l_flag;
  bool n_flag;
  bool h_flag;
  bool s_flag;
} Flags;

int main_check_flag(int argc, char *argv[], char patterns[][MAX_LINE_LENGTH],
                    int *pattern_count, char *filenames[], Flags *flags);
void check_flags(int argc, char *argv[], char patterns[][MAX_LINE_LENGTH],
                 int *pattern_count, Flags *flags);
void reader(char *filenames[], int name_count, char patterns[][MAX_LINE_LENGTH],
            int pattern_count, Flags flags);
int find_pattern(char *line, char *pattern, bool i_flag);
int is_search_match(bool v_flag, int found);

struct option long_options[] = {
    {"i", 0, 0, 'i'}, {"v", 0, 0, 'v'}, {"c", 0, 0, 'c'}, {"l", 0, 0, 'l'},
    {"n", 0, 0, 'n'}, {"h", 0, 0, 'h'}, {"s", 0, 0, 's'}, {0, 0, 0, 0}};