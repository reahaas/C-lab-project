/*
 * errorHandler.h
 */

#ifndef ERROR_H
#define ERROR_H

/* Includes */
#include <stdio.h>
#include <stdlib.h>
#include "constants.h"

/* Statuses */
#define	EXIT_FAILURE	1	/* Failing exit status.  */
#define	EXIT_SUCCESS	0	/* Successful exit status.  */

/* Constants */
#define MAXIMUM_LENGTH 200

/* Error messages */
#define ERR_STR "File:%s Line:%d: %s\n"               /* TODO change the name and content */
#define FILES_SHORT "Lack of arguments. Specify at least one more file to compile.\n"
#define ASSEMBLER_ERROR "Assembler error! Failed finish for file: %s%s.\n"
#define ASSEMBLER_DONE "Assembler done for file: %s%s.\n"
#define LACK_OF_ARGUMENTS "Lack of arguments. "
#define MULTIPLE_LABEL_DEFINITIONS "Multiple definitions of the label: %s. Only can be exist. "
#define LABEL_NAME_MISSING "Label name missing."
#define OUT_OF_MEMORY "Limited memory. "
#define OUT_OF_STORAGE "Limited storage space. "
#define LINE_EXCEEDS_LENGTH "Line exceeds the maximum length. "
#define ILLEGAL_LABEL "Illegal label: %s. "
#define SYNTAX_ERROR "Syntax error. "
#define MISSING_OPERATOR "Operator or instruction missing "
#define UNKNOWN_OPERATOR "Unknown operator's or instruction's name. "
#define UNKNOWN_LABEL "Unknown label: %s. "
#define INVALID_ARGUMENT "Invalid argument: %s. "
#define UNKNOWN_ARGUMENT_TYPE "Unknown argument type. "
#define EMPTY_ARGUMENT "Empty argument. "
#define INELIGIBLE_FORMAT "Argument format not suitable. "
#define INELIGIBLE_ARGUMENT_COUNT "Count of argument is ineligible. "
#define UNKNOWN_ERROR "Unknown error. "
#define INELIGIBLE_ARGUMENTS_TYPE "Argument type ineligible. "
#define ENT_TO_UNDEF "Call to .entry with undefined label: %s. "

/* Global variables */
extern char error_message[];

extern char *cur_file_name;
FLAG error(int status);
void report(int lineIndex);

#endif


