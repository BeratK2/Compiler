// symbol_table.c

#include "symbol_table.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void initializeSymbolTable(SymbolTable *table, int capacity) {
    table->entries = (SymbolEntry *)malloc(capacity * sizeof(SymbolEntry));
    table->capacity = capacity;
    table->size = 0;
}

void insertEntry(SymbolTable *table, const char *name, int value) {
    if (table->size < table->capacity) {
        SymbolEntry *newEntry = &(table->entries[table->size]);
        strncpy(newEntry->name, name, sizeof(newEntry->name) - 1);
        newEntry->name[sizeof(newEntry->name) - 1] = '\0';
        newEntry->value = value;
        table->size++;
    } else {
        printf("Symbol table is full. Cannot insert entry.\n");
    }
}

void printSymbolTable(const SymbolTable *table) {
    printf("Symbol Table:\n");
    for (int i = 0; i < table->size; i++) {
        printf("Name: %s, Value: %d\n", table->entries[i].name, table->entries[i].value);
    }
    printf("\n");
}
