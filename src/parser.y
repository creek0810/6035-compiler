%{
#include <stdio.h>
#include "node.h"
extern int yylex();
extern FILE *yyin;
extern int yylineno;
Node *tree;
void yyerror(const char* msg) {
    printf("error: %s %d\n",msg, yylineno);
}
%}

%union {
    char *str;
    double decimal;
    long long int num;
    Node *node;
}

// keyword token
%token IF ELSE FOR CONTINUE BREAK
// built in function
%token PRINT APPEND LEN INPUT INT STR KW_DOUBLE
// punc token
%token SHL SHR LE GE EQ NE LOGIC_AND LOGIC_OR SEMI L_CURLY R_CURLY L_PARA R_PARA L_BRACKET R_BRACKET OR AND XOR ADD SUB MUL DIV MOD LT GT TILDE EXCLAM COMMA ASSIGN
// const token
%token <decimal> DOUBLE
%token <num> NUMBER
%token <str> STRING
// ident
%token <str> IDENT

/* non-terminal type */
%type <node> program
%type <node> statement_list
%type <node> statement
%type <node> expression_statement
%type <node> selection_statement
%type <node> iteration_statement
%type <node> compound_statement
%type <node> jump_statement
%type <node> expression
%type <node> opt_expression
%type <node> logical_or_expression
%type <node> logical_and_expression
%type <node> inclusive_or_expression
%type <node> exclusive_or_expression
%type <node> and_expression
%type <node> equality_expression
%type <node> relational_expression
%type <node> shift_expression
%type <node> multiplicative_expression
%type <node> additive_expression
%type <node> unary_expression
%type <node> postfix_expression
%type <node> primary_expression
%type <node> array


%right ASSIGN

// config
%start program


%%
program: { $$ = NULL; tree = $$;}
       | statement_list { $$ = $1; tree = $1;}
       ;


statement_list: { $$ = new_block_node(); }
              | statement_list statement {
                push_node($$, $2);
              }
              ;


/* statement */
statement: expression_statement { $$ = $1; }
         | compound_statement { $$ = $1; }
         | selection_statement { $$ = $1; }
         | iteration_statement { $$ = $1; }
         | jump_statement { $$ = $1; }
         ;




expression_statement: SEMI { $$ = NULL; }
                    | expression SEMI { $$ = $1; }
                    ;

compound_statement: L_CURLY statement_list R_CURLY {
                    $$ = $2;
                  }
                  ;


selection_statement: IF L_PARA expression R_PARA statement {
                        $$ = new_if_node($3, $5, NULL);
                   }
                   | IF L_PARA expression R_PARA statement ELSE statement {
                        $$ = new_if_node($3, $5, $7);
                   }
                   ;

jump_statement: CONTINUE SEMI { $$ = new_unary_node(NULL, continue_); }
              | BREAK SEMI { $$ = new_unary_node(NULL, break_); }
              ;


iteration_statement:
    FOR L_PARA opt_expression SEMI opt_expression SEMI opt_expression R_PARA statement {
        $$ = new_for_node($3, $5, $7, $9);
    }
    ;

opt_expression: { $$ = NULL; }
              | expression { $$ = $1; }
              ;


/* expression */
expression: logical_or_expression { $$ = $1; }
          | postfix_expression ASSIGN expression {
              $$ = new_assign_node($1, $3);
          }
          ;

logical_or_expression: logical_and_expression { $$ = $1; }
                     | logical_or_expression LOGIC_OR logical_and_expression {
                         $$ = new_binary_node($1, $3, logicOr);
                     }
                     ;

logical_and_expression: inclusive_or_expression { $$ = $1; }
                      | logical_and_expression LOGIC_AND inclusive_or_expression {
                         $$ = new_binary_node($1, $3, logicAnd);
                      }
                      ;

