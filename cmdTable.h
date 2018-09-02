#ifndef CMD_H
#define CMD_H

#include "assembler.h"

typedef struct cmdNode {
	word value;
	struct cmdNode* next;
} cmdNode;

typedef struct {
	cmdNode* head;
	int length;
} CMD_LIST;


extern CMD_LIST cmd_list;

int add_cmd(unsigned int decode, unsigned int destOperand, unsigned int srcOperand,
			unsigned int opcode, unsigned int paramOneType, unsigned int paramTwoType);

int add_argument(word arg);

int get_cmd_length(void);

void increase_cmd_length(int n);

#endif
