#include "interpreter.h"

// CONTINUE FLAG
int JUMP_STMT = 0;


void run(char *file_name);
Object *run_node(Node *cur_node);
Object *run_block_node(Node *cur_node);
Object *run_binary_node(Node *cur_node);
Object *run_if_node(Node *cur_node);
Object *run_ident_node(Node *cur_node);
Object *run_assign_node(Node *cur_node);
Object *run_for_node(Node *cur_node);
Object *run_array_node(Node *cur_node);
Object *run_unary_node(Node *cur_node);
Object *run_array_assign_node(Node *cur_node);
// built_in function
Object *run_print_node(Node *cur_node);
Object *run_len_node(Node *cur_node);
Object *run_append_node(Node *cur_node);

void run(char *file_name) {
    yyin = fopen(file_name, "r");
    yyparse();
    fclose(yyin);
    run_node(tree);
}

Object *run_node(Node *cur_node) {

    if(cur_node == NULL) return NULL;

    ASTType cur_type = cur_node->type;
    switch(cur_type) {
        case binaryNode:
            return run_binary_node(cur_node);
        case unaryNode:
            return run_unary_node(cur_node);
        case strNode:
            return new_str(cur_node->node.str_node);
        case numberNode:
            return new_number(cur_node->node.number_node);
        case identNode:
            return run_ident_node(cur_node);
        case ifNode:
            return run_if_node(cur_node);
        case forNode:
            return run_for_node(cur_node);
        case blockNode:
            return run_block_node(cur_node);
        case assignNode:
            return run_assign_node(cur_node);
        case arrayNode:
            return run_array_node(cur_node);
        /* built_int function */
        case printNode:
            return run_print_node(cur_node);
        case lenNode:
            return run_len_node(cur_node);
        case appendNode:
            return run_append_node(cur_node);
        default:
            printf("undefined node: %d\n", cur_type);
            return NULL;
    }
}

Object *run_block_node(Node *cur_node) {
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
    return NULL;
}

/* TODO: finish idx type checking */
Object *run_array_get_node(Node *cur_node, bool is_reference) {
    Object *left_value = run_node(cur_node->node.binary_node->lhs);
    Object *right_value = run_node(cur_node->node.binary_node->rhs);
    return array_get(left_value, right_value->value.number, is_reference);
}

Object *run_binary_node(Node *cur_node) {

    Operator op = cur_node->node.binary_node->op;
    // getArray is a special case
    if(op == getArray) {
        return run_array_get_node(cur_node, false);
    }

    Object *left_value = run_node(cur_node->node.binary_node->lhs);
    Object *right_value = run_node(cur_node->node.binary_node->rhs);
    switch(op) {
        case add:
            return obj_add(left_value, right_value);
        case sub:
            return obj_sub(left_value, right_value);
        case mul:
            return obj_mul(left_value, right_value);
        case mod:
            return obj_mod(left_value, right_value);
        case div_:
            return obj_div(left_value, right_value);
        case and:
            return obj_and(left_value, right_value);
        case or:
            return obj_or(left_value, right_value);
        case xor:
            return obj_xor(left_value, right_value);
        case logic_or:
            return obj_logic_or(left_value, right_value);
        case logic_and:
            return obj_logic_and(left_value, right_value);
        case shl:
            return obj_shl(left_value, right_value);
        case shr:
            return obj_shr(left_value, right_value);
        case eq:
            return obj_eq(left_value, right_value);
        case ne:
            return obj_ne(left_value, right_value);
        case lt:
            return obj_lt(left_value, right_value);
        case gt:
            return obj_lt(right_value, left_value);
        case le:
            return obj_le(left_value, right_value);
        case ge:
            return obj_le(right_value, left_value);
        default:
            printf("unexpected binary node: %d\n", op);
            return NULL;
    }
}

Object *run_if_node(Node *cur_node) {
    Object *cond = run_node(cur_node->node.if_node->condition);

    if(obj_is_true(cond)) {
        run_node(cur_node->node.if_node->true_action);
    } else {
        run_node(cur_node->node.if_node->false_action);
    }
    return NULL;
}

