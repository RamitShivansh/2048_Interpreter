%{
    // Definitions
    #include "interpreter.h"
    int yylex();
%}


%%

"ADD" {
    yylval.key = 1;
    return ADD;
}
"SUBTRACT" {
    yylval.key = 2;
    return SUBTRACT;
}
"MULTIPLY" {
    yylval.key = 3;
    return MULTIPLY;
}
"DIVIDE" {
    yylval.key = 4;
    return DIVIDE;
}
"LEFT" {
    yylval.key = 1;
    return LEFT;
}
"RIGHT" {
    yylval.key = 2;
    return RIGHT;
}
"UP" {
    yylval.key = 3;
    return UP;
}
"DOWN" {
    yylval.key = 4;
    return DOWN;
}
"ASSIGN" {
    yylval.key = ASSIGN;
    return ASSIGN;
}
"TO" {
    yylval.key = TO;
    return TO;
}
"VAR" {
    yylval.key = VAR;
    return VAR;
}
"IS" {
    yylval.key = IS;
    return IS;
}
"VALUE" {
    yylval.key = VALUE;
    return VALUE;
}
"IN" {
    yylval.key = IN;
    return IN;
}
"," {
    yylval.key = COMMA;
    return COMMA;
}
"." {
    yylval.key = EOL;
    return EOL;
}

[-][0-9]+|[0-9]+ {
    yylval.key = atoi(yytext); 
    return NUMBER;
}

[A-Za-z][_0-9A-Za-z]* { 
    bzero(yylval.name, 2 * MAX_VAR_LEN);
    strcpy(yylval.name, yytext);
    return VARNAME;
}
[0-9][0-9A-Za-z_]+ {
    yylval.key = ILLEGAL_VARNAME;
    return ILLEGAL_VARNAME;
}
[ \t]+ {}
[\n] { return NEWLINE; }
. { return INVALID_TOKEN; }

%%

int yywrap() { return 1; }