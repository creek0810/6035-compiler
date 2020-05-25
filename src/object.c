#include "object.h"

/* construct function */
Object *new_array() {
    Object *cur_obj = calloc(1, sizeof(Object));
    cur_obj->type = array;
    return cur_obj;
}

Object *new_number(int num) {
    Object *cur_obj = calloc(1, sizeof(Object));
    cur_obj->type = number;
    cur_obj->value.number = num;
    return cur_obj;
}

Object *new_str(char *str) {
    Object *cur_obj = calloc(1, sizeof(Object));
    cur_obj->type = string;
    cur_obj->value.str.value = strdup(str);
    cur_obj->value.str.length = strlen(str);
    return cur_obj;
}

/* help function */
void print_type(Object *obj) {
    switch(obj->type) {
        case number:
            printf("\'number\'");
            break;
        case string:
            printf("\'str\'");
            break;
        case array:
            printf("\'array\'");
            break;
        default:
            printf("unexpected type: %d\n", obj->type);
    }
}

void print_op_error(char *op, Object *a, Object *b) {
    printf("TypeError: unsupported operand type(s) for %s: ", op);
    print_type(a);
    printf(" and ");
    print_type(b);
    printf("\n");
    exit(1);
}

void print_function_error(char *func_name, Object *a) {
    printf("TypeError: object of type ");
    print_type(a);
    printf("has no %s()\n", func_name);
}

bool is_same_type(Object *a, Object *b) {
    return a->type == b->type;
}

Object *copy_obj(Object *a) {
    /*
        number: value
        str: value
        array: reference
    */
    switch(a->type) {
        case number:
            return new_number(a->value.number);
        case string:
            return new_str(a->value.str.value);
        case array:
            return a;
        default:
            printf("unexpected type of ident: %d\n", a->type);
            return NULL;
    }
}

void clear_obj(Object *a) {
    if(a == NULL) return;

    switch(a->type) {
        case number:
            break;
        case string:
            free(a->value.str.value);
            break;
        case array: {
            for(int i=0; i<a->value.array.capacity; i++) {
                free_obj(a->value.array.array[i]);
            }
            free(a->value.array.array);
        }
            break;
    }
}

void free_obj(Object *a) {
    if(a == NULL) return;
    clear_obj(a);
    free(a);
}

/* array function */
void array_push(Object *a, Object *b) {
    if(a->type != array) {
        printf("append failed: variable is not a list\n");
        return;
    }

    Object **cur_array = a->value.array.array;
    int new_capacity = a->value.array.capacity + 1;
    // init memory
    if(cur_array == NULL) {
        Object **new_array = calloc(1, sizeof(Object*) * new_capacity);
        // update value
        a->value.array.array = new_array;
        a->value.array.capacity = new_capacity;
        new_array[new_capacity - 1] = copy_obj(b);
    } else {
        Object **new_array = realloc(cur_array, sizeof(Object*) * new_capacity);
        // update value
        a->value.array.array = new_array;
        a->value.array.capacity = new_capacity;
        new_array[new_capacity - 1] = copy_obj(b);
    }
}

Object *array_get(Object *a, int idx, bool is_reference) {

    switch(a->type) {
        case array:
            if(is_reference) {
                return a->value.array.array[idx];
            } else {
                return copy_obj(a->value.array.array[idx]);
            }
        case string: {
            char ch[2] = {0};
            ch[0] = a->value.str.value[idx];
            return new_str(ch);
        }
        default:
            printf("type error: type %d can't use idx\n", a->type);
            return NULL;
    }
}

/* operation function */
Object *obj_add(Object *a, Object *b) {

    if(!is_same_type(a, b)) {
        print_op_error("+", a, b);
        return NULL;
    }
    switch(a->type) {
        case number:
            return new_number(a->value.number + b->value.number);
        case string: {
            // construct concate result
            int new_len = a->value.str.length + b->value.str.length + 1;
            char *tmp = calloc(1, sizeof(char) * new_len);
            strcpy(tmp, a->value.str.value);
            strcat(tmp, b->value.str.value);
            // use concate result to construct string object
            Object *result = new_str(tmp);
            return result;
        }
        default:
            print_op_error("+", a, b);
            return NULL;
    }
}

