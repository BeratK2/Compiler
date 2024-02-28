// main.c

#include "symbol_table.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main() {
    SymbolTable symbolTable;
    initializeSymbolTable(&symbolTable, 10);

    char inputName[30];
    int inputValue;

    printf("Enter symbol table entries (type 'exit' to finish):\n");

    while (1) {
        printf("Name: ");
        scanf("%s", inputName);

        if (strcmp(inputName, "exit") == 0) {
            break;
        }

        printf("Value: ");
        scanf("%d", &inputValue);

        insertEntry(&symbolTable, inputName, inputValue);
    }

    printSymbolTable(&symbolTable);

    free(symbolTable.entries);

    return 0;
}