inclusive_or_expression: exclusive_or_expression { $$ = $1; }
                       | inclusive_or_expression OR exclusive_or_expression {
                         $$ = new_binary_node($1, $3, or);
                       }
                       ;

exclusive_or_expression: and_expression { $$ = $1; }
                       | exclusive_or_expression XOR and_expression {
                         $$ = new_binary_node($1, $3, xor);
                       }
                       ;

and_expression: equality_expression { $$ = $1; }
              | and_expression AND equality_expression {
                $$ = new_binary_node($1, $3, and);
              }
              ;

equality_expression: relational_expression { $$ = $1; }
                   | equality_expression EQ relational_expression {
                        $$ = new_binary_node($1, $3, eq);
                   }
                   | equality_expression NE relational_expression {
                        $$ = new_binary_node($1, $3, ne);
                   }
                   ;

relational_expression: shift_expression { $$ = $1; }
                     | relational_expression LT shift_expression {
                        $$ = new_binary_node($1, $3, lt);
                     }
                     | relational_expression GT shift_expression {
                        $$ = new_binary_node($1, $3, gt);
                     }
                     | relational_expression LE shift_expression {
                        $$ = new_binary_node($1, $3, le);
                     }
                     | relational_expression GE shift_expression {
                        $$ = new_binary_node($1, $3, ge);
                     }
                     ;

shift_expression: additive_expression { $$ = $1; }
                | shift_expression SHL additive_expression {
                    $$ = new_binary_node($1, $3, shl);
                }
                | shift_expression SHR additive_expression {
                    $$ = new_binary_node($1, $3, shr);
                }
                ;

additive_expression: multiplicative_expression { $$ = $1; }
                   | additive_expression ADD multiplicative_expression {
                        $$ = new_binary_node($1, $3, add);
                   }
                   | additive_expression SUB multiplicative_expression {
                        $$ = new_binary_node($1, $3, sub);
                   }
                   ;

multiplicative_expression: unary_expression { $$ = $1; }
                         | multiplicative_expression MUL unary_expression {
                            $$ = new_binary_node($1, $3, mul);
                         }
                         | multiplicative_expression DIV unary_expression {
                            $$ = new_binary_node($1, $3, div_);
                         }
                         | multiplicative_expression MOD unary_expression {
                            $$ = new_binary_node($1, $3, mod);
                         }
                         ;

unary_expression: postfix_expression { $$ = $1; }
                | ADD unary_expression {
                    $$ = new_unary_node($2, add);
                }
                | SUB unary_expression {
                    $$ = new_unary_node($2, sub);
                }
                | TILDE unary_expression {
                    $$ = new_unary_node($2, bitNot);
                }
                | EXCLAM unary_expression {
                    $$ = new_unary_node($2, not);
                }
                ;

postfix_expression: primary_expression { $$ = $1; }
                  | postfix_expression L_BRACKET expression R_BRACKET {
                      $$ = new_binary_node($1, $3, getArray);
                  }
                  | PRINT L_PARA expression R_PARA {
                      $$ = new_unary_node($3, print);
                  }
                  | LEN L_PARA expression R_PARA {
                      $$ = new_unary_node($3, len);
                  }
                  | INPUT L_PARA R_PARA {
                      $$ = new_unary_node(NULL, input_);
                  }
                  | INT L_PARA expression R_PARA {
                      $$ = new_unary_node($3, toInt);
                  }
                  | STR L_PARA expression R_PARA {
                      $$ = new_unary_node($3, toStr);
                  }
                  | KW_DOUBLE L_PARA expression R_PARA {
                      $$ = new_unary_node($3, toDouble);
                  }
                  | APPEND L_PARA IDENT COMMA expression R_PARA {
                      $$ = new_append_node($3, $5);
                  }
                  ;

primary_expression: IDENT { $$ = new_ident_node($1); }
                  | NUMBER { $$ = new_number_node($1); }
                  | DOUBLE { $$ = new_double_node($1); }
                  | L_PARA expression R_PARA { $$ = $2; }
                  | STRING { $$ = new_str_node($1); }
                  | L_BRACKET array R_BRACKET { $$ = $2; }
                  ;

