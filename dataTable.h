#ifndef DATA_H
#define DATA_H

#include <stdio.h>
#include <stdlib.h>

typedef struct dataNode {
	unsigned int value;
	struct dataNode* next;
} dataNode;

typedef struct {
	dataNode* head;
	int length;
} dataList;

extern dataList data_list;

int add_data(unsigned int);

#endif
