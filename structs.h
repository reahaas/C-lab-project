/*
 * structs.h
 */


#ifndef STRUCTS_H
#define STRUCTS_H

/* Structure of an operation (command) word */ /* TODO bit understand and changes */
typedef struct {/*p00 p00 oc0000 s00 d00 dc00 */
	unsigned int decode 	: DECODE_SIZE;
	unsigned int destOperand: OPERAND_SIZE;
	unsigned int srcOperand : OPERAND_SIZE;
	unsigned int opcode 	: OPCODE_SIZE;
	unsigned int paramOne 		: PARAM_SIZE;
	unsigned int paramTwo 		: PARAM_SIZE;
} cmd_word;

/* Structure of an argument word */
typedef struct {/*n000000000000 dc00*/
	unsigned int decode: DECODE_SIZE;
	unsigned int value : VALUE_SIZE;
} arg_word;


typedef struct { /*r1000000 r2000000 dc00*/
	unsigned int decode 	: DECODE_SIZE;
	unsigned int destOperand: RED_OPERAND_SIZE;
	unsigned int srcOperand : RED_OPERAND_SIZE;
} reg_word;

typedef union {/*can choose one of them*/
	cmd_word cmd;
	reg_word reg;
	arg_word num;
	unsigned int print :WORD_SIZE;
} word;

typedef struct{
	char *label;
	int cmd;
	char **args;
	bool isEffectless;
	bool isEOF;
}input_line;

#endif
