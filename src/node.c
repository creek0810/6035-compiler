#include "node.h"

Node *new_node(void *cur_node, ASTType type) {
    Node *result = calloc(1, sizeof(Node));
    result->type = type;
    switch(type) {
        case unaryNode:
            result->node.unary_node = cur_node;
            break;
        case binaryNode:
            result->node.binary_node = cur_node;
            break;
        case ifNode:
            result->node.if_node = cur_node;
            break;
        case forNode:
            result->node.for_node = cur_node;
            break;
        case assignNode:
            result->node.assign_node = cur_node;
            break;
        case blockNode:
            result->node.block_node = cur_node;
            break;
        case printNode:
            result->node.print_node = cur_node;
            break;
        case lenNode:
            result->node.print_node = cur_node;
            break;
        case strNode:
            result->node.str_node = strdup((char*)cur_node);
            break;
        case identNode:
            result->node.ident_node = strdup((char*)cur_node);
            break;
        case numberNode:
            result->node.number_node = *(int*)cur_node;
            break;
        case arrayNode:
            result->node.array_node = cur_node;
            break;
        default:
            printf("undefined node type: %d\n", type);
    }
    return result;
}

Node *new_assign_node(char *name, Node *value) {
    AssignNode *cur_node = calloc(1, sizeof(AssignNode));
    if(cur_node == NULL) {
        return NULL;
    }
    cur_node->name = strdup(name);
    cur_node->value = value;
    return new_node(cur_node, assignNode);
}

Node *new_binary_node(Node *lhs, Node *rhs, Operator op) {
    BinaryNode *cur_node = calloc(1, sizeof(BinaryNode));
    if(cur_node == NULL) {
        return NULL;
    }
    cur_node->lhs = lhs;
    cur_node->rhs = rhs;
    cur_node->op = op;
    return new_node(cur_node, binaryNode);
}

Node *new_unary_node(Node *child, Operator op) {
    UnaryNode *cur_node = calloc(1, sizeof(UnaryNode));
    if(cur_node == NULL) {
        return NULL;
    }
    cur_node->child = child;
    cur_node->op = op;
    return new_node(cur_node, unaryNode);
}

Node *new_if_node(Node *condition, Node *true_action, Node *false_action) {
    IfNode *cur_node = calloc(1, sizeof(IfNode));
    if(cur_node == NULL) {
        return NULL;
    }
    cur_node->condition = condition;
    cur_node->true_action = true_action;
    cur_node->false_action = false_action;
    return new_node(cur_node, ifNode);
}

Node *new_for_node(Node *init, Node *stop, Node *after, Node *action) {
    ForNode *cur_node = calloc(1, sizeof(ForNode));
    if(cur_node == NULL) {
        return NULL;
    }
    cur_node->init = init;
    cur_node->stop = stop;
    cur_node->after = after;
    cur_node->action = action;
    return new_node(cur_node, forNode);
}

Node *new_print_node(Node *child) {
    return new_node(child, printNode);
}

Node *new_len_node(Node *child) {
    return new_node(child, lenNode);
}

Node *new_str_node(char *str) {
    return new_node(str, strNode);
}

Node *new_ident_node(char *name) {
    return new_node(name, identNode);
}

Node *new_number_node(int number) {
    return new_node(&number, numberNode);
}

Node *new_block_node() {
    BlockNode *cur_node = calloc(1, sizeof(BlockNode));
    if(cur_node == NULL) {
        return NULL;
    }
    return new_node(cur_node, blockNode);
}

Node *new_array_node() {
    ArrayNode *cur_node = calloc(1, sizeof(ArrayNode));
    if(cur_node == NULL) {
        return NULL;
    }
    return new_node(cur_node, arrayNode);
}


// block node function
void push_node(Node *cur_block_node, Node *cur_node) {
    // construct new node list node
    NodeList *new_node_list_node = calloc(1, sizeof(NodeList));
    new_node_list_node->node = cur_node;
    if(cur_block_node->node.block_node->head == NULL) {
        cur_block_node->node.block_node->head = new_node_list_node;
        cur_block_node->node.block_node->cur_node= new_node_list_node;
    } else {
        cur_block_node->node.block_node->cur_node->next = new_node_list_node;
        cur_block_node->node.block_node->cur_node = new_node_list_node;
    }
}

// array node function
void push_array_node(Node *cur_array_node, Node *cur_node) {
    // construct new node list node
    NodeList *new_node_list_node = calloc(1, sizeof(NodeList));
    new_node_list_node->node = cur_node;
    if(cur_array_node->node.array_node->head == NULL) {
        cur_array_node->node.array_node->head = new_node_list_node;
        cur_array_node->node.array_node->cur_node= new_node_list_node;
    } else {
        cur_array_node->node.array_node->cur_node->next = new_node_list_node;
        cur_array_node->node.array_node->cur_node = new_node_list_node;
    }
}
