#ifndef _NODE_H
#define _NODE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* enumerate */
typedef enum ASTType ASTType;
typedef enum Operator Operator;

enum ASTType{
    // general node
    binaryNode,
    unaryNode,
    forNode,
    blockNode,
    assignNode,
    ifNode,
    identNode,
    // const node
    arrayNode,
    doubleNode,
    strNode,
    numberNode,
    // built in function
    appendNode,
};

enum Operator {
    add,
    sub,
    mod,
    div_,
    and,
    or,
    xor,
    logicOr,
    logicAnd,
    shl,
    shr,
    eq,
    ne,
    lt,
    gt,
    le,
    ge,
    not,
    bitNot,
    mul,
    getArray,
    /* unary */
    input_,
    toInt,
    print,
    len,
    toStr,
    toDouble,
    break_,
    continue_,
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
typedef struct AppendNode AppendNode;
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
    Node *ident;
    Node *value;
};

struct AppendNode{
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
    double double_node;
    long long number_node;
    char *str_node;
    char *ident_node;
    // built in function
    AppendNode *append_node;
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
Node *new_number_node(long long number);
Node *new_double_node(double number);
Node *new_if_node(Node *condition, Node *true_action, Node *false_action);
Node *new_for_node(Node *init, Node *stop, Node *after, Node *action);
Node *new_block_node();
Node *new_assign_node(Node *ident, Node *value);
Node *new_ident_node(char *name);
Node *new_array_node();
Node *new_append_node(char *name, Node *value);

// block node function
void push_node(Node *cur_block_node, Node *cur_node);

// array node function
void push_array_node(Node *cur_array_node, Node *cur_node);


#endif