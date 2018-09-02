#include "cmdTable.h"
#include "dataTable.h"
#include "assembler.h"
#include "converter.h"
#include "symbolStructs.h"
#include <string.h>

static bool save_object(FILE *file, int *i);
static bool save_data(FILE *file, int *i);
static bool save_external(FILE *file, int *i);
static bool save_entries(FILE *file, int *i);

void saveFiles(void){
	char *name;
	FILE *out;
	bool result;
	int i = MEMORY_START;
	copy_string(&name, cur_file_name);
	name[strlen(name) - EXTENSION_LENGTH + 1] = '\0';
	strcat(name, OB_EXTENSION);
	out = fopen(name, "w");
	result = save_object(out, &i);
	fclose(out);
	if (!result)
		remove(name);
	name[strlen(name) - EXTENSION_LENGTH + 1] = '\0';
	strcat(name, EXT_EXTENSION);
	out = fopen(name, "w");
	result = save_external(out, &i);
	fclose(out);
	if (!result)
		remove(name);

	name[strlen(name) - EXTENSION_LENGTH] = '\0';
	strcat(name, ENT_EXTENSION);
	out = fopen(name, "w");
	result = save_entries(out, &i);
	fclose(out);
	if (!result)
		remove(name);

	free(name);
}

static bool save_object(FILE *file, int *i){
	cmdNode* cmd_node = NULL;
	int start = *i, cmd_amount = 0;
	cmd_node = cmd_list.head;
	if (cmd_node != NULL) {
		while (cmd_node != NULL){
			cmd_node = cmd_node->next;
			cmd_amount++;
		}
	}
	fprintf(file, "%s\t%s\n", print_value_in_string(cmd_amount),
			print_value_in_string(data_list.length));
	printf("%s\t%s\n", print_value_in_string(cmd_amount),
		   print_value_in_string(data_list.length));
	for (cmd_node = cmd_list.head; cmd_node != NULL;
			cmd_node = cmd_node->next, (*i)++) {
		fprintf(file, "%s\t%s\n", print_value_in_string(*i),
				change_base_2_weird(cmd_node->value.print));
	}
	if (save_data(file, i))
		return true;

	if (start == *i)
		return false;
	else
		return true;
}


bool save_data(FILE *file, int *i){
	dataNode* data_node = NULL;
	int start = *i;

	for (data_node = data_list.head; data_node != NULL;
			data_node = data_node->next, (*i)++) {
		fprintf(file, "%s\t%s\n", print_value_in_string(*i),
				change_base_2_weird(data_node->value));
		printf("%s\t%s\n", print_value_in_string(*i),
			   change_base_2_weird(data_node->value));
	}
	if (start == *i)
		return false;
	else
		return true;
}


bool save_external(FILE *file, int *i) {
	char *name;
	int num;
	bool has_content = false;

	while ((name = popExt(&num))) {
		fprintf(file, "%s\t%s\n", name, print_value_in_string(num));
		printf("%s\t%s\n", name, print_value_in_string(num));
		free(name);
		has_content = true;
	}
	return has_content;
}


bool save_entries(FILE *file, int *i){
	char *name;
	int num;
	bool has_content = false;

	while ((name = popEnt(&num))) {
		fprintf(file, "%s\t%s\n", name, print_value_in_string(num));
		printf("%s\t%s\n", name, print_value_in_string(num));
		has_content = true;
	}
	return has_content;
}
