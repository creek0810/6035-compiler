#ifndef _OBJECT_H
#define _OBJECT_H

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

/* define object value struct */
typedef struct Str Str;
typedef struct Array Array;
typedef union Value Value;

typedef enum ObjectType ObjectType;
typedef struct Object Object;

struct Str {
    char *value;
    int length;
};

struct Array {
    Object **array;
    int capacity;
};

union Value {
    int number;
    Str str;
    Array array;
};

/* define object */
enum ObjectType  {
    number = 0,
    string = 1,
    array = 2,
};

struct Object {
    ObjectType type;
    Value value;
};

/* define function */

// construct function
Object *new_array();
Object *new_number(int num);
Object *new_str(char *str);

// destruct function
void free_obj(Object *a);

// copy function
Object *copy_obj(Object *a);

// array function
void array_push(Object *a, Object *b);
Object *array_get(Object *a, int idx, bool is_reference);

// operation function
Object *obj_add(Object *a, Object *b);
Object *obj_sub(Object *a, Object *b);
Object *obj_mul(Object *a, Object *b);
Object *obj_mod(Object *a, Object *b);
Object *obj_div(Object *a, Object *b);
Object *obj_and(Object *a, Object *b);
Object *obj_or(Object *a, Object *b);
Object *obj_xor(Object *a, Object *b);
Object *obj_logic_or(Object *a, Object *b);
Object *obj_logic_and(Object *a, Object *b);
Object *obj_shl(Object *a, Object *b);
Object *obj_shr(Object *a, Object *b);
Object *obj_eq(Object *a, Object *b);
Object *obj_ne(Object *a, Object *b);
Object *obj_lt(Object *a, Object *b); // lt, gt
Object *obj_le(Object *a, Object *b); // le, ge
Object *obj_not(Object *a);
Object *obj_bit_not(Object *a);
Object *obj_len(Object *a);
Object *obj_to_int(Object *a);
Object *obj_assign(Object *a, Object *b);
bool obj_is_true(Object *a);
void obj_print(Object *a);

void print_object(Object *a);
#endif