array: { $$ = new_array_node(); }
     | STRING  { $$ = new_array_node(); push_array_node($$, new_str_node($1)); }
     | NUMBER { $$ = new_array_node(); push_array_node($$, new_number_node($1)); }
     | DOUBLE { $$ = new_array_node(); push_array_node($$, new_double_node($1)); }
     | array COMMA STRING { push_array_node($$, new_str_node($3)); }
     | array COMMA NUMBER { push_array_node($$, new_number_node($3)); }
     | array COMMA DOUBLE { push_array_node($$, new_double_node($3)); }
     ;

%%

void print_node(Node *cur_node, int depth);

void print_operator(int op) {
    switch(op) {
        case add:
            printf("add\n");
            break;
        case sub:
            printf("sub\n");
            break;
        case mod:
            printf("mod\n");
            break;
        case div_:
            printf("div_\n");
            break;
        case and:
            printf("and\n");
            break;
        case or:
            printf("or\n");
            break;
        case xor:
            printf("xor\n");
            break;
        case logicOr:
            printf("logicOr\n");
            break;
        case logicAnd:
            printf("logicAnd\n");
            break;
        case shl:
            printf("shl\n");
            break;
        case shr:
            printf("shr\n");
            break;
        case eq:
            printf("eq\n");
            break;
        case ne:
            printf("ne\n");
            break;
        case lt:
            printf("lt\n");
            break;
        case gt:
            printf("gt\n");
            break;
        case le:
            printf("le\n");
            break;
        case ge:
            printf("ge\n");
            break;
        case not:
            printf("not\n");
            break;
        case bitNot:
            printf("bitNot\n");
            break;
        case mul:
            printf("mul\n");
            break;
        case break_:
            printf("break\n");
            break;
        case continue_:
            printf("continue\n");
            break;
        case getArray:
            printf("getArray\n");
            break;
        case input_:
            printf("input\n");
            break;
        case print:
            printf("print\n");
            break;
        case len:
            printf("len\n");
            break;
        /* type convert */
        case toInt:
            printf("toInt\n");
            break;
        case toDouble:
            printf("toDouble\n");
            break;
        case toStr:
            printf("toStr\n");
            break;
        default:
            printf("undefined %d\n", op);
    }
}

void print_binary_node(Node *cur_node, int depth) {
    if(cur_node == NULL) return;

    printf("%*sbinary node:\n", depth * 4, " ");
    printf("%*soperator: ", (depth + 1) * 4, " ");

    print_operator(cur_node->node.binary_node->op);
    // print lhs
    printf("%*slhs: \n", (depth + 1) * 4, " ");
    print_node(cur_node->node.binary_node->lhs, depth + 2);
    // print rhs
    printf("%*srhs: \n", (depth + 1) * 4, " ");
    print_node(cur_node->node.binary_node->rhs, depth + 2);
}

void print_unary_node(Node *cur_node, int depth) {
    if(cur_node == NULL) return;

    printf("%*sunary node:\n", depth * 4, " ");
    printf("%*soperator: ", (depth + 1) * 4, " ");

    print_operator(cur_node->node.unary_node->op);
    // print child
    printf("%*schild: \n", (depth + 1) * 4, " ");
    print_node(cur_node->node.unary_node->child, depth + 2);
}

void print_if_node(Node *cur_node, int depth) {
    if(cur_node == NULL) return;
    printf("%*sif node:\n", depth * 4, " ");
    // print condition
    printf("%*scondition: \n", (depth + 1) * 4, " ");
    print_node(cur_node->node.if_node->condition, depth + 2);
    // print true action
    printf("%*strue: \n", (depth + 1) * 4, " ");
    print_node(cur_node->node.if_node->true_action, depth + 2);
    // print false action
    printf("%*sfalse: \n", (depth + 1) * 4, " ");
    print_node(cur_node->node.if_node->false_action, (depth + 2));
}

