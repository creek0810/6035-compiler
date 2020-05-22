#include "symbolTable.h"

// current symbol table
SymbolTable *TABLE = NULL;
int TABLE_NUM = 0;

/* function */
// TODO: support free table
void push_symbol_table() {
    SymbolTable *new_table = calloc(1, sizeof(SymbolTable));
    new_table->prev = TABLE;
    TABLE = new_table;
    TABLE_NUM += 1;
}

void pop_symbol_table() {
    TABLE = TABLE->prev;
    TABLE_NUM -= 1;
}

Symbol *find_symbol(char *name) {
    SymbolTable *cur_table = TABLE;
    // find symbol in table list
    while(cur_table) {
        // find symbol in current table
        Symbol *cur_node = cur_table->head;
        while(cur_node) {
            if(strcmp(cur_node->name, name) == 0) {
                return cur_node;
            }
            cur_node = cur_node->prev;
        }
        cur_table = cur_table->prev;
    }
    return NULL;
}

/* upsert function */
void upsert_symbol(char *name, Object *obj) {
    Symbol *cur_symbol = find_symbol(name);
    if(cur_symbol) {
        // obj assign may allocate a new obj
        cur_symbol->value = obj_assign(cur_symbol->value, obj);
    } else {
        cur_symbol = calloc(1, sizeof(Symbol));
        if(cur_symbol == NULL) {
            printf("symbol malloc error: %s\n", name);
        }
        // set attribute
        cur_symbol->name = strdup(name);
        cur_symbol->value = copy_obj(obj);
        cur_symbol->prev = TABLE->head;
        // insert into table
        TABLE->head = cur_symbol;
    }
}

/* debug function */
void print_table() {
    int number = TABLE_NUM;
    SymbolTable *cur_table = TABLE;
    while(cur_table) {
        printf("======== TABLE %d ============\n", number);

        Symbol *cur_node = cur_table->head;
        while(cur_node) {
            printf("%s:\n    ", cur_node->name);
            print_object(cur_node->value);
            cur_node = cur_node->prev;
        }
        cur_table = cur_table->prev;
        number --;
    }
}


