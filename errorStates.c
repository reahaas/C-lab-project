/*
 * errorHandler.c
 */

#include "errorStates.h"
#include "constants.h"

extern int flag;

char error_message[MAXIMUM_LENGTH];

/* Changes the status to error and returns the status */
FLAG error(int status) {
	flag = ASSEMBLER_FAILURE;
	return ASSEMBLER_FAILURE;
}

/* Prints an error message and exits the program */
void report(int lineIndex) {
	fprintf(stderr, ERR_STR, cur_file_name, lineIndex, error_message);
}
