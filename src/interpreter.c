#include "interpreter.h"

// CONTINUE FLAG
int JUMP_STMT = 0;


void run(char *file_name);
void run_node(Node *cur_node);
void run_block_node(Node *cur_node);
void run_binary_node(Node *cur_node);
void run_if_node(Node *cur_node);
void run_ident_node(Node *cur_node);
void run_assign_node(Node *cur_node);
void run_for_node(Node *cur_node);
void run_array_node(Node *cur_node);
void run_unary_node(Node *cur_node);
void run_array_assign_node(Node *cur_node);
// built_in function
void run_print_node(Node *cur_node);
void run_len_node(Node *cur_node);
void run_append_node(Node *cur_node);

void run(char *file_name) {
    yyin = fopen(file_name, "r");
    yyparse();
    fclose(yyin);
    run_node(tree);
}

void run_node(Node *cur_node) {

    if(cur_node == NULL) return;

    ASTType cur_type = cur_node->type;
    switch(cur_type) {
        case binaryNode:
            run_binary_node(cur_node);
            break;
        case unaryNode:
            run_unary_node(cur_node);
            break;
        case strNode:
            push(new_str(cur_node->node.str_node), false);
            break;
        case numberNode:
            push(new_number(cur_node->node.number_node), false);
            break;
        case identNode:
            run_ident_node(cur_node);
            break;
        case ifNode:
            run_if_node(cur_node);
            break;
        case forNode:
            run_for_node(cur_node);
            break;
        case blockNode:
            run_block_node(cur_node);
            break;
        case assignNode:
            run_assign_node(cur_node);
            break;
        case arrayNode:
            run_array_node(cur_node);
            break;
        /* built_int function */
        case appendNode:
            run_append_node(cur_node);
            break;
        default:
            printf("undefined node: %d\n", cur_type);
            return;
    }
}

void run_block_node(Node *cur_node) {
    push_symbol_table();
    NodeList *cur_ptr = cur_node->node.block_node->head;
    while(cur_ptr) {
        if(cur_ptr->node) {
            run_node(cur_ptr->node);
            if(JUMP_STMT == BREAK_FLAG || JUMP_STMT == CONTINUE_FLAG) {
                break;
            }
        }
        cur_ptr = cur_ptr->next;
    }
    pop_symbol_table();
    return;
}

void run_binary_node(Node *cur_node) {
    // run left and right
    run_node(cur_node->node.binary_node->lhs);
    run_node(cur_node->node.binary_node->rhs);
    // calc result
    Operator op = cur_node->node.binary_node->op;
    stack_biop(op);
}

void run_if_node(Node *cur_node) {
    run_node(cur_node->node.if_node->condition);

    if(stack_is_true()) {
        run_node(cur_node->node.if_node->true_action);
    } else {
        run_node(cur_node->node.if_node->false_action);
    }
}

void run_ident_node(Node *cur_node) {
    char *symbol_name = cur_node->node.ident_node;
    Symbol *cur_ident = find_symbol(symbol_name);

    if(cur_ident == NULL) {
        printf("undefined symbol: %s\n", symbol_name);
        return;
    }
    if(cur_ident->value->type == array) {
        push(cur_ident->value, true);
    } else {
        push(copy_obj(cur_ident->value), false);
    }
}

void run_assign_node(Node *cur_node) {

    Node *ident_node = cur_node->node.assign_node->ident;
    // ident assign
    if(ident_node->type == identNode) {

        run_node(cur_node->node.assign_node->value);

        char *name = ident_node->node.ident_node;
        stack_assign(name);
        return;
    // array assign
    } else if(ident_node->type == binaryNode && ident_node->node.binary_node->op == getArray) {
        run_node(ident_node);
        run_node(cur_node->node.assign_node->value);
        stack_array_assign();
        return;
    }
    printf("assign error\n");
}

void run_for_node(Node *cur_node) {

    push_symbol_table();

    // run init
    run_node(cur_node->node.for_node->init);

    run_node(cur_node->node.for_node->stop);

    while(stack_is_true()) {
        run_node(cur_node->node.for_node->action);
        if(JUMP_STMT == BREAK_FLAG) {
            JUMP_STMT = 0;
            break;
        } else if(JUMP_STMT == CONTINUE_FLAG) {
            JUMP_STMT = 0;
        }
        run_node(cur_node->node.for_node->after);

        run_node(cur_node->node.for_node->stop);
    }
    pop_symbol_table();
    return;
}

void run_array_node(Node *cur_node) {
    Object *result = new_array();
    NodeList *cur_ptr = cur_node->node.array_node->head;
    while(cur_ptr) {
        ASTType cur_type = cur_ptr->node->type;
        Object *push_obj = NULL;
        switch(cur_type) {
            case numberNode:
                push_obj = new_number(cur_ptr->node->node.number_node);
                break;
            case strNode:
                push_obj = new_str(cur_ptr->node->node.str_node);
                break;
            default:
                printf("unexpected type in array: %d\n", cur_type);
                exit(1);
        }
        array_push(result, push_obj);
        cur_ptr = cur_ptr->next;
    }
    push(result, true);
}

void run_append_node(Node *cur_node) {
    run_node(cur_node->node.append_node->value);

    char *name = cur_node->node.append_node->name;
    stack_array_append(name);
}

void run_unary_node(Node *cur_node) {
    Operator op = cur_node->node.unary_node->op;
    switch(op) {
        // input
        case input_: {
            char buffer[2048] = {0};
            fgets(buffer, 2048, stdin);
            // handle new line
            buffer[strcspn(buffer, "\r\n")] = 0;
            push(new_str(buffer), false);
            break;
        }
        // jump
        case continue_:
            JUMP_STMT = CONTINUE_FLAG;
            break;
        case break_:
            JUMP_STMT = BREAK_FLAG;
            break;
        case toInt:
        case toStr:
        case not:
        case bit_not:
        case print:
        case len:
            run_node(cur_node->node.unary_node->child);
            stack_unop(op);
            break;
        default:
            printf("unexpected unary node: %d\n", op);
            break;
    }
}
