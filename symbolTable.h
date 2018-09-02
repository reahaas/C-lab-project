#ifndef SYMBOL_H
#define SYMBOL_H
#include <stdbool.h>


typedef struct {
	char *label_name;
	int address;
	bool is_extern;
	bool is_operator;
} label;


bool add_label(char *label_name, int address, bool is_extern, bool isOp);
label *getLabel(const char *name);
bool addExt(char *name, int address);
char *popExt(int *address);
bool addEnt(label *lbl);
char *popEnt(int *address);

#endif
