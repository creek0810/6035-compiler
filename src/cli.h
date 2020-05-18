#ifndef _COMPILER_H
#define _COMPILER_H

#include "node.h"
#include "parser.h"
#include "tokenizer.h"
#include "interpreter.h"

extern Node *tree;
extern void print_token(char*);
extern void print_tree(char*);


#endif