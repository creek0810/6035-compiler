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
// TODO: print array info
void print_type(Object *obj) {
    switch(obj->type) {
        case number:
            printf("\'number\'");
            break;
        case string:
            printf("\'str\'");
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

bool is_same_type(Object *a, Object *b) { return a->type == b->type;
}

// TODO: finish this function
void clear_object(Object *obj) {

}

/* update function */
Object *update_number(Object *obj, int num) {
    if(obj->type != number) {
        clear_object(obj);
    }
    obj->type = number;
    obj->value.number = num;
    return obj;
}

Object *update_str(Object *obj, char *str) {
    if(obj->type != string) {
        clear_object(obj);
    }
    obj->type = string;
    obj->value.str.value = strdup(str);
    obj->value.str.length = strlen(str);
    return obj;
}

/* array function */
void push_obj(Object *array, Object *content) {
    Object **cur_array = array->value.array.array;
    int new_capacity = array->value.array.capacity + 1;
    // init memory
    if(cur_array == NULL) {
        Object **new_array = calloc(1, sizeof(Object*) * new_capacity);
        // update value
        array->value.array.array = new_array;
        array->value.array.capacity = new_capacity;
        new_array[new_capacity - 1] = content;
    } else {
        Object **new_array = realloc(cur_array, sizeof(Object*) * new_capacity);
        // update value
        array->value.array.array = new_array;
        array->value.array.capacity = new_capacity;
        new_array[new_capacity - 1] = content;
    }
}

void push_array_number(Object *a, int num) {
    if(a->type != array) {
        printf("append failed: variable is not a list\n");
        return;
    }
    push_obj(a, new_number(num));
}

void push_array_str(Object *a, char *str) {
    if(a->type != array) {
        printf("append failed: variable is not a list\n");
        return;
    }
    push_obj(a, new_str(str));
}

Object *array_get(Object *a, int idx) {
    switch(a->type) {
        case array:
            return a->value.array.array[idx];
        case string: {
            char ch[2] = {0};
            ch[0] = a->value.str.value[idx];
            return new_str(ch);
        }
        default:
            printf("type error: type %d can't use idx\n", a->type);
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
            free(tmp);
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
    int result = is_true(a) || is_true(b);
    return new_number(result);
}

Object *obj_logic_and(Object *a, Object *b) {
    int result = is_true(a) && is_true(b);
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
        case string:
        // TODO: support string
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
        case string:
        // TODO: support string
        default:
            print_op_error("<=", a, b);
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

/* another function */
bool is_true(Object *obj) {
    if(obj == NULL) return false;
    if(obj->type == number && obj->value.number == 0) return false;
    return true;
}

Object *to_int(Object *obj) {
    switch(obj->type) {
        case number:
            return obj;
        case array:
            printf("array can't be convert to int");
            return NULL;
        case string: {
            int result = 0;
            for(int i=0; i<obj->value.str.length; i++) {
                result = result * 10 + (obj->value.str.value[i] - '0');
            }
            return new_number(result);
        }
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