#include "assembler.h"
#include "errorStates.h"
#include "string.h"

/* External variables */
FLAG flag;
int string_index = 0;

/* Prototypes */
static bool get_next_argument(char *src, char *dest);
static int get_operator(const char **ops, const char *str, int ops_amount);
static int get_operator_valid(const char **ops, const char *str, const int ops_amount);
void trimmer(char * command_string, input_line * line);
bool rec_label(char *tmp_str, input_line *line);

const char *ops[] = { "mov", "cmp", "add", "sub", "not", "clr", "lea",
                      "inc", "dec", "jmp", "bne", "red", "prn", "jsr", "rts", "stop",
                      ".data", ".string", ".entry", ".extern" };

 static bool check_and_fix_second_addr(char *src, input_line *line) {       /* TODO check if static needed*/
	 static char *symbol, *first_parameter, *second_parameter, *check = NULL, *has_spaces = NULL;
	 check = strchr(src, '(');
	 has_spaces = strchr(src, SPACE);
	 if (check != NULL){
		if (has_spaces == NULL){
		 if (!valid_label(symbol = strtok(src, OPEN_PARENTHESES))){
			 return false;
		 } else if ((!valid_label_for_second_addressing(first_parameter = strtok(NULL, ","))) &&
					(!valid_register(first_parameter) && (!valid_number(first_parameter)))){
		 	return false;
		 }

		 else if ((!valid_label_for_second_addressing(second_parameter = strtok(NULL, CLOSE_PARENTHESES))) &&
				  (!valid_register(second_parameter) && (!valid_number(second_parameter)))){
		 	return false;
		 }
		 else {
			 if (!(copy_string(&(line->args[0]), symbol))){
				 free_line(line);
				 return false;
			 }
			 if (!(copy_string(&(line->args[1]), first_parameter))){
				 free_line(line);
				 return false;
			 }
			 if (!(copy_string(&(line->args[2]), second_parameter))){
				 free_line(line);
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
input_line * get_line(FILE *input){
	char cmd_string[MAXIMUM_LINE_LENGTH];
	char temp_string[MAXIMUM_LINE_LENGTH];
	input_line *line;
	int length;
	int i, status;
	if (!(line = malloc(sizeof(input_line)))){
		error(sprintf(error_message, OUT_OF_MEMORY));
		free_line(line);
		return NULL;
	}
	line->cmd = -1;
	line->label = NULL;
	line->unnecessary = false;
	line->is_end_of_file = false;
	line->args = malloc(sizeof(char *) * MAX_ARG_COUNT);

	for (i = 0; i < MAX_ARG_COUNT; i++)
		line->args[i] = NULL;

	if (!fgets(cmd_string, MAXIMUM_LINE_LENGTH, input)){
		line->is_end_of_file = true;
		free(line->args);
		return line;
	}

	if ((length = strlen(cmd_string)) == 0 || sscanf(cmd_string, "%s", temp_string) == 0 || temp_string[0] == COMMENT_SIGN){
		line->unnecessary = true;
		free(line->args);
		line->args = NULL;
		return line;
	} else if (cmd_string[length - 1] != NEWLINE){
		error(sprintf(error_message, LINE_EXCEEDS_LENGTH));
		free_line(line);
		return NULL;
	}

	trimmer(cmd_string, line);
	if (line->unnecessary)
		return line;

	sscanf(cmd_string, "%s", temp_string);

	if (!rec_label(temp_string, line))
		return NULL;

	if (!sscanf((cmd_string + string_index), "%s", temp_string)){
		error(sprintf(error_message, SYNTAX_ERROR MISSING_OPERATOR));
		free_line(line);
		return NULL;
	}

	if ((line->cmd = get_operator(ops, temp_string, sizeof(ops) / sizeof(ops[0]))) == -1) {
		free_line(line);
		return NULL;
	} else
		string_index += (strlen(temp_string) + (strcmp(temp_string, cmd_string + string_index) == 0 ? 0 : 1));
	if(line->cmd >= DOT_ENTRY){
		free(line->label);
		line->label = NULL;
	}

    i = THREE_OPERANDS;
	if (!(check_and_fix_second_addr(cmd_string + string_index, line))){
		i=0;
		if (!(status = get_next_argument(cmd_string + string_index, temp_string))){
			free(line->args);
			line->args = NULL;
		} else if (status == -1)
			return NULL;
		else {
			i++;
			if (!(copy_string(&(line->args[0]), temp_string))){
				free_line(line);
				return NULL;
			}
		}


		for (; (status = get_next_argument(NULL, temp_string)); i++) {
			if (status == -1) {
				free_line(line);
				return NULL;
			}
			if (!(copy_string(&(line->args[i]), temp_string))){
				free_line(line);
				return NULL;
			}
		}
    }

    if ((length = i) > 0)
			line->args = realloc(line->args, sizeof(char *) * (length + 1));
	string_index = 0;
	return line;
}

/*TODO change api*/
/**
 * Create canonical form of the command_string
 * @param command_string, the origin string
 * @param line, the input line object that scanned.
 */
void trimmer(char * command_string, input_line * line){
	char *p1, *p2;

	p1 = p2 = command_string;
	while (isspace(*p2))
		p2++;
	if (*p2 == '\0') {
		free(line->args);
		line->args = NULL;
		line->unnecessary = true;
		return;
	}

	for (; p2 - command_string < (strlen(command_string) -1 ); p2++){
		if (isspace(*p2) && isspace(*(p2 + 1)))
			continue;
		*p1++ = isspace(*p2) ? SPACE : *p2;
	}
	*p1 = '\0';
}

/**
 * handle the label section
 * @param tmp_str
 * @param line
 * @return true while label is valid
 */
bool rec_label(char *tmp_str, input_line *line){
    int length = (int)(strlen(tmp_str)) - 1;
	if (tmp_str[(length)] == LABEL_DELIM){                   /*TODO change delim */
		tmp_str[length] = '\0';
		if (valid_label(tmp_str)){
			if (copy_string(&(line->label), tmp_str))
                string_index += strlen(line->label) + 2;
			else {
				free_line(line);
				return false;
			}
		} else {
			error(sprintf(error_message, ILLEGAL_LABEL, tmp_str));
			free_line(line);
			return false;
		}
	} else {
		line->label = NULL;
		return true;
	}
}

void free_line(input_line *line){
	int i;
	if (line -> is_end_of_file)
		return;
	free(line->label);
	if (line->args != NULL)
		for (i = 0; line->args[i] != NULL; i++)
			free(line->args[i]);
	free(line->args);
	if(line){
		free(line);
	}
}

bool valid_label(const char *label_str){
	int i;
	if (get_operator_valid(ops, label_str, (sizeof(ops) / sizeof(ops[0]))) != -1){
		error(sprintf(error_message, ILLEGAL_LABEL, label_str));
		return false;
	}

	if (valid_register(label_str)){
		error(sprintf(error_message, ILLEGAL_LABEL, label_str));
		return false;
	}

	if (!isalpha(label_str[0])){
		return false;
	} else {
		for (i = 1; label_str[i]; i++){
			if (!isalnum(label_str[i]))
				return false;
		}
		return true;
	}
}

bool valid_register(const char *string_register){
	return ((string_register[0] == REG_FLAG && '0' <= string_register[1] && string_register[1] <= '7'	&& string_register[2] == '\0') ? true : false);
}


bool copy_string(char **dest, const char *src){
	int str_ind = 0;
	if (!(*dest = malloc(str_ind = (strlen(src) + 1)))){
		error(sprintf(error_message, OUT_OF_MEMORY));
		return false;
	}
	strcpy(*dest, src);
	return true;
}

bool string_to_int(const char *str, int *dest){
	int num;
	char *end;
	num = (int) strtol(str, &end, 10);
	if (strcmp(end, "") == 0) {
		*dest = num;
		return true;
	}
	return false;
}

/**
 * Gets the next argument from the current line.
 * @param src, the current line as a string.
 * @param dest, string for the next argument.
 * @return true while succeeded.
 */
static bool get_next_argument(char *src, char *dest){
    static char *cmd_str;
    int in_str = 0, i;
    if (src != NULL){
        cmd_str = src;
    }
    while (isspace(*cmd_str))                               /* TODO change isspace and the ctype file */
        cmd_str++;
    if (*cmd_str == '\0')
        return false;
    for (i = 0; *cmd_str != ARG_SEPERATOR && *cmd_str != '\0'; cmd_str++){
        if (in_str) {
            if (*cmd_str == STR_DELIM)                    /* TODO change delim*/
                in_str = 0;
        } else if (isspace(*cmd_str))
            break;
        else if (*cmd_str == STR_DELIM){
            in_str = 1;
        }
        dest[i] = *cmd_str;
        i++;
    }
    if (i == 0){
        error(sprintf(error_message, SYNTAX_ERROR EMPTY_ARGUMENT));
        return -1;
    }
    while (isspace(*cmd_str))
        cmd_str++;
    if (*cmd_str != '\0' && *cmd_str != ARG_SEPERATOR){
        error(sprintf(error_message, SYNTAX_ERROR UNKNOWN_ARGUMENT_TYPE));
        return -1;
    }
    dest[i] = '\0';
    if (*cmd_str == ARG_SEPERATOR){
        cmd_str++;
        if (*cmd_str == '\0'){
            error(sprintf(error_message, SYNTAX_ERROR EMPTY_ARGUMENT));
            return -1;
        }
    }
    return true;
}

static int get_operator(const char **ops, const char *str, int ops_amount){
	int i;
	for (i = 0; i < ops_amount; i++)
		if (strcmp(str, ops[i]) == 0)
			return i;
	error(sprintf(error_message, SYNTAX_ERROR UNKNOWN_OPERATOR));
	return -1;
}

static int get_operator_valid(const char **ops, const char *str, const int ops_amount){
	int i;
	for (i = 0; i < ops_amount; i++)
		if (strcmp(str, ops[i]) == 0)
			return i;
	return -1;
}

bool valid_number(char *str){
	int num;
	if (str[0] == IMD_FLAG) {                             /*TODO change IMD_FLAG*/
		if (!string_to_int(str + 1, &num)) {
			error(sprintf(error_message, SYNTAX_ERROR UNKNOWN_ARGUMENT_TYPE));
			return -1;
		}
	}														/* TODO return some value in else*/
}

bool valid_label_for_second_addressing(char *label_string){
	int i;
	if (get_operator_valid(ops, label_string, (sizeof(ops) / sizeof(ops[0]))) != -1){
		error(sprintf(error_message, ILLEGAL_LABEL, label_string));
		return false;
	}
	if (!isalpha(label_string[0])){
		return false;
	} else {
		for (i = 1; label_string[i]; i++){
			if (!isalnum(label_string[i]))
				return false;
		}
		return true;
	}
}