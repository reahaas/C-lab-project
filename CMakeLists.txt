cmake_minimum_required(VERSION 3.12)
project(C_lab_project)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(C_lab_project
        assembler.c
        assembler.h
        cmdTable.c
        cmdTable.h
        constants.h
        converter.c
        converter.h
        dataTable.c
        dataTable.h
        errorHandler.c
        errorHandler.h
        firstCycle.c
        main.c
        README.md
        saveFiles.c
        saveFiles.h
        secondCycle.c
        structs.h
        symbolStructs.h
        symbolTable.c
        symbolTable.h)