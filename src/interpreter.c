#include "interpreter.h"



// CONTINUE FLAG
int JUMP_STMT = 0;



void run(char *file_name);
Object *run_node(Node *cur_node, bool need_return);
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
    run_node(tree, false);
}

Object *run_node(Node *cur_node, bool need_return) {

    if(cur_node == NULL) return NULL;

    ASTType cur_type = cur_node->type;
    Object *result = NULL;
    switch(cur_type) {
        case binaryNode:
            result = run_binary_node(cur_node);
            break;
        case unaryNode:
            result = run_unary_node(cur_node);
            break;
        case strNode:
            result = new_str(cur_node->node.str_node);
            break;
        case numberNode:
            result = new_number(cur_node->node.number_node);
            break;
        case identNode:
            result = run_ident_node(cur_node);
            break;
        case ifNode:
            result = run_if_node(cur_node);
            break;
        case forNode:
            result = run_for_node(cur_node);
            break;
        case blockNode:
            result = run_block_node(cur_node);
            break;
        case assignNode:
            result = run_assign_node(cur_node);
            break;
        case arrayNode:
            result = run_array_node(cur_node);
            break;
        /* built_int function */
        case printNode:
            result = run_print_node(cur_node);
            break;
        case lenNode:
            result = run_len_node(cur_node);
            break;
        case appendNode:
            result = run_append_node(cur_node);
            break;
        default:
            printf("undefined node: %d\n", cur_type);
            return NULL;
    }

    if(need_return) {
        return result;
    } else {
        pool_delete_obj(result);
        return NULL;
    }
}

Object *run_block_node(Node *cur_node) {
    push_symbol_table();
    NodeList *cur_ptr = cur_node->node.block_node->head;
    while(cur_ptr) {
        if(cur_ptr->node) {
            run_node(cur_ptr->node, false);
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
    Object *left_value = run_node(cur_node->node.binary_node->lhs, true);
    Object *right_value = run_node(cur_node->node.binary_node->rhs, true);
    Object *result = NULL;
    result = array_get(left_value, right_value->value.number, is_reference);
    pool_delete_obj(left_value);
    pool_delete_obj(right_value);
    return result;
}

Object *run_binary_node(Node *cur_node) {

    Operator op = cur_node->node.binary_node->op;
    // getArray is a special case
    if(op == getArray) {
        return run_array_get_node(cur_node, false);
    }

    Object *left_value = run_node(cur_node->node.binary_node->lhs, true);
    Object *right_value = run_node(cur_node->node.binary_node->rhs, true);
    Object *result = NULL;
    switch(op) {
        case add:
            result = obj_add(left_value, right_value);
            break;
        case sub:
            result = obj_sub(left_value, right_value);
            break;
        case mul:
            result = obj_mul(left_value, right_value);
            break;
        case mod:
            result = obj_mod(left_value, right_value);
            break;
        case div_:
            result = obj_div(left_value, right_value);
            break;
        case and:
            result = obj_and(left_value, right_value);
            break;
        case or:
            result = obj_or(left_value, right_value);
            break;
        case xor:
            result = obj_xor(left_value, right_value);
            break;
        case logic_or:
            result = obj_logic_or(left_value, right_value);
            break;
        case logic_and:
            result = obj_logic_and(left_value, right_value);
            break;
        case shl:
            result = obj_shl(left_value, right_value);
            break;
        case shr:
            result = obj_shr(left_value, right_value);
            break;
        case eq:
            result = obj_eq(left_value, right_value);
            break;
        case ne:
            result = obj_ne(left_value, right_value);
            break;
        case lt:
            result = obj_lt(left_value, right_value);
            break;
        case gt:
            result = obj_lt(right_value, left_value);
            break;
        case le:
            result = obj_le(left_value, right_value);
            break;
        case ge:
            result = obj_le(right_value, left_value);
            break;
        default:
            printf("unexpected binary node: %d\n", op);
            return NULL;
    }
    pool_delete_obj(left_value);
    pool_delete_obj(right_value);

    return result;
}

Object *run_if_node(Node *cur_node) {
    Object *cond = run_node(cur_node->node.if_node->condition, true);

    if(obj_is_true(cond)) {
        run_node(cur_node->node.if_node->true_action, false);
    } else {
        run_node(cur_node->node.if_node->false_action, false);
    }
    pool_delete_obj(cond);
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
        Object *value = run_node(cur_node->node.assign_node->value, true);
        if(value == NULL) {
            printf("assign null value\n");
            exit(1);
        }

        upsert_symbol(name, value);
        pool_delete_obj(value);
        return NULL;
    // array assign
    } else if(ident_node->type == binaryNode && ident_node->node.binary_node->op == getArray) {

        Object *cur_array = run_node(ident_node->node.binary_node->lhs, true);
        Object *idx = run_node(ident_node->node.binary_node->rhs, true);

        Object *value = run_node(cur_node->node.assign_node->value, true);
        if(value == NULL || idx == NULL) {
            printf("assign null value\n");
            exit(1);
        }
        obj_array_assign(cur_array, idx, value);
        pool_delete_obj(cur_array);
        pool_delete_obj(idx);
        pool_delete_obj(value);

        return NULL;
    }
    printf("assign error\n");
    return NULL;
}

Object *run_for_node(Node *cur_node) {

    push_symbol_table();

    // run init
    run_node(cur_node->node.for_node->init, false);


    Object *cond = run_node(cur_node->node.for_node->stop, true);

    while(obj_is_true(cond)) {
        run_node(cur_node->node.for_node->action, false);
        if(JUMP_STMT == BREAK_FLAG) {
            JUMP_STMT = 0;
            break;
        } else if(JUMP_STMT == CONTINUE_FLAG) {
            JUMP_STMT = 0;
        }
        run_node(cur_node->node.for_node->after, false);

        pool_delete_obj(cond);
        cond = run_node(cur_node->node.for_node->stop, true);
    }
    pool_delete_obj(cond);
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
        pool_delete_obj(push_obj);
        cur_ptr = cur_ptr->next;
    }
    return result;
}

