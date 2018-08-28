/*
 * converter.c
 */

#include "assembler.h"    
#include "converter.h"
#include <math.h>

/*TODO all is running good delete, for now it is here for example if needed.*/
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
	char *numInBaseWeird2 = (char *) malloc(WORD_SIZE-1); /* TODO is the (-1) is needed?? */
	if (!numInBaseWeird2) {
		return NULL;
	}
	for (i = 0; i < WORD_SIZE-1; i++) {/* TODO is the (-1) is needed?? */
		numInBaseWeird2[i] = '.';
	}
	i = WORD_SIZE-1; /* TODO is the (-1) is needed?? */
	while(value!=0)
	{
		numInBaseWeird2[i] = base2[value%2];
		value=value/2;
		i--;
	}
	return numInBaseWeird2;
}


char *valueToBase10DecimalString(long unsigned int value){
	char *numInBase10Decimal = (char *) malloc(WORD_SIZE); /* TODO is the (-1) is needed?? */ /* buffer */
	if (!numInBase10Decimal){
		return NULL;
	}
	if(value<=999){
		sprintf( numInBase10Decimal, "0%d", value );
	}
	else if (value<=9999){
		sprintf( numInBase10Decimal, "%d", value) ;
	}
	return numInBase10Decimal;

}