Object *obj_sub(Object *a, Object *b) {
    if(!is_same_type(a, b)) {
        print_op_error("-", a, b);
        return NULL;
    }
    switch(a->type) {
        case number:
            return new_number(a->value.number - b->value.number);
        default:
            print_op_error("-", a, b);
            return NULL;
    }
}

Object *obj_mul(Object *a, Object *b) {
    if(!is_same_type(a, b)) {
        print_op_error("*", a, b);
        return NULL;
    }
    switch(a->type) {
        case number:
            return new_number(a->value.number * b->value.number);
        case string:
        // TODO: support string mul
        default:
            print_op_error("*", a, b);
            return NULL;
    }
}

Object *obj_mod(Object *a, Object *b) {
    if(!is_same_type(a, b)) {
        print_op_error("%", a, b);
        return NULL;
    }
    switch(a->type) {
        case number:
            return new_number(a->value.number % b->value.number);
        default:
            print_op_error("%", a, b);
            return NULL;
    }
}

Object *obj_div(Object *a, Object *b) {
    if(!is_same_type(a, b)) {
        print_op_error("/", a, b);
        return NULL;
    }
    switch(a->type) {
        case number:
            return new_number(a->value.number / b->value.number);
        default:
            print_op_error("/", a, b);
            return NULL;
    }
}

Object *obj_and(Object *a, Object *b) {
    if(!is_same_type(a, b)) {
        print_op_error("&", a, b);
        return NULL;
    }
    switch(a->type) {
        case number:
            return new_number(a->value.number & b->value.number);
        default:
            print_op_error("&", a, b);
            return NULL;
    }
}

Object *obj_or(Object *a, Object *b) {
    if(!is_same_type(a, b)) {
        print_op_error("|", a, b);
        return NULL;
    }
    switch(a->type) {
        case number:
            return new_number(a->value.number | b->value.number);
        default:
            print_op_error("|", a, b);
            return NULL;
    }
}

Object *obj_xor(Object *a, Object *b) {
    if(!is_same_type(a, b)) {
        print_op_error("^", a, b);
        return NULL;
    }
    switch(a->type) {
        case number:
            return new_number(a->value.number ^ b->value.number);
        default:
            print_op_error("^", a, b);
            return NULL;
    }
}

Object *obj_logic_or(Object *a, Object *b) {
    int result = obj_is_true(a) || obj_is_true(b);
    return new_number(result);
}

Object *obj_logic_and(Object *a, Object *b) {
    int result = obj_is_true(a) && obj_is_true(b);
    return new_number(result);
}

Object *obj_shl(Object *a, Object *b) {
    if(!is_same_type(a, b)) {
        print_op_error("<<", a, b);
        return NULL;
    }
    switch(a->type) {
        case number:
            return new_number(a->value.number << b->value.number);
        default:
            print_op_error("<<", a, b);
            return NULL;
    }
}

Object *obj_shr(Object *a, Object *b) {
    if(!is_same_type(a, b)) {
        print_op_error(">>", a, b);
        return NULL;
    }
    switch(a->type) {
        case number:
            return new_number(a->value.number >> b->value.number);
        default:
            print_op_error(">>", a, b);
            return NULL;
    }
}

Object *obj_eq(Object *a, Object *b) {
    if(!is_same_type(a, b)) {
        print_op_error("==", a, b);
        return NULL;
    }
    switch(a->type) {
        case number:
            return new_number(a->value.number == b->value.number);
        case string: {
            int result = (strcmp(a->value.str.value, b->value.str.value) == 0);
            return new_number(result);
        }
        case array:
        // TODO: support array cmp
        default:
            print_op_error("==", a, b);
            return NULL;
    }
}

Object *obj_ne(Object *a, Object *b) {
    if(!is_same_type(a, b)) {
        print_op_error("!=", a, b);
        return NULL;
    }
    switch(a->type) {
        case number:
            return new_number(a->value.number != b->value.number);
        case string: {
            int result = (strcmp(a->value.str.value, b->value.str.value) != 0);
            return new_number(result);
        }
        // TODO: support array cmp
        case array:
        default:
            print_op_error("!=", a, b);
            return NULL;
    }
}

