#include <stdio.h>
#include <stdlib.h>
#include "cmdTable.h"
#include "dataTable.h"
#include "converter.h"

/*Static Variables*/
CMD_LIST cmd_list = { NULL, MEMORY_START };

/*Private Functions*/
static cmdNode* createCmdNode(word row) {
	cmdNode* newCmdNode = NULL;
	if ((newCmdNode = malloc(sizeof(cmdNode)))) {
		newCmdNode->value = row;
		newCmdNode->next = NULL;
	}
	return newCmdNode;
}

void increaseCmdLength(int n) {
	cmd_list.length += n;
}


int getCmdLength(void) {
	int cmdAmount = 0;
	cmdNode *cmd_node = cmd_list.head;
	while (cmd_node->next != NULL){
		cmd_node = cmd_node->next;
		cmdAmount++;
	}
	return cmdAmount;
}

int addArg(word arg) {
	cmdNode *current, *newNode;
	if ((newNode = createCmdNode(arg))
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

int addCmd(unsigned int decode, unsigned int destOperand,
		unsigned int srcOperand, unsigned int opcode, unsigned int paramOneType,
		unsigned int paramTwoType) {
	cmdNode *current, *newNode;
	word newWord;

	newWord.cmd.decode = decode;
	newWord.cmd.destOperand = destOperand;
	newWord.cmd.srcOperand = srcOperand;
	newWord.cmd.opcode = opcode;
	newWord.cmd.paramOne = paramOneType;
	newWord.cmd.paramTwo = paramTwoType;
	if ((newNode = createCmdNode(newWord))
			&& cmd_list.length + data_list.length < MAX_MEMORY_SIZE){

		if (cmd_list.head == NULL){
			cmd_list.head = createCmdNode(newWord);
		} else {
			for (current = cmd_list.head; current->next != NULL; current =
					current->next)
				;
			current->next = createCmdNode(newWord);
		}
		return true;
	}
	return false;
}
