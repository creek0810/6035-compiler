%{
#include <stdio.h>
#include "node.h"
extern int yylex();
extern FILE *yyin;
extern int yylineno;
Node *debug;
void yyerror(const char* msg) {
    printf("error: %s %d\n",msg, yylineno);
}
%}

%union {
    char ch;
    char *str;
    int num;
    Node *node;
}

// keyword token
%token IF ELSE FOR CONTINUE BREAK INT CHAR
// punc token
%token SHL SHR ADD_EQ SUB_EQ DIV_EQ MOD_EQ MUL_EQ SHL_EQ SHR_EQ AND_EQ XOR_EQ OR_EQ LE GE EQ NE LOGIC_AND LOGIC_OR SPREAD PAST SEMI L_CURLY R_CURLY L_PARA R_PARA L_BRACKET R_BRACKET OR AND XOR ADD SUB MUL DIV MOD LT GT TILDE EXCLAM COMMA ASSIGN
// const token
%token NUMBER
%token <str> STR
%token <str> CHARACTER
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
%type <node> primary_expression


// config
%start program


%%
program: { $$ = NULL; debug = $$;}
       | statement_list { $$ = $1; debug = $1;}
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
/* TODO: support advanced assign operator */
expression: logical_or_expression { $$ = $1; }
          | IDENT ASSIGN expression { $$ = new_assign_node($1, $3); }
          | IDENT MUL_EQ expression { }
          | IDENT DIV_EQ expression {}
          | IDENT MOD_EQ expression {}
          | IDENT ADD_EQ expression {}
          | IDENT SUB_EQ expression {}
          | IDENT SHL_EQ expression {}
          | IDENT SHR_EQ expression {}
          | IDENT AND_EQ expression {}
          | IDENT XOR_EQ expression {}
          | IDENT OR_EQ expression {}
          ;

logical_or_expression: logical_and_expression { $$ = $1; }
                     | logical_or_expression LOGIC_OR logical_and_expression {
                         $$ = new_binary_node($1, $3, logic_or);
                     }
                     ;

logical_and_expression: inclusive_or_expression { $$ = $1; }
                      | logical_and_expression LOGIC_AND inclusive_or_expression {
                         $$ = new_binary_node($1, $3, logic_and);
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

unary_expression: primary_expression { $$ = $1; }
                | ADD unary_expression {
                    $$ = new_unary_node($2, add);
                }
                | SUB unary_expression {
                    $$ = new_unary_node($2, sub);
                }
                | TILDE unary_expression {
                    $$ = new_unary_node($2, bit_not);
                }
                | EXCLAM unary_expression {
                    $$ = new_unary_node($2, not);
                }
                ;

primary_expression: IDENT { $$ = new_ident_node(yylval.str); }
                  | NUMBER { $$ = new_number_node(yylval.num); }
                  | CHARACTER { $$ = new_char_node(yylval.ch); }
                  | L_PARA expression R_PARA { $$ = $2; }
                  ;
%%

void print_node(Node *cur_node, int depth);

void print_operator(int op) {
    switch(op) {
        case 0:
            printf("add\n");
            break;
        case 1:
            printf("sub\n");
            break;
        case 2:
            printf("mod\n");
            break;
        case 3:
            printf("div_\n");
            break;
        case 4:
            printf("and\n");
            break;
        case 5:
            printf("or\n");
            break;
        case 6:
            printf("xor\n");
            break;
        case 7:
            printf("logic_or\n");
            break;
        case 8:
            printf("logic_and\n");
            break;
        case 9:
            printf("shl\n");
            break;
        case 10:
            printf("shr\n");
            break;
        case 11:
            printf("eq\n");
            break;
        case 12:
            printf("ne\n");
            break;
        case 13:
            printf("lt\n");
            break;
        case 14:
            printf("gt\n");
            break;
        case 15:
            printf("le\n");
            break;
        case 16:
            printf("ge\n");
            break;
        case 17:
            printf("not\n");
            break;
        case 18:
            printf("bit_not\n");
            break;
        case 19:
            printf("mul\n");
            break;
        case 20:
            printf("break\n");
            break;
        case 21:
            printf("continue\n");
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

void print_assign_node(Node *cur_node, int depth) {
    printf("%*sassign node:\n", depth * 4, " ");
    // print name
    printf("%*sname: %s\n", (depth + 1) * 4, " ", cur_node->node.assign_node->name);
    // print value
    printf("%*svalue:\n", (depth + 1) * 4, " ");
    print_node(cur_node->node.assign_node->value, depth + 2);
}

void print_node(Node *cur_node, int depth) {
    if(cur_node == NULL) return;

    switch(cur_node->type) {
        // binary node
        case 0:
            print_binary_node(cur_node, depth);
            break;
        // unary node
        case 1:
            print_unary_node(cur_node, depth);
            break;
        // str node
        case 2:
            printf("%*s%s\n", depth * 4 , " ", cur_node->node.str_node);
            break;
        // char node
        case 3:
            printf("%*s%c\n", depth * 4 , " ", cur_node->node.char_node);
            break;
        // number node
        case 4:
            printf("%*s%d\n", depth * 4 , " ", cur_node->node.number_node);
            break;
        // ident node
        case 5:
            printf("%*sident: %s\n", depth * 4 , " ", cur_node->node.ident_node);
            break;
        // if node
        case 6:
            print_if_node(cur_node, depth);
            break;
        // for node
        case 7:
            print_for_node(cur_node, depth);
            break;
        // block node
        case 8:
            print_block_node(cur_node, depth);
            break;
        // assign node
        case 9:
            print_assign_node(cur_node, depth);
            break;
        default:
            printf("unexpected node type: %d\n", cur_node->type);
    }
}

void print_tree(char *file_name) {
    yyin = fopen(file_name, "r");
    yyparse();
    if(debug) {
        print_node(debug, 0);
    }
    fclose(yyin);
}

