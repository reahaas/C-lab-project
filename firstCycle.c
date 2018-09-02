#include "assembler.h"
#include "dataTable.h"
#include "cmdTable.h"
#include "symbolTable.h"

static bool analyze_line(input_line *);

/**
 * first cycle update ic and dc and build the symbol table.
 * @param source_file, the file the cycle work on.
 * @return flag, while error occur in the file, flag return true.
 */
FLAG firstCycle(FILE *source_file){
	input_line *line;
	int lineIndex;
	fseek(source_file, 0L, SEEK_SET);
	for (lineIndex = 1; true; lineIndex++){
		if ((line = get_line(source_file)))
		{
			if (line->is_end_of_file){
                return false;
				break;
			}
			if (line->unnecessary){
				continue;
			}
			if (!analyze_line(line)){
				report(lineIndex);
				if (cmd_list.length + data_list.length >= MAX_MEMORY_SIZE){
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
}

/**
 * Function scanning the line, checks legal addressing, if there are operators, labels and externs.
 * @param line, the line from source
 * @returns false while function can't recognise the line, otherwise true
 * */
static bool analyze_line(input_line *line){
	int is_operator = false;
	int is_extern = false;
	int has_label = (line->label ? true : false);
	int word_counter = 0;

	switch (line->cmd){
    case MOV:
    case CMP:
    case ADD:
    case SUB:
    case LEA:
        if (line->args != NULL){
            if (line->args[1] != NULL){
                if (!(valid_register(line->args[0]) && valid_register(line->args[1]))){
                    word_counter++;
                }
            } else {
                error(sprintf(error_message, LACK_OF_ARGUMENTS));
                return false;
            }
        } else {
            error(sprintf(error_message, LACK_OF_ARGUMENTS));
            return false;
        }
    case NOT:
    case CLR:
    case INC:
    case DEC:
    case RED:
    case PRN:
        word_counter++;
    case RTS:
    case STOP:
        is_operator = true;
        if (has_label) {
            if (!(add_label(line->label, cmd_list.length, is_extern, is_operator))){
                return false;
            }
        }
            increase_cmd_length(++word_counter);
        break;
    case JMP:
    case JSR:
    case BNE:
        is_operator = true;
        if (line->args != NULL){
            word_counter++;
            if(line->args[1] != NULL && line->args[2] != NULL){
                word_counter++;
                if(!valid_register(line->args[1]) || !valid_register(line->args[2]))
                    word_counter++;
            }
        } else {
            error(sprintf(error_message, LACK_OF_ARGUMENTS));
            return false;
        }
        if (has_label) {
            if (!(add_label(line->label, cmd_list.length, is_extern, is_operator))) {
                return false;
            }
        }
            increase_cmd_length(++word_counter);
        break;

    case DOT_DATA:
        if (line->args != NULL){
            char** arg = line->args;
            if (has_label) {
                if (!(add_label(line->label, data_list.length, is_extern, is_operator))) {
                    return false;
                }
            }
            while (*arg != NULL){
                int num;
                char* rst;
                num = (int) strtol(*arg++, &rst, 10);
                if (strcmp(rst, "") == 0){
                    if (!(add_data(num))){
                        error(sprintf(error_message, OUT_OF_MEMORY));
                        return false;
                    }
                } else {
                    error(sprintf(error_message, UNKNOWN_ARGUMENT_TYPE));
                    return false;
                }
            }
        } else {
            error(sprintf(error_message, LACK_OF_ARGUMENTS));
            return false;
        }
        break;

    case DOT_STRING:
        if (line->args != NULL && line->args[1] == NULL){
            char* c = line->args[0];
            if (has_label) {
                if (!(add_label(line->label, data_list.length, is_extern, is_operator))){
                    return false;
                }
            }
            if (*c++ == QUOTATION_MARK){
                while (*c != QUOTATION_MARK){
                    if (!(add_data(*c++))){
                        error(sprintf(error_message, OUT_OF_MEMORY));
                        return false;
                    }
                }
                if (*++c != '\0') {
                    error(sprintf(error_message, INELIGIBLE_FORMAT));
                    return false;
                }
                if (!(add_data('\0'))){
                    error(sprintf(error_message, OUT_OF_MEMORY));
                    return false;
                }
            } else {
                error(sprintf(error_message, INELIGIBLE_FORMAT));
                return false;
            }
        } else {
            error(sprintf(error_message, INELIGIBLE_ARGUMENT_COUNT));
            return false;
        }
        break;
    case DOT_EXTERN:
        is_extern = true;
        if (line->args[1] == NULL){
            if (valid_label(line->args[0])) {
                if (add_label(line->args[0], 0, is_extern, is_operator)){
                } else {
                    return false;
                }
            } else {
                error(sprintf(error_message, ILLEGAL_LABEL, line->args[0]));
                return false;
            }
        } else {
            error(sprintf(error_message, LACK_OF_ARGUMENTS));
            return false;
        }
        break;
    case DOT_ENTRY:
        break;
    default:
        error(sprintf(error_message, UNKNOWN_ERROR));
        return false;
        break;
	}
    return true;
}
