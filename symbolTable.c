#include "assembler.h"
#include "symbolTable.h"
#include "symbolStructs.h"

#include "converter.h"

static label *create_label(char *label_name, int address, bool is_extern,
						   bool is_operator);
static labelNode *create_label_node(label *thisLabel, labelNode *nextNode);

labelList symbol_list = { NULL };

static externList ext_list = { NULL };
static entryList ent_list = { NULL };

/** Adds a new label to the symbol table.
 * @return status of errors.
 */
bool add_label(char *label_name, int address, bool is_extern, bool isOp){
	labelNode *new_label;
	labelNode *current = symbol_list.head;

	if (label_name == NULL){
		error(sprintf(error_message, LABEL_NAME_MISSING));
		return false;
	}
	while (current){
		if (strcmp(current->this->label_name, label_name) == 0){
			error(sprintf(error_message, MULTIPLE_LABEL_DEFINITIONS, label_name));
			return false;
		} else if (current->next){
			current = current->next;
		} else
			break;
	}

	new_label = create_label_node(create_label(label_name, address, is_extern, isOp), NULL);
	if (new_label == NULL)
		return false;

	if (!symbol_list.head)
		symbol_list.head = new_label;
	else
		current->next = new_label;
	return true;
}

/**
 * Function add to the external table the label
 * @param address
 * @param symbol
 * @returns true while successfully added
 * */
bool add_external(char *symbol, int address){
	externNode *node = ext_list.head;

	if (!node && !(node = malloc(sizeof(node)))) {
		error(sprintf(error_message, OUT_OF_MEMORY));
		return false;
	}
	if (ext_list.head != NULL) {
		while (node->next)
			node = node->next;
		if (!(node->next = malloc(sizeof(node)))) {
			error(sprintf(error_message, OUT_OF_MEMORY));
			return false;
		}
		node = node->next;
	} else {
		ext_list.head = node;
	}
	copy_string(&node->this.label_name, symbol);
	node->this.address = address + MEMORY_START;
	node->next = NULL;
	return true;
}

/**
 * Function add to the entries table the label
 * @param address
 * @param symbol
 * @returns true while successfully added
 * */
bool addEnt(label *label_pointer){
	entryNode *node = ent_list.head;

	if (!node && !(node = malloc(sizeof(node)))) {
		error(sprintf(error_message, OUT_OF_MEMORY));
		return false;
	}
	if (ent_list.head != NULL) {
		while (node->next)
			node = node->next;
		if (!(node->next = malloc(sizeof(node)))) {
			error(sprintf(error_message, OUT_OF_MEMORY));
			return false;
		}
		node = node->next;
	} else {
		ent_list.head = node;
	}
	node->this.label_name = label_pointer->label_name;
	node->this.address = label_pointer->address;
	node->next = NULL;
	return true;
}

/**
 * Function removes reference from the external list, and pop it out
 * @param address, to pop
 * @returns label_name
 * */
char *popExt(int *address) {
	externNode *node = ext_list.head;
	char *label_name;
	if (!node)
		return NULL;
	ext_list.head = node->next;
	*address = node->this.address;
	label_name = node->this.label_name;
	free(node);
	return label_name;
}

/**
 * Function removes reference from the entries list, and pop it out
 * @param address, to pop
 * @returns label_name
 * */
char *popEnt(int *address) {
	entryNode *node = ent_list.head;
	char *label_name;
	if (!node)
		return NULL;
	ent_list.head = node->next;
	*address = node->this.address;
	label_name = node->this.label_name;
	free(node);
	return label_name;
}

/**
 * Function sorting out the addressing with padding
 * @param spacing, number of spaces the function works from the 100nth char
 * */
void relocate(int spacing) {
	labelNode *node = symbol_list.head;
	if (node) {
		do {
			if ((!node->this->is_operator) && (!node->this->is_extern)){
				node->this->address += spacing;
			}
		} while ((node = node->next));
	}
}

/**
 * function find the required label
 * @param name, name of required label
 * @return the reuired label
 * */
label *getLabel(const char *name) {
	labelNode *node = symbol_list.head;
	do {
		if (strcmp(node->this->label_name, name) == 0)
			return node->this;
	} while ((node = node->next) != NULL);
	return NULL;
}

/**
 * function create node of label and insert all the parameters into the node fields
 * @param label_name, name of the label
 * @param address, 
 * */
static label *create_label(char *label_name, int address, bool is_extern, bool is_operator) {
	label *newLabel = malloc(sizeof(label));
	if (newLabel == NULL) {
		error(sprintf(error_message, OUT_OF_MEMORY));
		return NULL;
	}

	if (!copy_string(&newLabel->label_name, label_name)) {
		free(newLabel);
		error(sprintf(error_message, OUT_OF_MEMORY));
		return NULL;
	}
	newLabel->address = address;
	newLabel->is_extern = is_extern;
	newLabel->is_operator = is_operator;
	return newLabel;
}

/**
 * Function creates label node with dynamic allocated storage
 * @return NULL while error occur
 */
static labelNode *create_label_node(label *this, labelNode *next) {
	labelNode *newLabelNode = malloc(sizeof(labelNode));
	if (newLabelNode == NULL || this == NULL) {
		if (this)
			free(this->label_name);
		free(this);
		free(newLabelNode);
		error(sprintf(error_message, OUT_OF_MEMORY));
		return NULL;
	}
	newLabelNode->this = this;
	newLabelNode->next = next;
	return newLabelNode;
}
