#include "dataTable.h"
#include "cmdTable.h"
#include "constants.h"
#include "math.h"
#include "converter.h"

dataList data_list = { NULL, 0 };

/**
 *@param data, after organized
 * @returns new_data_node, after added
 * */
static dataNode* create_data_node(int data) {
	dataNode* new_data_node = NULL;
	if ((new_data_node = malloc(sizeof(dataNode)))) {
		new_data_node->value = data;
		new_data_node->next = NULL;
	}

	return new_data_node;
}

/**
 * @param data, the data (char or int)
 * @returns true while successful adding occurs, false otherwise
 * */
int add_data(unsigned int data){
	dataNode *curr, *newNode;
	data_list.length++;
	if ((newNode = create_data_node(data % (int) pow(2, WORD_SIZE)))
			&& cmd_list.length + data_list.length < MAX_MEMORY_SIZE) {
		if (data_list.head == NULL){
			data_list.head = newNode;
		} else {
			for (curr = data_list.head; curr->next != NULL; curr = curr->next)
				;
			curr->next = newNode;
		}
		return true;
	}
	return false;
}
