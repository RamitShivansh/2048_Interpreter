%{
    /* Definitions */
    #include "interpreter.h"

    void yyerror(char *s);

    Board *board;
    Var_Table *vartable;
%}

%union {
    int key;
    char name[1024];
}

%start prog

%type <key> line move val_asgn var_asgn val_access opn dir val err_st keyword all
%token <key> ADD SUBTRACT MULTIPLY DIVIDE LEFT RIGHT UP DOWN ASSIGN TO VAR IS VALUE IN EOL NUMBER COMMA ILLEGAL_VARNAME NEWLINE INVALID_TOKEN
%token <name> VARNAME

%%
    prog: line prog | line;

    line: move | val_asgn | var_asgn | val_access | err_st | NEWLINE { dec(1); };

    move:   opn dir EOL NEWLINE { make_move(board, vartable, $2, $1); }
    |       opn dir NEWLINE { eol_err(); }
    |       opn VARNAME EOL NEWLINE { move_err(1, $1, 1, $2); }
    |       opn VARNAME NEWLINE { eol_err(); }
    |       opn EOL NEWLINE { move_err(69, 0,0, ""); }
    |       opn NEWLINE { eol_err(); }
    |       dir opn EOL NEWLINE { move_err(3, $2, $1, ""); }
    |       dir opn NEWLINE { eol_err(); }
    ;

    opn: ADD | SUBTRACT | MULTIPLY | DIVIDE;
    dir: LEFT | RIGHT | UP | DOWN;

    val_asgn:   ASSIGN val TO NUMBER COMMA NUMBER EOL NEWLINE { assign(board, vartable, $2, $4, $6); }
    |           ASSIGN val TO NUMBER COMMA NUMBER NEWLINE { eol_err(); }
    |           VARNAME val TO NUMBER COMMA NUMBER EOL NEWLINE { asgn_err(1, $2, $4, $6); }
    |           VARNAME val TO NUMBER COMMA NUMBER NEWLINE { eol_err(); }
    |           ASSIGN val keyword NUMBER COMMA NUMBER EOL NEWLINE { asgn_err(2, $2, $4, $6); }
    |           ASSIGN val keyword NUMBER COMMA NUMBER NEWLINE { eol_err(); }
    |           ASSIGN val VARNAME NUMBER COMMA NUMBER EOL NEWLINE { asgn_err(2, $2, $4, $6); }
    |           ASSIGN val VARNAME NUMBER COMMA NUMBER NEWLINE { eol_err(); }
    |           ASSIGN NUMBER COMMA NUMBER TO val EOL NEWLINE { asgn_err(2, $6, $2, $4); }
    |           ASSIGN NUMBER COMMA NUMBER TO val EOL { eol_err(); }
    ;

    val:    VALUE IN NUMBER COMMA NUMBER { $$ = query(board, $3, $5, 0); }
    |       NUMBER
    ;

    var_asgn:   VAR VARNAME IS NUMBER COMMA NUMBER EOL NEWLINE { make_var(vartable, board, $4, $6, $2); }
    |           VAR VARNAME IS NUMBER COMMA NUMBER NEWLINE { eol_err(); }
    |           VAR keyword IS NUMBER COMMA NUMBER EOL NEWLINE { var_asgn_err(1); }
    |           VAR keyword IS NUMBER COMMA NUMBER NEWLINE { eol_err(); }
    |           VAR all IS NUMBER COMMA NUMBER EOL NEWLINE { var_asgn_err(2); }
    |           VAR all IS NUMBER COMMA NUMBER NEWLINE { eol_err(); }
    ;

    val_access: VALUE IN NUMBER COMMA NUMBER EOL NEWLINE { query(board, $3, $5, 1); }
    |           VALUE IN NUMBER COMMA NUMBER NEWLINE { eol_err(); }

    err_st: all err_st
    |       all { gen_err(); }
    ;

    all: keyword | VARNAME { $$ = 0; } | ILLEGAL_VARNAME | INVALID_TOKEN { $$ = 0; };
    keyword: ADD | SUBTRACT | MULTIPLY | DIVIDE | LEFT | RIGHT | UP | DOWN | ASSIGN | TO | VAR | IS | VALUE | IN | EOL | NUMBER | COMMA;

%%

void yyerror(char *s) {
    /* dec(0);
    fflush(stdout);
    print_stderr(1);
    dec(1); */
    yyparse();
}

int main() {
    system("clear");
    print_init();
    srand(time(0));
    board = (Board *) malloc(N * sizeof(Board));
    vartable = (Var_Table *) malloc(sizeof(Var_Table));
    zero_board(board);
    init_vartable(vartable);
    dec(1);
    yyparse();
    return 0;
}