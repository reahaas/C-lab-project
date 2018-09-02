#include <string.h>
#include "assembler.h"
#include "dataTable.h"
#include "cmdTable.h"
#include "symbolTable.h"
#include "errorStates.h"
#include <math.h>

static bool analyze_line2(input_line *);
static addressing get_word_for_argument(const char *str, word *wrd);

/**
 * The second cycle function prepare the line for analyze2, look for error if exists
 * @param src, the line
 * @returns flag
 * */
FLAG secondCycle(FILE *src){
	input_line *line;
	int line_index;
	fseek(src, 0L, SEEK_SET);
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
			if (!analyze_line2(line)){
				if (cmd_list.length + data_list.length > MAX_MEMORY_SIZE){
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

/**
 * Analyze the line by using the addressing methods by cases
 * @param line, the input line
 * @returns true while line successfully analyze, false otherwise
 * */
static bool analyze_line2(input_line *line){
	switch (line->cmd){
	case MOV:
	case CMP:
	case ADD:
	case SUB:
	case LEA:
		if (line->args != NULL && line->args[1] != NULL && line->args[2] == NULL){
			addressing src_addres, dest_addres;
			word src_arg, dest_arg;
			switch (src_addres = get_word_for_argument(line->args[0], &src_arg)){
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

			switch (dest_addres = get_word_for_argument(line->args[1], &dest_arg)){
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
			switch (adders = get_word_for_argument(line->args[0], &arg)){
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
			switch (addres = get_word_for_argument(line->args[0], &arg)){
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
			word argument0, argument1, argument2;
			addressing address_argument0, address_argument1, address_argument2;
			address_argument0 = get_word_for_argument(line->args[0], &argument0);
			address_argument1 = get_word_for_argument(line->args[1], &argument1);
			address_argument2 = get_word_for_argument(line->args[2], &argument2);

			add_cmd(ABS, address_argument0, IMD, line->cmd, address_argument1, address_argument2);
			if ( argument0.num.value == 0 ){
				if (!addExt(line->args[0], get_cmd_length() + 1)) {
					return false;
				}
			}
			add_argument(argument0);
			if (address_argument1 == REG && (address_argument2 == REG )){
				word multi_registers;
				multi_registers.reg.destOperand = argument2.reg.destOperand;
				multi_registers.reg.srcOperand = argument1.reg.srcOperand;
				multi_registers.reg.decode = ABS;
				add_argument(multi_registers);
			} else {
				if ( argument1.num.value == 0 ){
					if (!addExt(line->args[1], get_cmd_length() + 1 )){
						return false;
					}
				}
				add_argument(argument1);

				if ( argument2.num.value == 0 ){
					if (!addExt(line->args[2], get_cmd_length() + 1 )){
						return false;
					}
				}
				add_argument(argument2);
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
				error(sprintf(error_message, UNDEFINED_ENTRY, line->args[0]));
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
	}
    return true;
}

static addressing get_word_for_argument(const char *str, word *wrd){
	int num;
	label *lbl;

	if (str[0] == IMMEDIATE_FLAG){
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
		wrd->num.decode = lbl->is_extern ? EXT : RLC;
		return DIR;
	}
	error(sprintf(error_message, SYNTAX_ERROR INVALID_ARGUMENT, str));
	return -1;
}
