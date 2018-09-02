/*
 * secondRun.c
 */
#include <string.h>
#include "assembler.h"
#include "dataTable.h"
#include "cmdTable.h"
#include "symbolTable.h"
#include "errorHandler.h"
#include <math.h>

static bool handleLine2(input_line*);
static addressing getArgWord(const char *str, word *wrd);

FLAG secondRun(FILE *src) {
	input_line *line;
	int lineIndex;
	fseek(src, 0L, SEEK_SET);
	relocate(cmd_list.length); /*sorting out the addressing with padding*/
	for (lineIndex = 1; true; lineIndex++) { /* Runs through all the lines. */
		if ((line = getLine(src))) {
			if (line->isEOF) {
				freeLine(line);
				break; /* End when EOF is encountered */
			}
			if (line->isEffectless) {
				continue;
			}
			if (!handleLine2(line)) {
				if (cmd_list.length + data_list.length > MAX_MEMORY_SIZE) {
					error(sprintf(errMsg, OUT_OF_STORAGE));
					report(lineIndex);
					return flag;
				}
				report(lineIndex);
			}
		} else {
			report(lineIndex);
		}
		freeLine(line);
	}
	return flag;
}/* End secondRun */

/* Analyses an line of code and operates accordingly */
static bool handleLine2(input_line* line) {
	/* char* argslabel,*args2,*args3; */


	switch (line->cmd) {
	/* Two operands group */
	case MOV:
	case CMP:
	case ADD:
	case SUB:
	case LEA:
		if (line->args != NULL && line->args[1] != NULL && line->args[2] == NULL) { /* Two args */
			addressing srcAdders, destAdders; /*0,1,2,3*/
			word srcArg, destArg;/*command,regiter or number*/

			/* Handle source arg  */

			switch (srcAdders = getArgWord(line->args[0], &srcArg)) { /*getting the src address*/
				case REG:
					srcArg.reg.srcOperand = srcArg.reg.destOperand; /*fixing srcArg to be the src*/
					srcArg.reg.destOperand = 0;
				case IMD:
					if (line->cmd == LEA) {
						error(sprintf(errMsg, WRONG_ARG_FOR_FUNC));
						return false;
					}
					break;
				case DIR:
					break;
				case JWP:
				default:
					return false;
			}
			/* handle dest arg  */

			switch (destAdders = getArgWord(line->args[1], &destArg)) {
				case IMD:
					if (line->cmd != CMP) {
						error(sprintf(errMsg, WRONG_ARG_FOR_FUNC));
						return false;
					}
				case DIR:
				case REG:
					break;
				case JWP:
				default:
					return false;

			} /*end of switch that referse to 2 arguments*/

			addCmd(ABS, destAdders, srcAdders, line->cmd, IRELEVANT_BITS, IRELEVANT_BITS);
			if (destAdders == REG && (srcAdders == REG )) {
				word multiReg;
				multiReg.reg.destOperand = destArg.reg.destOperand;
				multiReg.reg.srcOperand = srcArg.reg.srcOperand;
				multiReg.reg.decode = ABS;
				addArg(multiReg);
			} else {
				if (srcAdders == DIR && srcArg.num.value == 0) {/* If the arg is external */
					if (!addExt(line->args[0], getCmdLength() + 1)) {
						return false;
					}
				}
				addArg(srcArg);
				if (destAdders == DIR && destArg.num.value == 0) {/* If the arg is external */
					if (!addExt(line->args[1], getCmdLength() + 1)) {
						return false;
					}
				}
				addArg(destArg);
			}
		} else {
			error(sprintf(errMsg, WRONG_ARG_COUNT));
			return false;
			/* Expected 2 arg error */
		}
		break;

	/* One operands group */

	case NOT:
	case CLR:
	case INC:
	case DEC:
	case RED:
	case PRN:
		/*argslabel = line-> args[0]; */
		/*	args2 = line -> args[1]; */
		/*	args3 = line -> args[2]; */
			if (line->args != NULL && line->args[1] == NULL) { /* Only one arg*/
			addressing adders; /*can be 0,1,2,3*/
			word arg;
			switch (adders = getArgWord(line->args[0], &arg)) {
			case IMD:
				if (line->cmd != PRN) {
					error(sprintf(errMsg, WRONG_ARG_FOR_FUNC));
					return false;
					break;
				}
			case DIR:
			case REG:
				addCmd(ABS, adders, IMD, line->cmd, IRELEVANT_BITS, IRELEVANT_BITS);
				if (adders == DIR && arg.num.value == 0) {/* If the arg is external */
					if (!addExt(line->args[0], getCmdLength() + 1)) {
						return false;
					}
				}
				addArg(arg);
				break;
			case JWP:   /* this case invalide syntex one operator jump in addressing 2  */
			default:
				return false;
			}
		} else {
			error(sprintf(errMsg, WRONG_ARG_COUNT));
			return false;
			/* Expected 1 arg error */
				break;
		}
		break;

	case JSR:
	case BNE:
	case JMP:
		if (line->args != NULL && line->args[1] == NULL){
			addressing addres;
			word arg;
			switch (addres = getArgWord(line->args[0], &arg)) { /* addres must be DIR because "getline()" test the formmat for the JWP lines */
				case DIR:
				case REG:
					addCmd(ABS, addres, IMD, line->cmd, IRELEVANT_BITS, IRELEVANT_BITS);
					if (addres == DIR && arg.num.value == 0) {/* If the arg is external */
						if (!addExt(line->args[0], getCmdLength() + 1)) {
							return false;
						}
					}
					addArg(arg);
					break;
				case IMD:
				case JWP: /* jump commands with only one param cant be JWP */
				default:
					return false;
					break;
			}
		} else if(line->args != NULL && line->args[1] != NULL && line->args[2] != NULL){
			addressing addresArgZero, addresArgOne, addresArgTwo ;
			word argZero, argOne, argTwo ;

			addresArgZero = getArgWord(line->args[0], &argZero); /* put the command word into argZero */

			addresArgZero = JWP;  /* the logic that lead till here means that the addressing is jump with two params  */
			addresArgOne = getArgWord(line->args[1], &argOne);
			addresArgTwo = getArgWord(line->args[2], &argTwo);

			addCmd(ABS, addresArgZero, IMD, line->cmd, addresArgOne, addresArgTwo);
			if ( argZero.num.value == 0 ) { /* argZero is a label, so i want to check If the arg is external */
				if (!addExt(line->args[0], getCmdLength() + 1)) {
					return false;
				}
			}
			addArg(argZero);

			if (addresArgOne == REG && (addresArgTwo == REG )) {
				word multiReg;
				multiReg.reg.destOperand = argTwo.reg.destOperand;
				multiReg.reg.srcOperand = argOne.reg.srcOperand;
				multiReg.reg.decode = ABS;
				addArg(multiReg);
			} else {
				if ( argOne.num.value == 0 ) { /* argOne is a label, so i want to check If the arg is external */
					if (!addExt(line->args[1], getCmdLength() + 1 )) {
						return false;
					}
				}
				addArg(argOne);

				if ( argTwo.num.value == 0 ) { /* argTwo is a label, so i want to check If the arg is external */
					if (!addExt(line->args[2], getCmdLength() + 1 )) {
						return false;
					}
				}
				addArg(argTwo);

			}

		} else {
			error(sprintf(errMsg, WRONG_ARG_COUNT));
			return false;
		}/*end of JWP (one or three operands) group*/
			break;
	/* No operands group */
	case RTS:
	case STOP:
		if (line->args == NULL) { /*no args*/
			addCmd(0, IMD, IMD, line->cmd, NO_ARGS, 0);
		} else {
			error(sprintf(errMsg, WRONG_ARG_COUNT));
			return false;
		}
		break;

	case DOT_ENTRY:
		if (line->args != NULL && line->args[1] == NULL) {
			label* entLable;
			if ((entLable = getLabel(line->args[0])) != NULL) {
				if (!(addEnt(entLable))) {
					return false;
				}
			} else {
				error(sprintf(errMsg, ENT_TO_UNDEF, line->args[0]));
				return false;
			}
		}
		break;
	case DOT_DATA:
	case DOT_STRING:
	case DOT_EXTERN:
		/* do nothing */
		break;
	default:
		/* Not a command error. Probably impossible to reach. */
		error(sprintf(errMsg, UNKNOWN_ERR));
		return false;
		break;
	}

    return true;
}/* End handleLine2 */


