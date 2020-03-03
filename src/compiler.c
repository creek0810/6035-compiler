#include <string.h>
#include <stdbool.h>
#include "compiler.h"


bool str_eq(char *a, char *b) {
    return strcmp(a, b) == 0;
}

int main(int argc, char **argv) {
    // debug mode have 3 arg
    if(argc == 3) {
        // tokenizer debug
        if(str_eq(argv[1], "-t")) print_token(argv[2]);
        // parser debug
        else if(str_eq(argv[1], "-p")) print_tree(argv[2]);
    } else {
    // interpeter mode
    // TODO: finish interpreter part
    }
    return 0;
}