void print_for_node(Node *cur_node, int depth) {
    if(cur_node == NULL) return;
    printf("%*sfor node:\n", depth * 4, " ");
    // print init
    printf("%*sinit: \n", (depth + 1) * 4, " ");
    print_node(cur_node->node.for_node->init, depth + 2);
    // print stop
    printf("%*sstop: \n", (depth + 1) * 4, " ");
    print_node(cur_node->node.for_node->stop, depth + 2);
    // print after
    printf("%*safter: \n", (depth + 1) * 4, " ");
    print_node(cur_node->node.for_node->after, (depth + 2));
    // print action
    printf("%*saction: \n", (depth + 1) * 4, " ");
    print_node(cur_node->node.for_node->action, (depth + 2));

}

void print_block_node(Node *cur_node, int depth) {
    printf("%*sblock node:\n", depth * 4, " ");

    NodeList *cur_ptr = cur_node->node.block_node->head;

    while(cur_ptr) {
        if(cur_ptr->node) {
            print_node(cur_ptr->node, depth + 1);
        }
        cur_ptr = cur_ptr->next;
    }
}

void print_array_node(Node *cur_node, int depth) {
    printf("%*sarray node:\n", depth * 4, " ");

    NodeList *cur_ptr = cur_node->node.array_node->head;

    while(cur_ptr) {
        if(cur_ptr->node) {
            print_node(cur_ptr->node, depth + 1);
        }
        cur_ptr = cur_ptr->next;
    }
}

void print_assign_node(Node *cur_node, int depth) {
    printf("%*sassign node:\n", depth * 4, " ");
    // print name
    printf("%*sident:\n", (depth + 1) * 4, " ");
    print_node(cur_node->node.assign_node->ident, depth + 2);
    // print value
    printf("%*svalue:\n", (depth + 1) * 4, " ");
    print_node(cur_node->node.assign_node->value, depth + 2);
}

void print_append_node(Node *cur_node, int depth) {
    printf("%*sappend node:\n", depth * 4, " ");
    // print name
    printf("%*sname: %s\n", (depth + 1) * 4, " ", cur_node->node.append_node->name);
    // print value
    printf("%*svalue:\n", (depth + 1) * 4, " ");
    print_node(cur_node->node.append_node->value, depth + 2);
}


void print_node(Node *cur_node, int depth) {
    if(cur_node == NULL) return;

    switch(cur_node->type) {
        case binaryNode:
            print_binary_node(cur_node, depth);
            break;
        case unaryNode:
            print_unary_node(cur_node, depth);
            break;
        case identNode:
            printf("%*sident: %s\n", depth * 4 , " ", cur_node->node.ident_node);
            break;
        case ifNode:
            print_if_node(cur_node, depth);
            break;
        case forNode:
            print_for_node(cur_node, depth);
            break;
        case blockNode:
            print_block_node(cur_node, depth);
            break;
        case assignNode:
            print_assign_node(cur_node, depth);
            break;
        case appendNode:
            print_append_node(cur_node, depth);
            break;
        case arrayNode:
            print_array_node(cur_node, depth);
            break;
        // const node
        case strNode:
            printf("%*sstring:%s\n", depth * 4 , " ", cur_node->node.str_node);
            break;
        case numberNode:
            printf("%*snumber: %lld\n", depth * 4 , " ", cur_node->node.number_node);
            break;
        case doubleNode:
            printf("%*sdouble: %lf\n", depth * 4 , " ", cur_node->node.double_node);
            break;
        default:
            printf("unexpected node type: %d\n", cur_node->type);
    }
}

void print_tree(char *file_name) {
    yyin = fopen(file_name, "r");
    yyparse();
    if(tree) {
        print_node(tree, 0);
    }
    fclose(yyin);
}
