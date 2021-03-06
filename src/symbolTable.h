#ifndef _SYMBOL_TABLE_H
#define _SYMBOL_TABLE_H

#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "object.h"

/*
    Manage obj pool and symbol table.
    When symbol table is popped, program will call obj clean function.
*/

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
void push_symbol_table();
void pop_symbol_table();
Symbol *find_symbol(char *name);
void upsert_symbol(char *name, Object *obj);

/* debug function */
void print_table();

#endif