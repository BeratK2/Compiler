// symbol_table.h

#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

typedef struct {
    char name[30];
    int value;
} SymbolEntry;

typedef struct {
    SymbolEntry *entries;
    int capacity;
    int size;
} SymbolTable;

void initializeSymbolTable(SymbolTable *table, int capacity);
void insertEntry(SymbolTable *table, const char *name, int value);
void printSymbolTable(const SymbolTable *table);

#endif  // SYMBOL_TABLE_H
