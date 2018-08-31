#include "assembler.h"
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include "symbolTable.h"
#include "cmdTable.h"
#include "dataTable.h"
#include "saveFiles.h"
#include "converter.h"

char *cur_file_name;

int main(int argc, char* argv[]) {
	int i;
	FILE *fd;
	srand(time(NULL)); /* seeds the random number generator used by the function rand */
	if (argc < 2) {
		fprintf(stderr, FILES_SHORT);
		return EXIT_FAILURE;
	} else {
		for (i = 1; i < argc; i++) {
			cur_file_name = malloc(strlen(argv[i]) + EXTENSION_LENGTH + 1);
			sprintf(cur_file_name, "%s%s", argv[i], INPUT_EXTENSION);
			if ((fd = fopen(cur_file_name, "r")) != NULL) {
				if (assembler(fd) == ASSEMBLER_FAILURE) {
					fprintf(stderr, ASSEMBLER_ERROR, argv[i], INPUT_EXTENSION);
				}else{
					fprintf(stdout, ASSEMBLER_DONE, argv[i], INPUT_EXTENSION);
				}
				fclose(fd);
			} else {
				fprintf(stderr, "Invalid file name\n");
			}
			free(cur_file_name);
		}
		return EXIT_SUCCESS;
	}
}


/* Assembler's functions from main*/
FLAG assembler(FILE *src) {
	flag = ASSEMBLER_SUCCESS;
	printf("\nBegin of the first run\n\n");
	firstCycle(src);
	if (flag == ASSEMBLER_SUCCESS) {
		printf("\nFirst run succeeded, start of the second run\n\n");

		secondRun(src);
	} else {
		printf("\nFirst run failed\n\n");
	}
	if (flag == ASSEMBLER_SUCCESS) {
		printf("\nsecond run succeeded, save files\n\n");
		printf("Data table is:\n\n\n");
		saveFiles();
		printf("\nSave files function finished\n\n");
	} else {
		printf("\nSecond run failed\n\n");
	}
}