Object *run_ident_node(Node *cur_node) {
    char *symbol_name = cur_node->node.ident_node;
    Symbol *cur_ident = find_symbol(symbol_name);

    if(cur_ident == NULL) {
        printf("undefined symbol: %s\n", symbol_name);
        return NULL;
    }
    /*
        number: value
        str: value
        array: reference
    */
    return copy_obj(cur_ident->value);
}

Object *run_assign_node(Node *cur_node) {
    Node *ident_node = cur_node->node.assign_node->ident;
    // ident assign
    if(ident_node->type == identNode) {
        char *name = ident_node->node.ident_node;
        Object *value = run_node(cur_node->node.assign_node->value);
        if(value == NULL) {
            printf("assign null value\n");
            exit(1);
        }

        upsert_symbol(name, value);
        return NULL;
    // array assign
    } else if(ident_node->type == binaryNode && ident_node->node.binary_node->op == getArray) {

        Object *cur_array = run_node(ident_node->node.binary_node->lhs);
        Object *idx = run_node(ident_node->node.binary_node->rhs);

        Object *value = run_node(cur_node->node.assign_node->value);
        if(value == NULL || idx == NULL) {
            printf("assign null value\n");
            exit(1);
        }
        obj_array_assign(cur_array, idx, value);

        return NULL;
    }
    printf("assign error\n");
    return NULL;
}

Object *run_for_node(Node *cur_node) {

    push_symbol_table();

    // run init
    run_node(cur_node->node.for_node->init);

    Object *cond = run_node(cur_node->node.for_node->stop);

    while(obj_is_true(cond)) {
        run_node(cur_node->node.for_node->action);
        if(JUMP_STMT == BREAK_FLAG) {
            JUMP_STMT = 0;
            break;
        } else if(JUMP_STMT == CONTINUE_FLAG) {
            JUMP_STMT = 0;
        }
        run_node(cur_node->node.for_node->after);

        cond = run_node(cur_node->node.for_node->stop);
    }
    pop_symbol_table();
    return NULL;
}

Object *run_array_node(Node *cur_node) {
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
    return result;
}

Object *run_print_node(Node *cur_node) {

    Object *result = run_node(cur_node->node.print_node);
    obj_print(result);
    return NULL;
}

Object *run_len_node(Node *cur_node) {
    Object *value = run_node(cur_node->node.len_node);
    return obj_len(value);
}

Object *run_append_node(Node *cur_node) {

    char *name = cur_node->node.append_node->name;
    Object *value = run_node(cur_node->node.append_node->value);
    Symbol *cur_symbol = find_symbol(name);
    if(value == NULL) {
        printf("assign null value\n");
        exit(1);
    } else if(cur_symbol == NULL) {
        printf("undefined symbol: %s\n", name);
        exit(1);
    }
    array_push(cur_symbol->value, value);
    return NULL;
}

Object *run_unary_node(Node *cur_node) {
    Operator op = cur_node->node.unary_node->op;
    switch(op) {
        case input_: {
            char buffer[1024] = {0};
            fgets(buffer, 1024, stdin);
            // handle new line
            buffer[strcspn(buffer, "\r\n")] = 0;
            return new_str(buffer);
        }
        case toInt: {
            Object *tmp = run_node(cur_node->node.unary_node->child);
            return obj_to_int(tmp);
        }
        case continue_:
            JUMP_STMT = CONTINUE_FLAG;
            break;
        case break_:
            JUMP_STMT = BREAK_FLAG;
            break;
        case not: {
            Object *tmp = run_node(cur_node->node.unary_node->child);
            return obj_not(tmp);
        }
        case bit_not: {
            Object *tmp = run_node(cur_node->node.unary_node->child);
            return obj_bit_not(tmp);
        }
        default:
            printf("unexpected unary node: %d\n", op);
            break;
    }
    return NULL;
}
