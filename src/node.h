#ifndef _NODE_H
#define _NODE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* enumerate */
typedef enum ASTType ASTType;
typedef enum Operator Operator;

enum ASTType{
    binaryNode = 0,
    unaryNode = 1,
    strNode = 2,
    numberNode = 4,
    identNode = 5,
    ifNode = 6,
    forNode = 7,
    blockNode = 8,
    assignNode = 9,
    printNode = 10,
    arrayNode = 11,
    lenNode = 12,
};

enum Operator {
    add = 0,
    sub = 1,
    mod = 2,
    div_ = 3,
    and = 4,
    or = 5,
    xor = 6,
    logic_or = 7,
    logic_and = 8,
    shl = 9,
    shr = 10,
    eq = 11,
    ne = 12,
    lt = 13,
    gt = 14,
    le = 15,
    ge = 16,
    not = 17,
    bit_not = 18,
    mul = 19,
    break_ = 20,
    continue_ = 21,
    getArray = 22,
    input_ = 23,
    toInt = 24,
};


/* union and struct */

typedef struct NodeList NodeList;
typedef struct BinaryNode BinaryNode;
typedef struct UnaryNode UnaryNode;
typedef struct IfNode IfNode;
typedef struct ForNode ForNode;
typedef struct BlockNode BlockNode;
typedef struct AssignNode AssignNode;
typedef struct PrintNode PrintNode;
typedef struct ArrayNode ArrayNode;
typedef union NodeExtend NodeExtend;
typedef struct Node Node;

struct NodeList {
    NodeList *next;
    Node *node;
};

struct BinaryNode {
    Node *lhs, *rhs;
    Operator op;
};

struct UnaryNode {
    Node *child;
    Operator op;
};

struct IfNode {
    Node *condition, *true_action, *false_action;
};

struct ForNode {
    Node *init, *stop, *after, *action;
};

struct BlockNode {
    NodeList *head, *cur_node;
};

struct ArrayNode {
    NodeList *head, *cur_node;
};

struct AssignNode {
    char *name;
    Node *value;
};

union NodeExtend{
    BinaryNode *binary_node;
    UnaryNode *unary_node;
    IfNode *if_node;
    ForNode *for_node;
    BlockNode *block_node;
    AssignNode *assign_node;
    ArrayNode *array_node;
    // const and ident
    int number_node;
    char *str_node;
    char *ident_node;
    // built in function
    Node *print_node;
    Node *len_node;
};

struct Node {
    ASTType type;
    NodeExtend node;
};


/* function */

// node constructor
Node *new_binary_node(Node *lhs, Node *rhs, Operator op);
Node *new_unary_node(Node *child, Operator op);
Node *new_str_node(char *str);
Node *new_number_node(int number);
Node *new_if_node(Node *condition, Node *true_action, Node *false_action);
Node *new_for_node(Node *init, Node *stop, Node *after, Node *action);
Node *new_block_node();
Node *new_assign_node(char *name, Node *value);
Node *new_ident_node(char *name);
Node *new_print_node(Node *child);
Node *new_len_node(Node *child);
Node *new_array_node();

// block node function
void push_node(Node *cur_block_node, Node *cur_node);

// array node function
void push_array_node(Node *cur_array_node, Node *cur_node);


#endif