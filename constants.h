#ifndef CONSTANTS_H
#define CONSTANTS_H

#define NONE 0
#define MAX_MEMORY_SIZE 1000
#define MEMORY_START 100
#define MAXIMUM_LINE_LENGTH 81
#define MAX_ARG_COUNT ((MAXIMUM_LINE_LENGTH - 3)/2 + 1)
#define REG_OPERAND_SIZE 6
#define PARAMETER_SIZE 2
#define VALUE_SIZE 12
#define WORD_SIZE 14
#define EXTENSION_LENGTH 4
#define AS_EXTENSION ".as"
#define OB_EXTENSION ".ob"
#define EXT_EXTENSION ".ext"
#define ENT_EXTENSION ".ent"
#define THREE_OPERANDS 3
#define OPCODE_SIZE 4
#define OPCODES_AMOUNT 16
#define DECODE_SIZE 2
#define OPERAND_SIZE 2

enum ENCODE {
	ABS = 0, EXT, RLC
};

typedef enum ADDRESSING {
	IMD = 0, DIR, JWP, REG
} addressing;

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

#define NEWLINE '\n'
#define COMMENT_SIGN ';'
#define COLON ':'
#define QUOTATION_MARK '"'
#define REGISTER_FLAG 'r'
#define IMMEDIATE_FLAG '#'
#define COMMA ','
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
