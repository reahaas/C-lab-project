#include "assembler.h"
#include "symbolTable.h"
#include "symbolStructs.h"

#include "converter.h"

static label *create_label(char *label_name, int address, bool is_extern,
						   bool isOperation);
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

/* Register an external flag in a list */
bool addExt(char *symbol, int address) {
	externNode *node = ext_list.head;

	if (!node && !(node = malloc(sizeof(node)))) {
		error(sprintf(error_message, OUT_OF_MEMORY));
		return false;
	}
	if (ext_list.head != NULL) {
		while (node->next)/* Skip to the end */
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
} /* End addExt */

/* Register an external flag in a list */
bool addEnt(label *lbl) {
	entryNode *node = ent_list.head;

	if (!node && !(node = malloc(sizeof(node)))) {
		error(sprintf(error_message, OUT_OF_MEMORY));
		return false;
	}
	if (ent_list.head != NULL) {
		while (node->next)/* Skip to the end */
			node = node->next;
		if (!(node->next = malloc(sizeof(node)))) {
			error(sprintf(error_message, OUT_OF_MEMORY));
			return false;
		}
		node = node->next;
	} else {
		ent_list.head = node;
	}
	node->this.label_name = lbl->label_name;
	node->this.address = lbl->address;
	node->next = NULL;
	return true;
} /* End addEnt */

/* Pops an extern reference and removes it from the list */
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
} /* End popExt */

/* Pops an extern reference and removes it from the list */
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
} /* End popEnt */

/* Fix all relocatable */
void relocate(int spacing) {
	labelNode *node = symbol_list.head;
	if (node) {
		do {
			if ((!node->this->is_operator) && (!node->this->is_extern)){
				node->this->address += spacing;
			}
		} while ((node = node->next));
	}
}/* End relocate */


/* Searches the symbol table for the name given */
bool findLabel(const char *name) {
	return getLabel(name) ? true : false;
}/* End firstLabel */

/* Returns the requested label to the user. NULL if does not exist */
label *getLabel(const char *name) {
	labelNode *node = symbol_list.head;
	do {
		if (strcmp(node->this->label_name, name) == 0) /*runs throw the list and finding label*/
			return node->this;
	} while ((node = node->next) != NULL);
	return NULL;
}/* End getLabel */

/* Free all the elements in the symbol table */
void freeSymbolTable(void) {
	labelNode *node = symbol_list.head;
	if (node != NULL) {
		do {
			symbol_list.head = node->next;
			free(node->this->label_name);
			free(node->this);
			free(node);
		} while ((node = node->next) != NULL);
		symbol_list.head = NULL;
	}
}/* End freeSymbolTable */

void printSymbolTable(void) {/* Debug only. */
	labelNode *node = symbol_list.head;
	printf("%s\t\t%s\t\t%s\t%s\n", "Label", "Address", "is_extern", "is_operator");
	if (node) {
		do {
			printf("%s\t\t%s\t\t%d\t\t%d\n", node->this->label_name,
				   change_base_2_weird(node->this->address), node->this->is_extern,
					node->this->is_operator);
		} while ((node = node->next) != NULL);
	}

}/* End printSymbolTable */

/* ~-~-~-~ PRIVATE FUNCTIONS ~-~-~-~ */

/* Creates a label with a dynamic allocated storage
 *
 * @return NULL for errors.
 */
static label *create_label(char *label_name, int address, bool is_extern, bool isOp) {
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
	newLabel->is_operator = isOp;
	return newLabel;
}/* End create_label */

/* Creates a label node with a dynamic allocated storage
 *
 * @return NULL for errors.
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
}/* End create_label_node */
