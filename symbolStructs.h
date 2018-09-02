#ifndef SYMBOLSTRUCTS_H
#define SYMBOLSTRUCTS_H
#include "symbolTable.h"


typedef struct labelNode {
	label *this;
	struct labelNode *next;
} labelNode;

typedef struct {
	labelNode *head;
} labelList;

typedef struct externNode {
	struct {
		char *label_name;
		int address;
	}this;
	struct externNode *next;
} externNode;

typedef struct entryNode {
	struct {
		char *label_name;
		int address;
	}this;
	struct entryNode *next;
} entryNode ;

typedef struct {
	externNode *head;
} externList;

typedef struct {
	entryNode *head;
} entryList;

extern labelList symbol_list;

#endif