#ifndef INTERPRETER_H
#define INTERPRETER_H

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<time.h>
#include "y.tab.h"

#define N 4
#define MAX_VAR 16
#define MAX_VAR_LEN 256

typedef struct {
    int tiles[N][N];
} Board;

typedef struct {
    char variables[N][N][MAX_VAR][MAX_VAR_LEN];
    int ind[N][N];
} Var_Table;

Board *board;
Var_Table *vartable;

int yylex();

// utility
void dec();
void print_error_bred();
void print_init();


// 2048
void zero_board(Board* board);
void print_board(Board *board);
void move_merge_tiles(Board *board, Var_Table *vartable, int *dir, int merge, int opn);
int do_operation(int opn, int x);
void make_move(Board *board, Var_Table *vartable, int direction, int opn);
void assign(Board *board, Var_Table *vartable, int val, int i, int j);
int query(Board *board, int i, int j, int print);
void random_tile(Board *board);

// variable storing
void init_vartable(Var_Table *vartable);
int check_valid_var(Var_Table *vartable, Board *board, char *name, int i, int j);
void store_var(Var_Table *vartable, int i, int j, char *name);
int access_var(Board *board, Var_Table *vartable, char *name);
void make_var(Var_Table *vartable, Board *board, int i, int j, char *name);
int check_valid_co(int i, int j);

// error handling

void move_err(int err, int o, int d, char* x);
void eol_err();
void asgn_err(int err, int x, int i, int j);
void gen_err();
void print_stderr(int err);
void print_stderr_ok(Board *board, Var_Table *vartable);
void var_asgn_err(int err);







// terminal customization

/*
 * This is free and unencumbered software released into the public domain.
 *
 * For more information, please refer to <https://unlicense.org>
 * got from https://gist.github.com/RabaDabaDoba
 */

//Regular text
#define BLK "\e[0;30m"
#define RED "\e[0;31m"
#define GRN "\e[0;32m"
#define YEL "\e[0;33m"
#define BLU "\e[0;34m"
#define MAG "\e[0;35m"
#define CYN "\e[0;36m"
#define WHT "\e[0;37m"

//Regular bold text
#define BBLK "\e[1;30m"
#define BRED "\e[1;31m"
#define BGRN "\e[1;32m"
#define BYEL "\e[1;33m"
#define BBLU "\e[1;34m"
#define BMAG "\e[1;35m"
#define BCYN "\e[1;36m"
#define BWHT "\e[1;37m"

//Regular underline text
#define UBLK "\e[4;30m"
#define URED "\e[4;31m"
#define UGRN "\e[4;32m"
#define UYEL "\e[4;33m"
#define UBLU "\e[4;34m"
#define UMAG "\e[4;35m"
#define UCYN "\e[4;36m"
#define UWHT "\e[4;37m"

//Regular background
#define BLKB "\e[40m"
#define REDB "\e[41m"
#define GRNB "\e[42m"
#define YELB "\e[43m"
#define BLUB "\e[44m"
#define MAGB "\e[45m"
#define CYNB "\e[46m"
#define WHTB "\e[47m"

//High intensty background 
#define BLKHB "\e[0;100m"
#define REDHB "\e[0;101m"
#define GRNHB "\e[0;102m"
#define YELHB "\e[0;103m"
#define BLUHB "\e[0;104m"
#define MAGHB "\e[0;105m"
#define CYNHB "\e[0;106m"
#define WHTHB "\e[0;107m"

//High intensty text
#define HBLK "\e[0;90m"
#define HRED "\e[0;91m"
#define HGRN "\e[0;92m"
#define HYEL "\e[0;93m"
#define HBLU "\e[0;94m"
#define HMAG "\e[0;95m"
#define HCYN "\e[0;96m"
#define HWHT "\e[0;97m"

//Bold high intensity text
#define BHBLK "\e[1;90m"
#define BHRED "\e[1;91m"
#define BHGRN "\e[1;92m"
#define BHYEL "\e[1;93m"
#define BHBLU "\e[1;94m"
#define BHMAG "\e[1;95m"
#define BHCYN "\e[1;96m"
#define BHWHT "\e[1;97m"

//Reset
#define reset "\e[0m"

#endif