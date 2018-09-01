#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "constants.h"
#include "structures.h"
#include "errorStates.h"

extern FLAG flag;
extern int ic;
extern int dc;


FLAG assembler(FILE *source_file);
input_line *get_line(FILE *);
void free_line(input_line *line);
bool valid_label(const char *);
bool valid_register(const char *string_register);
bool copy_string(char **dest, const char *src);
bool string_to_int(const char *str, int *dest);
static bool check_and_fix_second_addr(char *src, input_line *line);
bool valid_label_for_second_addressing(char *label_string);
bool valid_number(char *str);

FLAG firstCycle(FILE *source_file);
void relocate(int spacing);

FLAG secondCycle(FILE *);

#endif
