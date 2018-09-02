myProject: assembler.o cmdTable.o converter.o dataTable.o errorHandler.o firstRun.o main.o saveFiles.o secondRun.o symbolTable.o
	gcc -g -ansi -Wall assembler.o cmdTable.o converter.o dataTable.o errorHandler.o firstRun.o main.o saveFiles.o secondRun.o symbolTable.o -o myProject -lm

assembler.o: assembler.c
	gcc -c -ansi -Wall assembler.c -o assembler.o

cmdTable.o: cmdTable.c
	gcc -c -ansi -Wall cmdTable.c -o cmdTable.o

converter.o: converter.c
	gcc -c -ansi -Wall converter.c -o converter.o

dataTable.o: dataTable.c
	gcc -c -ansi -Wall dataTable.c -o dataTable.o

errorStates.o: errorHandler.c
	gcc -c -ansi -Wall errorHandler.c -o errorHandler.o

firstRun.o: firstRun.c
	gcc -c -ansi -Wall firstRun.c -o firstRun.o

main.o: main.c
	gcc -c -ansi -Wall main.c -o main.o

saveFiles.o: saveFiles.c
	gcc -c -ansi -Wall saveFiles.c -o saveFiles.o

secondRun.o: secondRun.c
	gcc -c -ansi -Wall secondRun.c -o secondRun.o

symbolTable.o: symbolTable.c
	gcc -c -ansi -Wall symbolTable.c -o symbolTable.o