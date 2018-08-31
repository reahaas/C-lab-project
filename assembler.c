#include "assembler.h"
#include "errorHandler.h"
#include "string.h"

/* External variables */
FLAG flag;
int string_index = 0;

/* Prototypes */
static bool get_next_argument(char *src, char *dest);
static int get_operator(const char **ops, const char *str, int opsAmount);
static int get_operator_valid(const char **ops, const char *str, const int opsAmount);
void trimmer(char * cmdStr, input_line * line);
bool rec_label(char *tmpStr, input_line *line);  /*TODO change name*/

const char *ops[] = { "mov", "cmp", "add", "sub", "not", "clr", "lea",
                      "inc", "dec", "jmp", "bne", "red", "prn", "jsr", "rts", "stop", /* Ops until here */
                      ".data", ".string", ".entry", ".extern" }; /* Instructions *//* The order of these command stay identical to the order of the enum constants in constants.h so the index will match the enum value */

 static bool handleForAddressing2(char *src, input_line *line) {
	 static char *label1, *first_parameter, *second_parameter, *check = NULL, *has_spaces = NULL;
	 check = strchr(src, '(');
	 has_spaces = strchr(src, SPACE);
	 if (check != NULL){
		if (has_spaces == NULL){
		 if (!valid_label(label1 = strtok(src, OPEN_PARENTHESES))){
			 return false;
		 } else if ((!valid_label_for_second_adrresing(first_parameter = strtok(NULL, ","))) &&
					(!valid_register(first_parameter) && (!valid_number(first_parameter)))){
		 	return false;
		 }

		 else if ((!valid_label_for_second_adrresing(second_parameter = strtok(NULL, CLOSE_PARENTHESES))) &&
				  (!valid_register(second_parameter) && (!valid_number(second_parameter)))){
		 	return false;
		 }
		 else {
			 if (!(copy_string(&(line->args[0]), label1))){                     /*TODO change label1*/
				 freeLine(line);
				 return false;
			 }
			 if (!(copy_string(&(line->args[1]), first_parameter))){
				 freeLine(line);
				 return false;
			 }
			 if (!(copy_string(&(line->args[2]), second_parameter))){
				 freeLine(line);
				 return false;
			 }
			 return true;
		 }
	 }
		 error(sprintf(error_message, SYNTAX_ERROR UNKNOWN_ARGUMENT_TYPE));
		 return false;
 }
	else {
			return false;
	}
}

/**
 * @param input, the in input from the user
 * @return pointer to the line that received
 * */
input_line * getLine(FILE *input){
	char cmd_string[MAXIMUM_LINE_LENGTH];
	char tmp_string[MAXIMUM_LINE_LENGTH];
	input_line *line;
	int length;
	int i, status;
	if (!(line = malloc(sizeof(input_line)))){
		error(sprintf(error_message, OUT_OF_MEMORY));
		freeLine(line);
		return NULL;
	}
	line->cmd = -1;
	line->label = NULL;
	line->unnecessary = false;
	line->is_end_of_file = false;
	line->args = malloc(sizeof(char *) * MAX_ARG_COUNT);/*max arg = 40*/

	for (i = 0; i < MAX_ARG_COUNT; i++) /* Set all cells to NULL */
		line->args[i] = NULL;

	if (!fgets(cmd_string, MAXIMUM_LINE_LENGTH, input)) { /* EOF encountered */
		line->is_end_of_file = true;
		free(line->args);  /* FIXME is this line is needed ???? rea add */
		return line;
	}

	if ((length = strlen(cmd_string)) == 0 || sscanf(cmd_string, "%s", tmp_string) == 0 || tmp_string[0] == COMMENT_SIGN) { /* Check for effect-less line */
		line->unnecessary = true;
		free(line->args);
		line->args = NULL;
		return line;
	} else if (cmd_string[length - 1] != NEWLINE) { /* Line exceeds max length */
		error(sprintf(error_message, LINE_EXCEEDS_LENGTH));
		freeLine(line);
		return NULL;
	}

	trimmer(cmd_string, line); /* put in cmd_string the canonical form of itself */
	if (line->unnecessary) /* FIXME is needed ??? */
		return line;

	sscanf(cmd_string, "%s", tmp_string);  /* copy the first string, which can be label, or command, tmp_string will be comment or label */

	if (!rec_label(tmp_string, line))   /* Recognise label section */
		return NULL; /* error handle is in the function */

	if (!sscanf((cmd_string + string_index), "%s", tmp_string)) { /* Read the next word for the operator recognition */
		error(sprintf(error_message, SYNTAX_ERROR MISSING_OPERATOR));
		freeLine(line);
		return NULL;
	}


	/* Recognises the operator if not operand error it*/
	if ((line->cmd = get_operator(ops, tmp_string, sizeof(ops) / sizeof(ops[0]))) == -1) {
		freeLine(line);
		return NULL;
	} else
		string_index += (strlen(tmp_string) + (strcmp(tmp_string, cmd_string + string_index) == 0 ? 0 : 1)); /* check if it's the last word in the line */
	if(line->cmd >= DOT_ENTRY){ /*if it a '.'/string/data or else*/
		free(line->label);
		line->label = NULL;
	}
	/* End operator section */
    i = THREE_OPERANDS;
	if (!(handleForAddressing2(cmd_string + string_index, line))){
		/* Separates arguments */
		/* get the first argument */
		i=0;
		if (!(status = get_next_argument(cmd_string + string_index,
										 tmp_string))) { /*tmpstr will be destination of the wanted arg, this is boolian so return 1 if successful*/
			free(line->args);
			line->args = NULL;
		} else if (status == -1)
			return NULL;
		else /* status is 1: success */
		{
			i++;
			if (!(copy_string(&(line->args[0]), tmp_string))) { /*first argument in. args is array of arrays */
				freeLine(line);
				return NULL;
			}
		} /* End of get the first argument */

		/* get all the other arguments */
		for (; (status = get_next_argument(NULL, tmp_string)); i++) {
			if (status == -1) {
				freeLine(line);
				return NULL;
			}
			if (!(copy_string(&(line->args[i]), tmp_string))) { /*putting other arguments in to args*/
				freeLine(line);
				return NULL;
			}
		} /* End of get all the other arguments */
		//}
    }

    if ((length = i) > 0) /*length is number of arguments in*/
			line->args = realloc(line->args, sizeof(char *) * (length + 1)); /* Can't fail because it's shrinking, the initial size of args is bigger or equal to the actual size of args */

		/* End arguments section */
	string_index = 0;
	return line;
}/* End getLine */

