/*
 * converter.c
 */

#include "assembler.h"    
#include "converter.h"
#include <math.h>

char * base10to2Wierd(long unsigned int value) {
	int i;
	const char base2[3] = "./";
	char *numInBaseWeird2 = (char *) malloc(WORD_SIZE+1); /* the (+1) is for '\0' */
	if (!numInBaseWeird2) {
		return NULL;
	}
	for (i=0; i < WORD_SIZE-1 ;i++){
		numInBaseWeird2[i] = '.';
	}
	numInBaseWeird2[WORD_SIZE-1] = '\n'; /* the end of the string */
	if(value == 0){
		numInBaseWeird2[WORD_SIZE-1] = base2[0];
	}
	while(value!=0)
	{
		numInBaseWeird2[i] = base2[value%2];
		value=value/2;
		i--;
	}
	return numInBaseWeird2;
}



char *valueToBase10DecimalString(long unsigned int value){
	char *numInBase10Decimal = (char *) malloc(WORD_SIZE); /* buffer */
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

