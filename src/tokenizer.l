%{
    #include <stdio.h>
    #include "node.h"
    #include "parser.h"

%}

    /* ident */
NONDIGIT [a-zA-Z_]
DIGIT [0-9]
ID {NONDIGIT}({DIGIT}|{NONDIGIT})*

    /* constant */
DEC_CONST {DIGIT}+
ESC "\\v"|"\\n"|"\\'"|"\\\\"|"\\\""|"\\?"|"\\a"|"\\b"|"\\f"|"\\r"|"\\t"

    /* str */
CHARACTER ([^\n"\\]|{ESC})
CHAR \'{CHARACTER}\'
STR \"{CHARACTER}*\"

    /* config */
%option yylineno

%%

    /* keyword */
"if"                        { return IF; }
"else"                      { return ELSE; }
"continue"                  { return CONTINUE; }
"break"                     { return BREAK; }
"for"                       { return FOR; }
"int"                       { return INT; }
"char"                      { return CHAR; }

    /* const */
{DEC_CONST}                 { yylval.num = atoi(yytext); return NUMBER; }
{STR}                       { yylval.str = strdup(yytext); return STR; }
{CHAR}                      { yylval.str = strdup(yytext); return CHARACTER; }

    /* ident */
{ID}                        { yylval.str = strdup(yytext); return IDENT; }

    /* multiple letters punc */
"<<"                        { return SHL; }
">>"                        { return SHR; }
"+="                        { return ADD_EQ; }
"-="                        { return SUB_EQ; }
"/="                        { return DIV_EQ; }
"%="                        { return MOD_EQ; }
"*="                        { return MUL_EQ; }
"<<="                       { return SHL_EQ; }
">>="                       { return SHR_EQ; }
"&="                        { return AND_EQ; }
"^="                        { return XOR_EQ; }
"|="                        { return OR_EQ; }
"<="                        { return LE; }
">="                        { return GE; }
"=="                        { return EQ; }
"!="                        { return NE; }
"&&"                        { return LOGIC_AND; }
"||"                        { return LOGIC_OR; }

    /* simple letter punc */
"["                         { return L_BRACKET; }
"]"                         { return R_BRACKET; }
"{"                         { return L_CURLY; }
"}"                         { return R_CURLY; }
"("                         { return L_PARA; }
")"                         { return R_PARA; }
";"                         { return SEMI; }
"|"                         { return OR; }
"&"                         { return AND; }
"^"                         { return XOR; }
"+"                         { return ADD; }
"-"                         { return SUB; }
"*"                         { return MUL; }
"/"                         { return DIV; }
"%"                         { return MOD; }
">"                         { return GT; }
"<"                         { return LT; }
"~"                         { return TILDE; }
"!"                         { return EXCLAM; }
","                         { return COMMA; }
"="                         { return ASSIGN; }

    /* another */
[\n\t ]                      {}
. { return -1; }

%%
int yywrap() {
    return 1;
}

void print_token(char *file_name) {
    yyin = fopen(file_name, "r");
    int token;
    while(token = yylex()) {
        if(token == IDENT) {
            printf("ident: %s\n", yytext);
        } else if(token == NUMBER) {
            printf("number: %s\n", yytext);
        } else if(token >= IF && token <= CHAR) {
            printf("keyword: %s\n", yytext);
        } else if(token >= SHL && token <= ASSIGN) {
            printf("punc: %s\n", yytext);
        } else if(token == STR) {
            printf("str: %s\n", yytext);
        } else if(token == CHARACTER) {
            printf("char: %s\n", yytext);
        } else if(token != -1) {
            printf("punc: %s\n", yytext);
        } else {
            printf("unexpected: %s\n", yytext);
        }
    }
    fclose(yyin);
}


