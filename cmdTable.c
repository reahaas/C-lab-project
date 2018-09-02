#include <stdio.h>
#include <stdlib.h>
#include "cmdTable.h"
#include "dataTable.h"
#include "converter.h"

CMD_LIST cmd_list = { NULL, MEMORY_START };

static cmdNode* create_cmd_node(word row) {
	cmdNode* newCmdNode = NULL;
	if ((newCmdNode = malloc(sizeof(cmdNode)))) {
		newCmdNode->value = row;
		newCmdNode->next = NULL;
	}
	return newCmdNode;
}

void increase_cmd_length(int n) {
	cmd_list.length += n;
}

int get_cmd_length(void) {
	int cmd_amount = 0;
	cmdNode *cmd_node = cmd_list.head;
	while (cmd_node->next != NULL){
		cmd_node = cmd_node->next;
		cmd_amount++;
	}
	return cmd_amount;
}

int add_argument(word arg) {
	cmdNode *current, *newNode;
	if ((newNode = create_cmd_node(arg))
			&& cmd_list.length + data_list.length < MAX_MEMORY_SIZE) {
		if (cmd_list.head == NULL)
			cmd_list.head = newNode;
		else {
			for (current = cmd_list.head; current->next != NULL; current =
					current->next);

			current->next = newNode;
		}
		cmd_list.length++;
		return true;
	}
	return false;
}

int add_cmd(unsigned int decode, unsigned int destOperand,
			unsigned int srcOperand, unsigned int opcode, unsigned int paramOneType,
			unsigned int paramTwoType) {
	cmdNode *current, *new_node;
	word newWord;
	newWord.cmd.decode = decode;
	newWord.cmd.destOperand = destOperand;
	newWord.cmd.srcOperand = srcOperand;
	newWord.cmd.opcode = opcode;
	newWord.cmd.paramOne = paramOneType;
	newWord.cmd.paramTwo = paramTwoType;
	if ((new_node = create_cmd_node(newWord))
			&& cmd_list.length + data_list.length < MAX_MEMORY_SIZE){
		if (cmd_list.head == NULL){
			cmd_list.head = create_cmd_node(newWord);
		} else {
			for (current = cmd_list.head; current->next != NULL; current =
					current->next);
			current->next = create_cmd_node(newWord);
		}
		return true;
	}
	return false;
}