Object *run_print_node(Node *cur_node) {

    Object *result = run_node(cur_node->node.print_node, true);
    obj_print(result);

    pool_delete_obj(result);
    return NULL;
}

Object *run_len_node(Node *cur_node) {
    Object *value = run_node(cur_node->node.len_node, true);
    Object *result = obj_len(value);
    pool_delete_obj(value);
    return result;
}

Object *run_append_node(Node *cur_node) {

    char *name = cur_node->node.append_node->name;
    Object *value = run_node(cur_node->node.append_node->value, true);
    Symbol *cur_symbol = find_symbol(name);
    if(value == NULL) {
        printf("assign null value\n");
        exit(1);
    } else if(cur_symbol == NULL) {
        printf("undefined symbol: %s\n", name);
        exit(1);
    }
    array_push(cur_symbol->value, value);
    pool_delete_obj(value);
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
            Object *tmp = run_node(cur_node->node.unary_node->child, true);
            Object *result = obj_to_int(tmp);
            pool_delete_obj(tmp);
            return result;
        }
        case continue_:
            JUMP_STMT = CONTINUE_FLAG;
            break;
        case break_:
            JUMP_STMT = BREAK_FLAG;
            break;
        case not: {
            Object *tmp = run_node(cur_node->node.unary_node->child, true);
            Object *result = obj_not(tmp);
            pool_delete_obj(tmp);
            return result;
        }
        case bit_not: {
            Object *tmp = run_node(cur_node->node.unary_node->child, true);
            Object *result = obj_bit_not(tmp);
            pool_delete_obj(tmp);
            return result;
        }
        default:
            printf("unexpected unary node: %d\n", op);
            break;
    }
    return NULL;
}
