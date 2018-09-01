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

FLAG assembler(FILE *source_file){
	flag = ASSEMBLER_SUCCESS;
	printf("\nBegin of the first cycle\n\n");
	firstCycle(source_file);
	if (flag == ASSEMBLER_SUCCESS){
		printf("\nFirst cycle succeeded, second cycle begin\n\n");
		secondCycle(source_file);
	} else{
		printf("\nFirst cycle failed\n\n");
	}
	if (flag == ASSEMBLER_SUCCESS){
		printf("\nSecond cycle succeeded, save files\n\n");
		printf("Data table is:\n\n\n");
		saveFiles();
		printf("\nSave files finished\n\n");
	} else {
		printf("\nSecond cycle failed\n\n");
	}
}
