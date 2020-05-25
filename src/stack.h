#ifndef _STACK_H
#define _STACK_H

#include <stdbool.h>
#include "symbolTable.h"
#include "object.h"
#include "node.h"

typedef struct StackNode StackNode;
struct StackNode {
    Object *value;
    StackNode *prev;
    bool is_reference;
};


void push(Object *obj, bool is_reference);
void stack_biop(Operator op);
void stack_unop(Operator op);
void stack_array_append(char *name);
void stack_assign(char *name);
void stack_array_assign();
bool stack_is_true();

#endif