/**
 * put in cmdStr the canonical form of itself:
 * no spaces at the right & left, and only 1 space between each 2 arguments
 *
 * @param cmdStr origin string
 * @param line the input line object that builded for this line.
 */
void trimmer(char * cmdStr, input_line * line){
	char *p1, *p2;

	p1 = p2 = cmdStr;
	while (isspace(*p2))
		p2++;
	if (*p2 == '\0') {
		free(line->args);
		line->args = NULL;
		line->unnecessary = true;
		return;
	}

	for (; p2 - cmdStr < (strlen(cmdStr) -1 ); p2++) {
		if (isspace(*p2) && isspace(*(p2 + 1)))
			continue;
		*p1++ = isspace(*p2) ? SPACE : *p2;
	}
	*p1 = '\0';
}

/**
 * handle the label section
 * @param tmpStr
 * @param line
 * @return true while label is valid or it's not a label, otherwise false
 */
bool rec_label(char *tmpStr, input_line *line) {
    int length = (int)(strlen(tmpStr)) - 1;
	if (tmpStr[(length)] == LABEL_DELIM){ // if in last place there is ':'
		tmpStr[length] = '\0';
		if (valid_label(tmpStr)){
			if (copy_string(&(line->label), tmpStr)) /*copying label in to line->label*/
                string_index += strlen(line->label) + 2; /*putting the string index after the label name example: MAIN: [here]*/

			else {
				freeLine(line); /*doing free here don't need in label aswell*/
				return false; /* Error msg is placed in copy_string */
			}
		} else {
			error(sprintf(error_message, ILLEGAL_LABEL, tmpStr));
			freeLine(line); /*doing free here don't need in label aswell*/
			return false;
		}
	} else {
		line->label = NULL;
		return true;
	}/* End label section */
}
/* Release all allocated mem from struct */
void freeLine(input_line *line) {
	int i;
	if (line -> is_end_of_file)
		return;
	free(line->label);
	if (line->args != NULL)/* Release args */
		for (i = 0; line->args[i] != NULL; i++)
			free(line->args[i]);
	free(line->args);
	if(line){
		free(line);
	}
}/* End freeLine */


/* Validate whether the given str of a label is valid*/
bool valid_label(const char *labelStr) {
	int i;

	if (get_operator_valid(ops, labelStr, (sizeof(ops) / sizeof(ops[0]))) != -1){
		error(sprintf(error_message, ILLEGAL_LABEL, labelStr));
		return false;
	}

	if (valid_register(labelStr)) {/* Error. a label cannot be a register name */
		error(sprintf(error_message, ILLEGAL_LABEL, labelStr));
		return false;
	}

	if (!isalpha(labelStr[0])) {/* Starts with alphabetic */
		return false;
	} else {
		for (i = 1; labelStr[i]; i++) {/* Rest is alphanumeric */
			if (!isalnum(labelStr[i]))
				return false;
		}
		return true;
	}
}/* End valid_label */

