#ifndef ASSEMBLER_H
#define ASSEMBLER_H

/* Includes */
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "constants.h"
#include "structs.h"
#include "errorHandler.h"

extern FLAG flag;
extern int ic;
extern int dc;

/* Main assembler */
FLAG assembler(FILE *sourceFile);
input_line *getLine(FILE *);
void freeLine(input_line *line);
bool valid_label(const char *);
bool valid_register(const char *registerStr);
bool copy_string(char **dest, const char *src);
bool strToInt(const char *str, int *dest);
static bool check_and_fix_second_addr(char *src, input_line *line);
bool valid_label_for_second_adrresing(char *labelStr);
bool valid_number(char *str);

/* First run functions */
FLAG firstCycle(FILE *sourceFile);
void relocate(int paddin);

/* Second run functions */
FLAG secondRun(FILE *);

#endif
