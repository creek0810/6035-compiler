#include "interpreter.h"

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
// built_in function
Object *run_print_node(Node *cur_node);
Object *run_len_node(Node *cur_node);

void run(char *file_name) {
    yyin = fopen(file_name, "r");
    yyparse();
    fclose(yyin);
    run_node(tree);
}

Object *run_node(Node *cur_node) {

    if(cur_node == NULL) return 0;

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
        }
        cur_ptr = cur_ptr->next;
    }
    pop_symbol_table();
    return NULL;
}

Object *run_binary_node(Node *cur_node) {
    Object *left_value = run_node(cur_node->node.binary_node->lhs);
    Object *right_value = run_node(cur_node->node.binary_node->rhs);

    Operator op = cur_node->node.binary_node->op;
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
        case getArray:
            //TODO: warning for idx should be number
            return array_get(left_value, right_value->value.number);
        default:
            printf("unexpected binary node: %d\n", op);
            return NULL;
    }
}

Object *run_if_node(Node *cur_node) {
    Object *cond = run_node(cur_node->node.if_node->condition);

    if(is_true(cond)) {
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
    return cur_ident->value;
}

Object *run_assign_node(Node *cur_node) {
    char *name = cur_node->node.assign_node->name;
    Object *value = run_node(cur_node->node.assign_node->value);
    // TODO: support array action
    // TODO: support a = b = c
    if(value == NULL) {
        printf("assign null value\n");
        exit(1);
    }

    switch (value->type) {
        case number:
            upsert_number_symbol(name, value->value.number);
            return NULL;
        case string:
            upsert_str_symbol(name, value->value.str.value);
            return NULL;
        case array:
            upsert_array_symbol(name, value);
            return NULL;
        default:
            printf("assign unexpected object type: %d\n", value->type);
            return NULL;
    }
}

Object *run_for_node(Node *cur_node) {
    push_symbol_table();

    // run init
    run_node(cur_node->node.for_node->init);

    // TODO: handle continue break
    while (is_true(run_node(cur_node->node.for_node->stop))) {
        run_node(cur_node->node.for_node->action);
        run_node(cur_node->node.for_node->after);
    }
    pop_symbol_table();
    return NULL;
}

Object *run_array_node(Node *cur_node) {
    Object *result = new_array();
    NodeList *cur_ptr = cur_node->node.array_node->head;
    while(cur_ptr) {
        ASTType cur_type = cur_ptr->node->type;
        switch(cur_type) {
            case numberNode:
                push_array_number(result, cur_ptr->node->node.number_node);
                break;
            case strNode:
                push_array_str(result, cur_ptr->node->node.str_node);
                break;
            default:
                printf("unexpected type in array: %d\n", cur_type);


        }
        cur_ptr = cur_ptr->next;
    }
    return result;
}

Object *run_print_node(Node *cur_node) {
    Object *result = run_node(cur_node->node.print_node);

    if(result == NULL) {
        printf("NULL\n");
        return NULL;
    }

    switch (result->type) {
        case number:
            printf("%d\n", result->value.number);
            break;
        case string:
            printf("%s\n", result->value.str.value);
            break;
        case array: {
            printf("[");
            Object **cur_array = result->value.array.array;
            for(int i=0; i<result->value.array.capacity; i++) {
                if(i) printf(", ");
                if(cur_array[i]->type == number) {
                    printf("%d", cur_array[i]->value.number);
                } else {
                    printf("\"%s\"", cur_array[i]->value.str.value);
                }
            }
            printf("]\n");
            break;
        }
        default:
            printf("print unexpected type object: %d\n", result->type);
    }

    return NULL;
}

Object *run_len_node(Node *cur_node) {
    Object *value = run_node(cur_node->node.len_node);
    if(value == NULL) {
        printf("undefined var\n");
        exit(1);
    }
    return obj_len(value);
}

Object *run_unary_node(Node *cur_node) {
    Operator op = cur_node->node.unary_node->op;
    switch(op) {
        case input_: {
            char buffer[1024] = {0};
            fgets(buffer, 1024, stdin);
            int len = strlen(buffer);
            // ignore \n
            buffer[len - 1] = 0;
            return new_str(buffer);
        }
        case toInt: {
            Object *tmp = run_node(cur_node->node.unary_node->child);
            return to_int(tmp);
        }

    }
    return NULL;
}
/* TODO: finish unary node */
/*
    not = 17,
    bit_not = 18,
    break_ = 20,
    continue_ = 21,
    assign = 22
*/
