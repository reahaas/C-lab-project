#ifndef STRUCTURES_H
#define STRUCTURES_H

#include <stdbool.h>
#include "constants.h"

typedef struct {
	unsigned int decode: DECODE_SIZE;
	unsigned int destOperand: OPERAND_SIZE;
	unsigned int srcOperand: OPERAND_SIZE;
	unsigned int opcode: OPCODE_SIZE;
	unsigned int paramOne: PARAMETER_SIZE;
	unsigned int paramTwo: PARAMETER_SIZE;
} cmd_word;

typedef struct {
	unsigned int decode: DECODE_SIZE;
	unsigned int value: VALUE_SIZE;
} arg_word;


typedef struct {
	unsigned int decode: DECODE_SIZE;
	unsigned int destOperand: REG_OPERAND_SIZE;
	unsigned int srcOperand: REG_OPERAND_SIZE;
} reg_word;

typedef union {
	cmd_word cmd;
	reg_word reg;
	arg_word num;
	unsigned int print :WORD_SIZE;
} word;

typedef struct{
	char *label;
	int cmd;
	char **args;
	bool unnecessary;
	bool is_end_of_file;
}input_line;

#endif
