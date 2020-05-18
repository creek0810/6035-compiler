#ifndef _SYMBOL_TABLE_H
#define _SYMBOL_TABLE_H

#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "object.h"

/* define symbol and symbol table */
typedef struct Symbol Symbol;
typedef struct  SymbolTable SymbolTable;

struct Symbol {
    char *name;
    Object *value;
    Symbol *prev;
};

struct SymbolTable {
    SymbolTable *prev;
    Symbol *head;
};


/* function */
SymbolTable *push_symbol_table();
void pop_symbol_table();
Symbol *find_symbol(char *name);

/* upsert function */
void upsert_number_symbol(char *name, int value);
void upsert_str_symbol(char *name, char *str);
void upsert_array_symbol(char *name, Object *value);

/* debug function */
void print_table();

#endif