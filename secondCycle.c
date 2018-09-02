#include <string.h>
#include "assembler.h"
#include "dataTable.h"
#include "cmdTable.h"
#include "symbolTable.h"
#include "errorStates.h"
#include <math.h>

static bool handleLine2(input_line*);
static addressing getArgWord(const char *str, word *wrd);

FLAG secondCycle(FILE *src){
	input_line *line;
	int line_index;
	fseek(src, 0L, SEEK_SET);                           /*TODO change seek_set*/
	relocate(cmd_list.length);
	for (line_index = 1; true; line_index++){
		if ((line = get_line(src))){
			if (line->is_end_of_file){
				free_line(line);
				break;
			}
			if (line->unnecessary){
				continue;
			}
			if (!handleLine2(line)){
				if (cmd_list.length + data_list.length > MAX_MEMORY_SIZE) {
					error(sprintf(error_message, OUT_OF_STORAGE));
					report(line_index);
					return flag;
				}
				report(line_index);
			}
		} else {
			report(line_index);
		}
		free_line(line);
	}
	return flag;
}

static bool handleLine2(input_line* line) {

	switch (line->cmd) {
	case MOV:
	case CMP:
	case ADD:
	case SUB:
	case LEA:
		if (line->args != NULL && line->args[1] != NULL && line->args[2] == NULL){
			addressing src_addres, dest_addres;
			word src_arg, dest_arg;

			switch (src_addres = getArgWord(line->args[0], &src_arg)){
				case REG:
					src_arg.reg.srcOperand = src_arg.reg.destOperand;
					src_arg.reg.destOperand = 0;
				case IMD:
					if (line->cmd == LEA){
						error(sprintf(error_message, INELIGIBLE_ARGUMENTS_TYPE));
						return false;
					}
					break;
				case DIR:
					break;
				case JWP:
				default:
					return false;
			}

			switch (dest_addres = getArgWord(line->args[1], &dest_arg)){
				case IMD:
					if (line->cmd != CMP) {
						error(sprintf(error_message, INELIGIBLE_ARGUMENTS_TYPE));
						return false;
					}
				case DIR:
				case REG:
					break;
				case JWP:
				default:
					return false;
			}

			add_cmd(ABS, dest_addres, src_addres, line->cmd, NONE, NONE);
			if (dest_addres == REG && (src_addres == REG )) {
				word multiReg;
				multiReg.reg.destOperand = dest_arg.reg.destOperand;
				multiReg.reg.srcOperand = src_arg.reg.srcOperand;
				multiReg.reg.decode = ABS;
				add_argument(multiReg);
			} else {
				if (src_addres == DIR && src_arg.num.value == 0){
					if (!addExt(line->args[0], get_cmd_length() + 1)){
						return false;
					}
				}
				add_argument(src_arg);
				if (dest_addres == DIR && dest_arg.num.value == 0){
					if (!addExt(line->args[1], get_cmd_length() + 1)){
						return false;
					}
				}
				add_argument(dest_arg);
			}
		} else {
			error(sprintf(error_message, INELIGIBLE_ARGUMENT_COUNT));
			return false;
		}
		break;

	case NOT:
	case CLR:
	case INC:
	case DEC:
	case RED:
	case PRN:
			if (line->args != NULL && line->args[1] == NULL){
			addressing adders;
			word arg;
			switch (adders = getArgWord(line->args[0], &arg)){
			case IMD:
				if (line->cmd != PRN){
					error(sprintf(error_message, INELIGIBLE_ARGUMENTS_TYPE));
					return false;
					break;
				}
			case DIR:
			case REG:
				add_cmd(ABS, adders, IMD, line->cmd, NONE, NONE);
				if (adders == DIR && arg.num.value == 0){
					if (!addExt(line->args[0], get_cmd_length() + 1)){
						return false;
					}
				}
					add_argument(arg);
				break;
			case JWP:
			default:
				return false;
			}
		} else {
			error(sprintf(error_message, INELIGIBLE_ARGUMENT_COUNT));
			return false;
				break;
		}
		break;

	case JSR:
	case BNE:
	case JMP:
		if (line->args != NULL && line->args[1] == NULL){
			addressing addres;
			word arg;
			switch (addres = getArgWord(line->args[0], &arg)){
				case DIR:
				case REG:
					add_cmd(ABS, addres, IMD, line->cmd, NONE, NONE);
					if (addres == DIR && arg.num.value == 0) {
						if (!addExt(line->args[0], get_cmd_length() + 1)) {
							return false;
						}
					}
					add_argument(arg);
					break;
				case IMD:
				case JWP:
				default:
					return false;
					break;
			}
		} else if(line->args != NULL && line->args[1] != NULL && line->args[2] != NULL){
			addressing addresArgZero, addresArgOne, addresArgTwo ;       /* TODO change addresArg...*/
			word argZero, argOne, argTwo ;
			addresArgZero = getArgWord(line->args[0], &argZero);
			addresArgZero = JWP;
			addresArgOne = getArgWord(line->args[1], &argOne);
			addresArgTwo = getArgWord(line->args[2], &argTwo);

			add_cmd(ABS, addresArgZero, IMD, line->cmd, addresArgOne, addresArgTwo);
			if ( argZero.num.value == 0 ){
				if (!addExt(line->args[0], get_cmd_length() + 1)) {
					return false;
				}
			}
			add_argument(argZero);
			if (addresArgOne == REG && (addresArgTwo == REG )){
				word multiReg;
				multiReg.reg.destOperand = argTwo.reg.destOperand;
				multiReg.reg.srcOperand = argOne.reg.srcOperand;
				multiReg.reg.decode = ABS;
				add_argument(multiReg);
			} else {
				if ( argOne.num.value == 0 ){
					if (!addExt(line->args[1], get_cmd_length() + 1 )){
						return false;
					}
				}
				add_argument(argOne);

				if ( argTwo.num.value == 0 ){
					if (!addExt(line->args[2], get_cmd_length() + 1 )){
						return false;
					}
				}
				add_argument(argTwo);
			}
		} else {
			error(sprintf(error_message, INELIGIBLE_ARGUMENT_COUNT));
			return false;
		}
			break;

	case RTS:
	case STOP:
		if (line->args == NULL) {
			add_cmd(0, IMD, IMD, line->cmd, NONE, 0);
		} else {
			error(sprintf(error_message, INELIGIBLE_ARGUMENT_COUNT));
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
				error(sprintf(error_message, ENT_TO_UNDEF, line->args[0]));
				return false;
			}
		}
		break;
	case DOT_DATA:
	case DOT_STRING:
	case DOT_EXTERN:
		break;
	default:
		error(sprintf(error_message, UNKNOWN_ERROR));
		return false;
		break;
	}

    return true;
}

static addressing getArgWord(const char *str, word *wrd){
	int num;
	label *lbl;

	if (str[0] == IMMEDIATE_FLAG) {                    					/*TODO change IMMEDIATE_FLAG*/
		if (!string_to_int(str + 1, &num)) {
			error(sprintf(error_message, SYNTAX_ERROR UNKNOWN_ARGUMENT_TYPE));
			return -1;
		}
		wrd->num.decode = ABS;
		wrd->num.value =  num % (int) pow(2, VALUE_SIZE);
		return IMD;
	} else if (valid_register(str)){
		wrd->reg.decode = ABS;
		wrd->reg.destOperand = str[1] - '0';
		wrd->reg.srcOperand = 0;
		return REG;
	} else if (valid_label(str)){
		if (!(lbl = getLabel(str))){
			error(sprintf(error_message, UNKNOWN_LABEL, str));
			return -1;
		}
		wrd->num.value = lbl->address;
		wrd->num.decode = lbl->isExt ? EXT : RLC;
		return DIR;
	}
	error(sprintf(error_message, SYNTAX_ERROR INVALID_ARGUMENT, str));
	return -1;
}