static addressing getArgWord(const char *str, word *wrd) { /*wrd is srcArg or desrArg*/
	int num;
	label *lbl;

	if (str[0] == IMD_FLAG) {/* Is immediate number */
		if (!strToInt(str + 1, &num)) {
			error(sprintf(errMsg, SYNTAX_ERROR UNKNOWN_ARG_TYPE));
			return -1;
		}
		wrd->num.decode = ABS;
		wrd->num.value =  num % (int) pow(2, VALUE_SIZE);  /* modulo on binary number remains only the less significant digits */
		return IMD;
	} else if (validReg(str)) {/* Is register name */
		wrd->reg.decode = ABS;
		wrd->reg.destOperand = str[1] - '0'; /*r3  = str[1] - '0' = 3, gives the number of register*/
		wrd->reg.srcOperand = 0;
		return REG;
	} else if (validLabel(str)) {/* Is label name */
		if (!(lbl = getLabel(str))) { /*getting the label from symbol list list*/
			error(sprintf(errMsg, UNKNOWN_LABEL, str));
			return -1;
		}
		wrd->num.value = lbl->address;
		wrd->num.decode = lbl->isExt ? EXT : RLC;
		return DIR;
	}
	error(sprintf(errMsg, SYNTAX_ERROR INVALID_ARG, str));/* Syntax error */
	return -1;
}
