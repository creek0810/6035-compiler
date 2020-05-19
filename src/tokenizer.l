%{
    #include <stdio.h>
    #include "node.h"
    #include "parser.h"

    char *clean_str(char *ori);
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
STR \"{CHARACTER}*\"

    /* config */
%option yylineno
%x COMMENT

%%

    /* keyword */
"if"                        { return IF; }
"else"                      { return ELSE; }
"continue"                  { return CONTINUE; }
"break"                     { return BREAK; }
"for"                       { return FOR; }
    /* built in function */
"print"                     { return PRINT; }
"len"                       { return LEN; }
"append"                    { return APPEND; }
"input"                     { return INPUT; }
"int"                       { return INT; }

    /* const */
{DEC_CONST}                 { yylval.num = atoi(yytext); return NUMBER; }
{STR}                       { yylval.str = clean_str(yytext); return STR; }

    /* ident */
{ID}                        { yylval.str = strdup(yytext); return IDENT; }

    /* multiple letters punc */
"<<"                        { return SHL; }
">>"                        { return SHR; }
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
"//".*                      {}

"/*"                        { BEGIN(COMMENT); }
<COMMENT>"*/"               { BEGIN(INITIAL); }
<COMMENT>.                  {}
<COMMENT>\n                 {}

[\n\t ]                     {}
.                           { return -1; }

%%
int yywrap() {
    return 1;
}

char *clean_str(char *ori) {
    int len = strlen(ori) - 2 + 1; // sub " "(2 char) add \0(1 char)
    char *result = calloc(1, sizeof(char) * len);
    strncpy(result, ori + 1, len - 1);
    return result;
}

void print_token(char *file_name) {
    yyin = fopen(file_name, "r");
    int token;
    while(token = yylex()) {
        if(token == IDENT) {
            printf("ident: %s\n", yytext);
        } else if(token == NUMBER) {
            printf("number: %s\n", yytext);
        } else if(token >= IF && token <= APPEND) {
            printf("keyword: %s\n", yytext);
        } else if(token >= SHL && token <= ASSIGN) {
            printf("punc: %s\n", yytext);
        } else if(token == STR) {
            printf("str: %s\n", yytext);
        } else if(token != -1) {
            printf("punc: %s\n", yytext);
        } else {
            printf("unexpected: %s\n", yytext);
        }
    }
    fclose(yyin);
}


