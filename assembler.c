/* assembler.c
 */

#include "assembler.h"
#include "errorHandler.h"

/*############EXTERN VARIABLES#############*/
FLAG flag; /* 0 for Success, non zero for failure */
int ic; /* Instructions Counter */
int dc; /* Data Counter */
int strIndex = 0; /* Index for next read in the current line */

/* Prototypes */
static bool getNextArg(char *src, char *dest);
static int getOp(const char **ops, const char *str, int opsAmount);
static int getOpForValidLabel(const char **ops, const char *str, const int opsAmount);
void trimmer(char * cmdStr, input_line * line);
bool RecogniseLabelSection(char  * tmpStr, input_line * line);
/* End prototypes */

const char *ops[] = { "mov", "cmp", "add", "sub", "not", "clr", "lea",
                      "inc", "dec", "jmp", "bne", "red", "prn", "jsr", "rts", "stop", /* Ops until here */
                      ".data", ".string", ".entry", ".extern" }; /* Instructions *//* The order of these command stay identical to the order of the enum constants in constants.h so the index will match the enum value */

/* Gets a line of code */
input_line * getLine(FILE *input) {

	char cmdStr[LINE_MAX_LEN];
	char tmpStr[LINE_MAX_LEN];
	/* TODO : is needed???:  "char *p1, *p2;" */
	input_line *line;
	int length; /* Temporary length */
	int i, status;

	if (!(line = malloc(sizeof(input_line)))) { /* Mem initialisation */
		error(sprintf(errMsg, OUT_OF_RAM));
		freeLine(line);
		return NULL;
	}
	line->cmd = -1;
	line->label = NULL;
	line->isEffectless = false;
	line->isEOF = false;
	line->args = malloc(sizeof(char *) * MAX_ARG_COUNT);

	for (i = 0; i < MAX_ARG_COUNT; i++) /* Set all cells to NULL */
		line->args[i] = NULL;

	if (!fgets(cmdStr, LINE_MAX_LEN, input)) { /* EOF encountered */
		line->isEOF = true;
		/* free(line->args) */  /* FIXME is this line is needed ???? rea add */
		return line;
	}
/* FIXME check the COMMENT_SIGN, is it the same as in our project?*/
	if ((length = strlen(cmdStr)) == 0 || sscanf(cmdStr, "%s", tmpStr) == 0 || tmpStr[0] == COMMENT_SIGN) { /* Check for effect-less line */
		line->isEffectless = true;
		free(line->args);
		line->args = NULL;
		return line;
	} else if (cmdStr[length - 1] != NEWLINE) { /* Line exceeds max length */
		error(sprintf(errMsg, LINE_EXCEEDS_LEN));
		freeLine(line);
		return NULL;
	}

	trimmer(cmdStr, line); /* put in cmdStr the canonical form of itself */
	if (line->isEffectless) /* FIXME is needed ??? */
		return line;

	sscanf(cmdStr, "%s", tmpStr);  /* copy the first string, which can be label, or command, tmpStr will be comment or label */

	if (!RecogniseLabelSection(tmpStr, line))   /* Recognise label section */
		return NULL; /* error handle is in the function */

	if (!sscanf((cmdStr + strIndex), "%s", tmpStr)) { /* Read the next word for the operator recognition */
		error(sprintf(errMsg, SYNTAX_ERROR MISSING_OP));
		freeLine(line);
		return NULL;
	}


	/* Recognises the operator */
	if ((line->cmd = getOp(ops, tmpStr, sizeof(ops) / sizeof(ops[0]))) == -1) {
		freeLine(line);
		return NULL;
	} else
		strIndex += (strlen(tmpStr) + (strcmp(tmpStr, cmdStr + strIndex) == 0 ? 0 : 1)); /* check if it's the last word in the line */
	if(line->cmd >= DOT_ENTRY){
		free(line->label);
		line->label = NULL;
	}
	/* End operator section */

	/* Separates arguments */
	/* get the first argument */
	i = 0;
	if (!(status = getNextArg(cmdStr + strIndex, tmpStr))) {
		free(line->args);
		line->args = NULL;
	} else if (status == -1)
		return NULL;
	else /* status is 1: success */
	{
		i++;
		if (!(copyStr(&(line->args[0]), tmpStr))) {
			freeLine(line);
			return NULL;
		}
	} /* End of get the first argument */

	/* get all the other arguments */
	for (; (status = getNextArg(NULL, tmpStr)); i++) {
		if (status == -1){
			freeLine(line);
			return NULL;
		}
		if (!(copyStr(&(line->args[i]), tmpStr))) {
			freeLine(line);
			return NULL;
		}
	} /* End of get all the other arguments */

	if ((length = i) > 0)
		line->args = realloc(line->args, sizeof(char *) * length + 1); /* Can't fail because it's shrinking, the initial size of args is bigger or equal to the actual size of args */

	/* End arguments section */
	strIndex = 0;
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
		line->isEffectless = true;
		return;
	}

	for (; p2 - cmdStr < (strlen(cmdStr) -1 ); p2++) { // length is not global !
		if (isspace(*p2) && isspace(*(p2 + 1)))
			continue;
		*p1++ = isspace(*p2) ? SPACE : *p2;
	}
	*p1 = '\0';
	printf("%s\n", cmdStr); /* TODO remove this print (rea add) */
}/* End trimmer */

