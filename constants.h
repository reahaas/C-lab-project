#ifndef CONSTANTS_H
#define CONSTANTS_H

#define MAX_MEMORY_SIZE 1000
#define MEMORY_START 100
#define MAXIMUM_LINE_LENGTH 81
#define MAX_ARG_COUNT ((MAXIMUM_LINE_LENGTH - 3)/2 + 1)
#define DECODE_SIZE 2
#define OPERAND_SIZE 2
#define RED_OPERAND_SIZE 6
#define OPCODE_SIZE 4
#define PARAM_SIZE 2
#define VALUE_SIZE 12
#define NONE 0
#define OPCODES_AMOUNT 16
#define WORD_SIZE 14
#define EXTENSION_LENGTH 4
#define INPUT_EXTENSION ".as"
#define OBJ_EXT ".ob"
#define EXT_EXT ".ext"
#define ENT_EXT ".ent"
#define THREE_OPERANDS 3

/*
 *	The three types of encode:
 *	absolute, external and relocatable.
 */
enum ENCODE {
	ABS = 0, EXT, RLC
};
/*
 * For addressing there are three types:
 * immediate, direct, jump-with-params and direct-register.
 */
typedef enum ADDRESSING {
	IMD = 0, DIR, JWP, REG
} addressing;



/* Opcodes enum table */
typedef enum {
	MOV,
	CMP,
	ADD,
	SUB,
	NOT,
	CLR,
	LEA,
	INC,
	DEC,
	JMP,
	BNE,
	RED,
	PRN,
	JSR,
	RTS,
	STOP
} OPCODE;

typedef enum {
	DOT_DATA = OPCODES_AMOUNT, DOT_STRING, DOT_ENTRY, DOT_EXTERN
} inst_type;

/* Definition for all signs */
#define NEWLINE '\n'
#define COMMENT_SIGN ';'
#define LABEL_DELIM ':'
#define STR_DELIM '"'
#define REG_FLAG 'r'
#define IMMEDIATE_FLAG '#'
#define ARG_SEPERATOR ','
#define SPACE ' '
#define OPEN_PARENTHESES "("
#define CLOSE_PARENTHESES ")"

typedef enum {
	ASSEMBLER_SUCCESS, ASSEMBLER_FAILURE
} FLAG;

typedef enum {
	false = 0, true = 1
} bool;

#endif
