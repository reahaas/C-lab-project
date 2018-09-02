#include "errorStates.h"
#include "constants.h"

extern int flag;

char error_message[MAXIMUM_LENGTH];

FLAG error(int status) {
	flag = ASSEMBLER_FAILURE;
	return ASSEMBLER_FAILURE;
}

/**
 * @param line_index
 * Function print error with specification and end the program
 * */
void report(int line_index) {
	fprintf(stderr, ERROR_STRING, cur_file_name, line_index, error_message);
}
