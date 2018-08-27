/*
 * converer.c
 *	Description: Converts number from different bases.
 */

#include "assembler.h"    
#include "converter.h"
#include <math.h>
// double log2(double);/* Throws an error without it for some reason */

/* TODO this function is from the previos project, need to delete */
/*char* base10to32enc(long unsigned int value) {
	char buffer[WORD_SIZE + 2];
	char *tmp;
	int offset = sizeof(buffer);
	buffer[--offset] = '\0';
	do {
		buffer[--offset] = base32(value % OUTPUT_BASE);
		if (offset <= 0)
			return NULL;
	} while (value /= OUTPUT_BASE);
	while (sizeof(buffer) / sizeof(buffer[0]) - offset <= WORD_SIZE_IN_BASE )/* Align to word size */
//		buffer[--offset] = '0';
//	copyStr(&tmp, buffer + offset);
//	return tmp; /* warning: this must be freed by the user */
//}*/

char * base10to2Wierd(long unsigned int value) {
	int i;
	const char base2[3] = "./";
	char *numInBaseWeird2 = (char *) malloc(WORD_SIZE-1);
	if (!numInBaseWeird2) {
		return NULL;
	}
	for (i = 0; i < WORD_SIZE-1; i++) {
		numInBaseWeird2[i] = '.';
	}
	i = WORD_SIZE-1;
	while(value!=0)
	{
		numInBaseWeird2[i] = base2[value%2];
		value=value/2;
		i--;
	}
	return numInBaseWeird2;
}

char *base10Decimal(long unsigned int value){
	int i;
	char *numInBase10Decimal = (char *) malloc(WORD_SIZE-1); // buffer
	if (!numInBase10Decimal){
		return NULL;
	}
	for (i = 0; i < 5; i++) {
		numInBase10Decimal[i] = '0';
	}
	snprintf(numInBase10Decimal, 10, "%d", value);

	return numInBase10Decimal;
	}