Object *obj_lt(Object *a, Object *b) {
    if(!is_same_type(a, b)) {
        print_op_error("<", a, b);
        return NULL;
    }
    switch(a->type) {
        case number:
            return new_number(a->value.number < b->value.number);
        case string: {
            if(strcmp(a->value.str.value, b->value.str.value) < 0) {
                return new_number(1);
            } else {
                return new_number(0);
            }
        }
        default:
            print_op_error("<", a, b);
            return NULL;
    }
}

Object *obj_le(Object *a, Object *b) {
    if(!is_same_type(a, b)) {
        print_op_error("<=", a, b);
        return NULL;
    }
    switch(a->type) {
        case number:
            return new_number(a->value.number <= b->value.number);
        case string: {
            if(strcmp(a->value.str.value, b->value.str.value) <= 0) {
                return new_number(1);
            } else {
                return new_number(0);
            }
        }
        default:
            print_op_error("<=", a, b);
            return NULL;
    }
}

Object *obj_not(Object *a) {
    return obj_is_true(a) ? new_number(0) : new_number(1);
}

Object *obj_bit_not(Object *a) {
    switch(a->type) {
        case number:
            return new_number(~(a->value.number));
        default:
            print_op_error("~", a, NULL);
            return NULL;
    }
}

Object *obj_len(Object *a) {
    switch(a->type) {
        case string:
            return new_number(a->value.str.length);
        case array:
            return new_number(a->value.array.capacity);
        default:
            print_function_error("len", a);
            return new_number(-1);
    }
}

Object *obj_to_int(Object *a) {
    switch(a->type) {
        case number:
            return a;
        case array:
            printf("array can't be convert to int");
            return NULL;
        case string: {
            int result = 0;
            for(int i=0; i<a->value.str.length; i++) {
                result = result * 10 + (a->value.str.value[i] - '0');
            }
            return new_number(result);
        }
    }
}

bool obj_is_true(Object *a) {
    if(a == NULL) return false;
    if(a->type == number && a->value.number == 0) return false;
    return true;
}

void obj_print(Object *a) {
    if(a == NULL) {
        printf("NULL\n");
        return;
    }

    switch (a->type) {
        case number:
            printf("%d\n", a->value.number);
            break;
        case string:
            printf("%s\n", a->value.str.value);
            break;
        case array: {
            printf("[");
            Object **cur_array = a->value.array.array;
            for(int i=0; i<a->value.array.capacity; i++) {
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
            printf("print unexpected type object: %d\n", a->type);
    }
}

Object *obj_assign(Object *a, Object *b) {
    if(a->type != array && b->type != array) {
        clear_obj(a);
        // copy value
        a->type = b->type;
        if(b->type == number) {
            a->value.number = b->value.number;
        } else {
            a->value.str.value = strdup(b->value.str.value);
            a->value.str.length = b->value.str.length;
        }
        return a;
    } else {
        return copy_obj(b);
    }
    return NULL;
}

void obj_array_assign(Object *arr, Object *idx, Object *value) {
    if(arr->type != array || idx->type != number || value->type == array) {
        printf("TypeError: array assign type error\n");
        return;
    }
    if(arr->value.array.capacity <= idx->value.number) {
        printf("IndexError: index out of array");
        return;
    }
    Object *el = arr->value.array.array[idx->value.number];
    clear_obj(el);
    // copy value
    el->type = value->type;
    if(value->type == number) {
        el->value.number = value->value.number;
    } else {
        el->value.str.value = strdup(value->value.str.value);
        el->value.str.length = value->value.str.length;
    }
}

/* debug function */
void print_object(Object *obj) {
    switch (obj->type) {
        case number:
            printf("number: %d\n", obj->value.number);
            break;
        case string:
            printf("str: %s\n", obj->value.str.value);
            break;
        case array: {
            printf("array:\n");
            printf("[");
            Object **cur_array = obj->value.array.array;
            for(int i=0; i<obj->value.array.capacity; i++) {
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
            printf("undefined object type: %d\n", obj->type);
            return;
    }
}