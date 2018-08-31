/*
 * main.c
 */

#include "assembler.h"
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include "symbolTable.h"
#include "cmdTable.h"
#include "dataTable.h"
#include "saveFiles.h"
#include "converter.h"

char *currFileName;

int main(int argc, char* argv[]) {
	int i;
	FILE *current;
	srand(time(NULL)); /* Generate seed */
	if (argc < 2) {
		fprintf(stderr, TOO_LESS_FILES);
		return EXIT_FAILURE;
	} else {
		for (i = 1; i < argc; i++) {
			currFileName = malloc(strlen(argv[i]) + EXT_LEN + 1);
			sprintf(currFileName, "%s%s", argv[i], INPUT_EXT);
			if ((current = fopen(currFileName, "r")) != NULL) { /* Opens the file */
				if (assembler(current) == ASM_FAILURE) {
					fprintf(stderr, ASSEMBLER_ERR, argv[i], INPUT_EXT);
				}else{
					fprintf(stdout, ASSEMBLER_SUCCESS, argv[i], INPUT_EXT);
				}
				fclose(current); /* Closes the file */
			} else {
				fprintf(stderr, "Invalid file name\n");/* Here comes error handler for invalid input file */
			}
			free(currFileName);
		}
		return EXIT_SUCCESS;
	}
}


/* Main assembler function. */
FLAG assembler(FILE *src) {
	flag = ASM_SUCCESS;
	printf("\nstart the first run\n\n");
	firstRun(src); /* if an error occurs during the first run, flag turn to ASM_FAILURE */
	if (flag == ASM_SUCCESS) {
		printf("\nfirst run succede, start secont run\n\n");

		secondRun(src); /* if an error occur during the second run, flag turn to ASM_FAILURE */
	} else {
		printf("\nfirst run failure\n\n");
	}
	if (flag == ASM_SUCCESS) {
		printf("\nsecond run succeded, start save files\n\n");
		printf("the data table is:\n\n\n");
		saveFiles();
		printf("\nsave files function finished\n\n");
	} else {
		printf("\nsecond run failure\n\n");
	}
}