/* Validates whether the given str of register name is legal. */
bool valid_register(const char *regStr) {
	return ((regStr[0] == REG_FLAG && '0' <= regStr[1] && regStr[1] <= '7'	&& regStr[2] == '\0') ? true : false);
} /* End valid_register */

/* Copies the text from src to *dest with malloc */
bool copy_string(char **dest, const char *src) {
	int strIndex = 0;
	if (!(*dest = malloc(strIndex = (strlen(src) + 1)))) {
		error(sprintf(error_message, OUT_OF_MEMORY));
		return false;
	}
	strcpy(*dest, src);
	return true;
}/* End copy_string */

/* Get int value of str */
bool strToInt(const char *str, int *dest) {
	int num;
	char *end;
	num = (int) strtol(str, &end, 10);
	if (strcmp(end, "") == 0) {
		*dest = num;
		return true;
	}
	return false;
}/* End strToInt */

/* Gets the next argument from the current line */
/* End get_next_argument */

/**
 * Gets the next argument from the current line.
 * @param src the current line as a string.
 * @param dest string to put the next argument.
 * @return true if it succesed, else return false.
 */
static bool get_next_argument(char *src, char *dest){
    static char *cmdStr;
    int inStr = 0, i;
    if (src != NULL) {
        cmdStr = src;
    }
    while (isspace(*cmdStr))
        cmdStr++;
    if (*cmdStr == '\0')
        return false;
    for (i = 0; *cmdStr != ARG_SEPERATOR && *cmdStr != '\0'; cmdStr++) {
        if (inStr) {
            if (*cmdStr == STR_DELIM)
                inStr = 0;
        } else if (isspace(*cmdStr))
            break;
        else if (*cmdStr == STR_DELIM) { // if starts a string
            inStr = 1;
        }
        dest[i] = *cmdStr; // will t
        i++;
    } // end of for
    if (i == 0) {
        error(sprintf(error_message, SYNTAX_ERROR EMPTY_ARGUMENT));
        return -1;
    }
    while (isspace(*cmdStr))
        cmdStr++;
    if (*cmdStr != '\0' && *cmdStr != ARG_SEPERATOR) {
        error(sprintf(error_message, SYNTAX_ERROR UNKNOWN_ARGUMENT_TYPE));
        return -1;
    }
    dest[i] = '\0';
    if (*cmdStr == ARG_SEPERATOR) {
        cmdStr++;
        if (*cmdStr == '\0') {
            error(sprintf(error_message, SYNTAX_ERROR EMPTY_ARGUMENT));
            return -1;
        }
    }
    return true;
}

/* Find the operator in the given op list*/
static int get_operator(const char **ops, const char *str, int opsAmount) {
	int i;
	for (i = 0; i < opsAmount; i++)
		if (strcmp(str, ops[i]) == 0) /* Found the op */
			return i;
	error(sprintf(error_message, SYNTAX_ERROR UNKNOWN_OPERATOR));
	return -1;
}/* End get_operator */


static int get_operator_valid(const char **ops, const char *str, const int opsAmount){
	int i;
	for (i = 0; i < opsAmount; i++)
		if (strcmp(str, ops[i]) == 0) /* Found the op */
			return i;
	return -1;
}/* End get_operator_valid */

bool valid_number(char *str){
	int num;
	if (str[0] == IMD_FLAG) {/* Is immediate number */
		if (!strToInt(str + 1, &num)) {
			error(sprintf(error_message, SYNTAX_ERROR UNKNOWN_ARGUMENT_TYPE));
			return -1;
		}
	}
}

bool valid_label_for_second_adrresing(char *labelStr){ /*WITHOUT VALID REG*/
	int i;

	if (get_operator_valid(ops, labelStr, (sizeof(ops) / sizeof(ops[0]))) != -1){ /*if it a operand so error out*/
		error(sprintf(error_message, ILLEGAL_LABEL, labelStr));
		return false;
	}
	if (!isalpha(labelStr[0])) {/* Starts with alphabetic */
		return false;
	} else {
		for (i = 1; labelStr[i]; i++) {/* Rest is alphanumeric */
			if (!isalnum(labelStr[i]))
				return false;
		}
		return true;
	}
}/* End valid_label */
