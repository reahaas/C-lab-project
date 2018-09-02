#include "assembler.h"
#include "dataTable.h"
#include "cmdTable.h"
#include "symbolTable.h"

static bool handleLine(input_line*);


/**
 * first cycle update ic and dc and build the symbol table.
 * @param source_file, the file the cycle work on.
 * @return flag, while error occur in the file, flag return true.
 */
FLAG firstCycle(FILE *source_file) {
	input_line *line;
	int lineIndex; /* num of row in the file */
	fseek(source_file, 0L, SEEK_SET);
	for (lineIndex = 1; true; lineIndex++) { /* Runs through all the lines. */
		if ((line = get_line(source_file))) /* line is an object that contains all the data from one "string" line in the file. */
		{
			if (line->is_end_of_file) {
                return false;
				break; /* End when EOF is encountered */
			}
			if (line->unnecessary) {
				continue;
			}
			if (!handleLine(line)) {
				report(lineIndex);
				if (cmd_list.length + data_list.length >= MAX_MEMORY_SIZE) {
					error(sprintf(error_message, OUT_OF_STORAGE));
					return flag;
				}
			}
		} else {
			report(lineIndex);
		}
        free_line(line);
	}
	return flag;
}/* End firstCycle */

/* Analyses a line of code and operates accordingly,working by cases, if needed put in label list*/
static bool handleLine(input_line* line) {
	int isOp = false;
	int isExt = false;
	int hasLabel = (line->label ? true : false);
	int wordCounter = 0; /* counts the amount of words need to be in the memory */

	switch (line->cmd) {
    /* Two operands group */
    case MOV:
    case CMP:
    case ADD:
    case SUB:
    case LEA:
        if (line->args != NULL) {
            if (line->args[1] != NULL) {
                if (!(valid_register(line->args[0]) && valid_register(line->args[1]))) { /* If not register to register*/
                    wordCounter++; /* Two extra words*/
                }
            } else {
                error(sprintf(error_message, LACK_OF_ARGUMENTS));
                return false;
                /* Expected 2 args get one error */
            }
        } else {
            error(sprintf(error_message, LACK_OF_ARGUMENTS));
            return false;
            /*Expected 2 args error */
        }
        /* no break */
    /* One operands group */
    case NOT:
    case CLR:
    case INC:
    case DEC:
    case RED:
    case PRN:
        wordCounter++; /* One extra word*/
        /* no break */
    /* No operands group */
    case RTS:
    case STOP:
        isOp = true;
        /* Count lines */
        if (hasLabel) {
            if (!(addLabel(line->label, cmd_list.length, isExt, isOp))) {
                return false;
            }
        }
            increase_cmd_length(++wordCounter); /* Count number of rows in cmd table*/
        break;
    /* jump operands group, can have one operand or three. here i assuming that the syntax is good, handel at get line. */
    case JMP:
    case JSR:
    case BNE:
        isOp = true;
        /*dont forget the may can be that has */
        if (line->args != NULL ) { /* jump first argument is a label */
            wordCounter++;
            if(line->args[1] != NULL && line->args[2] != NULL ){ /*if it is addressing 2*/
                wordCounter++;
                if(!valid_register(line->args[1]) || !valid_register(line->args[2]))
                    wordCounter++;
            }
        } else {
            error(sprintf(error_message, LACK_OF_ARGUMENTS));
            return false;
            /*Expected 2 args error */
        }
        /* Count lines */
        if (hasLabel) {
            if (!(addLabel(line->label, cmd_list.length, isExt, isOp))) {
                return false;
            }
        }
            increase_cmd_length(++wordCounter); /* Count number of rows in cmd table*/
        break;

    case DOT_DATA:
        /* Save in data table and symbol table*/
        if (line->args != NULL) {
            char** arg = line->args;
            if (hasLabel) {
                if (!(addLabel(line->label, data_list.length, isExt, isOp))) {
                    return false;
                    /* Failed to add a label error */
                }
            }
            while (*arg != NULL) {
                int num;
                char* rst;
                /* Convert string to int */
                num = (int) strtol(*arg++, &rst, 10);
                if (strcmp(rst, "") == 0) { /* If no extra string*/
                    if (!(addData(num))) {
                        error(sprintf(error_message, OUT_OF_MEMORY));
                        return false;
                        /* Did't success to addData error */
                    }
                } else {
                    error(sprintf(error_message, UNKNOWN_ARGUMENT_TYPE));
                    return false;
                    /* String mix with int error */
                }
            }
        } else {
            error(sprintf(error_message, LACK_OF_ARGUMENTS));
            return false;
            /* Expected at least 1 arg error */
        }
        break;

    case DOT_STRING:
        /* Save in data table and symbol table*/
        if (line->args != NULL && line->args[1] == NULL) { /* Only one arg*/
            char* c = line->args[0];
            if (hasLabel) {
                if (!(addLabel(line->label, data_list.length, isExt, isOp))) {
                    return false;/* Error is set by addLabel already */
                    /* Did'nt success to add label error */
                }
            }
            if (*c++ == QUOTATION_MARK) {
                while (*c != QUOTATION_MARK) {
                    if (!(addData(*c++))) {
                        error(sprintf(error_message, OUT_OF_MEMORY));
                        return false;
                        /* Did't success to add char error */
                    }
                }
                if (*++c != '\0') {
                    error(sprintf(error_message, INELIGIBLE_FORMAT));
                    return false;
                }
                if (!(addData('\0'))) { /* End of string*/
                    error(sprintf(error_message, OUT_OF_MEMORY));
                    return false;
                    /* Did't succeed to add char error */
                }
            } else {
                error(sprintf(error_message, INELIGIBLE_FORMAT));
                return false;
                /* Not in a string format error */
            }
        } else {
            error(sprintf(error_message, INELIGIBLE_ARGUMENT_COUNT));
            return false;
            /* Expected 1 arg error */
        }
        break;
    case DOT_EXTERN:
        /* Save in symbol table*/
        isExt = true;
        if (line->args[1] == NULL) { /* Only one arg*/
            if (valid_label(line->args[0])) {
                if (addLabel(line->args[0], 0, isExt, isOp)) {

                } else {
                    return false;
                    /* Did't success to add label error */
                }
            } else {
                error(sprintf(error_message, ILLEGAL_LABEL, line->args[0]));
                return false;
                /* Extern not a valid label error */
            }
        } else {
            error(sprintf(error_message, LACK_OF_ARGUMENTS));
            return false;
            /* Expected at least 1 arg error */
        }
        break;
    case DOT_ENTRY:
        /* Do nothing */
        break;
    default:
        /* Not a command error. Probably impossible to reach. */
        error(sprintf(error_message, UNKNOWN_ERROR));
        return false;
        break;
	}

    return true;
}/* End handleLine */
