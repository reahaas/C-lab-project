#include "assembler.h"
#include "converter.h"
#include <math.h>

char * change_base_2_weird(long unsigned int value){
	int i;
	const char base2[3] = "./";
	char *result = (char *) malloc(WORD_SIZE+1);
	if (!result) {
		return NULL;
	}
	for (i=0; i < WORD_SIZE-1 ;i++){
		result[i] = '.';
	}
	result[WORD_SIZE-1] = '\n';
	if(value == 0){
		result[WORD_SIZE-1] = base2[0];
	}
	while(value!=0)
	{
		result[i] = base2[value%2];
		value=value/2;
		i--;
	}
	return result;
}

char *print_value_in_string(long unsigned int value){
	char *result = (char *) malloc(WORD_SIZE);
	if (!result){
		return NULL;
	}
	if(value<=999){
		sprintf( result, "0%lu", value );
	}
	else if (value<=9999){
		sprintf( result, "%lu", value) ;
	}
	return result;
}

