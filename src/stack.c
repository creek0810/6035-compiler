#include "stack.h"

StackNode *STACK = NULL;

void push(Object *obj, bool is_reference) {
    /*
        Object *value;
        StackNode *prev;
        bool is_reference;
    */
    if(obj) {
        StackNode *new_node = malloc(sizeof(StackNode));
        new_node->value = obj;
        new_node->prev = STACK;
        new_node->is_reference = is_reference;
        STACK = new_node;
    }
}

StackNode *pop() {
    StackNode *popped_node = STACK;
    STACK = STACK->prev;
    return popped_node;
}

void free_stack_node(StackNode *cur_node) {
    if(!(cur_node->is_reference)) {
        free_obj(cur_node->value);
    }
    free(cur_node);
}

void stack_biop(Operator op) {

    StackNode *right = pop();
    StackNode *left = pop();

    Object *right_value = right->value;
    Object *left_value = left->value;

    Object *result = NULL;
    bool is_reference = false;
    switch(op) {
        case getArray:
            // array: reference
            // str: value
            if(left_value->type == array) {
                is_reference = true;
            }
            result = array_get(left_value, right_value);
            break;
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
            printf("Unexpected binary node: %d\n", op);
            return;
    }

    free_stack_node(right);
    free_stack_node(left);
    push(result, is_reference);
}

void stack_unop(Operator op) {
    StackNode *tmp_node = pop();
    Object *tmp_value = tmp_node->value;
    Object *result = NULL;
    switch(op) {
        case toDouble:
            result = obj_to_double(tmp_value);
            break;
        case toInt:
            result = obj_to_int(tmp_value);
            break;
        case toStr:
            result = obj_to_str(tmp_value);
            break;
        case not:
            result = obj_not(tmp_value);
            break;
        case bit_not:
            result = obj_bit_not(tmp_value);
            break;
        case len:
            result = obj_len(tmp_value);
            break;
        case print:
            result = obj_print(tmp_value);
            break;
        default:
            printf("Unexpected unnary node: %d\n", op);
            return;

    }
    free_stack_node(tmp_node);
    push(result, false);
}

bool stack_is_true() {
    /* get top value and return if it is true.
        Used in for, if
    */
   StackNode *result = pop();
   bool return_value = obj_is_true(result->value);
   free_stack_node(result);
   return return_value;
}

void stack_array_append(char *name) {
    Symbol *cur_symbol = find_symbol(name);

    StackNode *cur_node = pop();
    Object *value = cur_node->value;

    if(value == NULL) {
        printf("assign null value\n");
        exit(1);
    } else if(cur_symbol == NULL) {
        printf("undefined symbol: %s\n", name);
        exit(1);
    }
    array_push(cur_symbol->value, value);
    free_stack_node(cur_node);
}

void stack_assign(char *name) {
    StackNode *cur_node = pop();
    Object *value = cur_node->value;

    upsert_symbol(name, value);
    free_stack_node(cur_node);
}

void stack_array_assign() {
    StackNode *value = pop();
    StackNode *var = pop();
    obj_assign(var->value, value->value);
    free_stack_node(var);
    free_stack_node(value);
}