/**
 * handle the label section
 * @param tmpStr
 * @param line
 * @return true while label is valid or it's not a label, otherwise false
 */
bool RecogniseLabelSection(char  * tmpStr, input_line *line) {
    int length = (int)(strlen(tmpStr)) - 1;
	if (tmpStr[(length)] == LABEL_DELIM){ // if in last place there is ':'
		tmpStr[length] = '\0';
		if (validLabel(tmpStr)){
			if (copyStr(&(line->label), tmpStr))
                strIndex += strlen(line->label) + 2;

			else {
				freeLine(line); /*doing free here don't need in label aswell*/
				return false; /* Error msg is placed in copyStr */
			}
		} else {
			error(sprintf(errMsg, ILLEGAL_LABEL, tmpStr));
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
	if (!line)
		return;
	free(line->label);
	if (line->args != NULL)/* Release args */
		for (i = 0; line->args[i] != NULL; i++)
			free(line->args[i]);
	free(line->args);
	free(line);
}/* End freeLine */

/* TODO add validCommand(tmpStr) {if(getOps())!= -1 : return false? return true;} to check if the label is a reserve word or not  */
/* Validate whether the given str of a label is valid*/
bool validLabel(const char *labelStr) {
	int i;

	if (getOpForValidLabel(ops,labelStr,(sizeof(ops) / sizeof(ops[0]))) != -1){
		error(sprintf(errMsg, ILLEGAL_LABEL, labelStr));
		return false;
	}

	if (validReg(labelStr)) {/* Error. a label cannot be a register name */
		error(sprintf(errMsg, ILLEGAL_LABEL, labelStr));
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
}/* End validLabel */

/* Validates whether the given str of register name is legal. */
bool validReg(const char* regStr) {
	return ((regStr[0] == REG_FLAG && '0' <= regStr[1] && regStr[1] <= '7'	&& regStr[2] == '\0') ? true : false);
} /* End validReg */

/* Copies the text from src to *dest with malloc */
bool copyStr(char **dest, const char *src) {
	int strIndex = 0;
	if (!(*dest = malloc(strIndex = (strlen(src) + 1)))) {
		error(sprintf(errMsg, OUT_OF_RAM));
		return false;
	}
	strcpy(*dest, src);
	return true;
}/* End copyStr */

/* Get int value of str */
bool strToInt(const char *str, int *dest) {
	int num;
	char *end;
	num = strtol(str, &end, 10);
	if (strcmp(end, "") == 0) {
		*dest = num;
		return true;
	}
	return false;
}/* End strToInt */

/**
 * Gets the next argument from the current line.
 * @param src the current line as a string.
 * @param dest string to put the next argument.
 * @return true if it succesed, else return false.
 */
static bool getNextArg(char *src, char *dest) {
	static char *cmdStr;
	int inStr = 0, i;
	if (src != NULL) {
		cmdStr = src;
	}
	while (isspace(*cmdStr))
		cmdStr++;
	if (*cmdStr == '\0')
		return false;
	for (i = 0; (*cmdStr != ARG_SEPERATOR  )  && *cmdStr != '\0'; cmdStr++) /* TODO resolved the issue of JWP line */
	{
		/* TODO printf("%s\n", cmdStr); */ /* TODO remove this print (rea add) */
		if (inStr) {
			if (*cmdStr == STR_DELIM)
				inStr = 0;
		} else if (isspace(*cmdStr))
			break;
		else if (*cmdStr == STR_DELIM) {
			inStr = 1;
		}
		dest[i] = *cmdStr;
		i++;
	}
	if (i == 0) {
		error(sprintf(errMsg, SYNTAX_ERROR EMPTY_ARG));
		return -1;
	}
	while (isspace(*cmdStr))
		cmdStr++;
	if (*cmdStr != '\0' && *cmdStr != ARG_SEPERATOR) {
		error(sprintf(errMsg, SYNTAX_ERROR UNKNOWN_ARG_TYPE));
		return -1;
	}
	dest[i] = '\0';
	if (*cmdStr == ARG_SEPERATOR) {
		cmdStr++;
		if (*cmdStr == '\0') {
			error(sprintf(errMsg, SYNTAX_ERROR EMPTY_ARG));
			return -1;
		}
	}
	printf("%s\n", dest); /* TODO remove this print (rea add) */
	return true;
}/* End getNextArg */

/* Find the operator in the given op list*/
static int getOp(const char **ops, const char *str, const int opsAmount) {
	int i;
	for (i = 0; i < opsAmount; i++)
		if (strcmp(str, ops[i]) == 0) /* Found the op */
			return i;
	error(sprintf(errMsg, SYNTAX_ERROR UNKNOWN_OP));
	return -1;
}/* End getOp */


static int getOpForValidLabel(const char **ops, const char *str, const int opsAmount){
	int i;
	for (i = 0; i < opsAmount; i++)
		if (strcmp(str, ops[i]) == 0) /* Found the op */
			return i;
	return -1;
}/* End getOpForValidLabel */