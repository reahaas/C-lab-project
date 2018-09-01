myProject: assembler.o cmdTable.o converter.o dataTable.o errorStates.o firstCycle.o main.o saveFiles.o secondCycle.o symbolTable.o
	gcc -g -ansi -Wall assembler.o cmdTable.o converter.o dataTable.o errorStates.o firstCycle.o main.o saveFiles.o secondCycle.o symbolTable.o myProject -lm

assembler.o: assembler.c
	gcc -c -ansi -Wall assembler.c -o assembler.o

cmdTable.o: cmdTable.c
	gcc -c -ansi -Wall cmdTable.c -o cmdTable.o

converter.o: converter.c
	gcc -c -ansi -Wall converter.c -o converter.o

dataTable.o: dataTable.c
	gcc -c -ansi -Wall dataTable.c -o dataTable.o

errorStates.o: errorStates.c
	gcc -c -ansi -Wall errorStates.c -o errorStates.o

firstCycle.o: firstCycle.c
	gcc -c -ansi -Wall firstCycle.c -o firstCycle.o

main.o: main.c
	gcc -c -ansi -Wall main.c -o main.o

saveFiles.o: saveFiles.c
	gcc -c -ansi -Wall saveFiles.c -o saveFiles.o

secondCycle.o: secondCycle.c
	gcc -c -ansi -Wall secondCycle.c -o secondCycle.o

symbolTable.o: symbolTable.c
	gcc -c -ansi -Wall symbolTable.c -o symbolTable.o