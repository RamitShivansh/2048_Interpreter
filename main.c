#include "interpreter.h"

// UTILITY
void dec(int w) {
    if(w) printf(BGRN "----> " reset);
    else printf(BGRN "2048> " reset);
}

void print_error_bred() {
    printf(BRED "error: " reset);
}

void zero_board(Board* board) {
    for (int i = 0; i < N; i++)
        for(int j = 0; j < N; j++)
            board->tiles[i][j] = 0;
    random_tile(board);

    print_board(board);
}

void print_init() {
    dec(0);
    printf(BCYN "WELCOME TO THE 2048 GAME ENGINE.\n" reset);
}

void print_board(Board *board) {
    dec(0);
    printf("The current state is:\n");
    for (int i = 0; i < N; i++)
    {
        printf("---------------------\n");
        for (int j = 0; j < N; j++)
        {
            if(board->tiles[i][j]) printf("|%4d", board->tiles[i][j]);
            else printf("|    ");
        }
        printf("|\n");
    }
    printf("---------------------\n");
}

void copy_vartable(Var_Table *a, Var_Table *b) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            a->ind[i][j] = b->ind[i][j];
        }
    }

    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            for (int k = 0; k < a->ind[i][j]; k++)
            {
                bzero(a->variables[i][j][k], MAX_VAR_LEN);
                strcpy(a->variables[i][j][k], b->variables[i][j][k]);
            }
        }
    }
}

void change_vartable(Var_Table *vartable, int new_board[N][N][2]) {
    Var_Table *temp = (Var_Table *) malloc(sizeof(Var_Table));
    copy_vartable(temp, vartable);

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if(new_board[i][j][0] < N) {
                vartable->ind[i][j] = temp->ind[new_board[i][j][0]][new_board[i][j][1]];
            }
            else vartable->ind[i][j] = 0;
        }
    }
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            for (int k = 0; k < vartable->ind[i][j]; k++) {
                bzero(vartable->variables[i][j][k], MAX_VAR_LEN);
                strcpy(vartable->variables[i][j][k], temp->variables[new_board[i][j][0]][new_board[i][j][1]][k]);
            }
        }
    }
}

void merge_tiles(Var_Table *vartable, int i1, int j1, int i2, int j2) {
    for (int k = 0; k < vartable->ind[i2][j2]; k++) {
        // printf(BBLU "merge: %s\n" reset, vartable->variables[i2][j2][k]);
        store_var(vartable, i1 + 1, j1 + 1, vartable->variables[i2][j2][k]);
    }
}

void destroy_tile(Var_Table *vartable, int i, int j) {
    for (int k = 0; k < vartable->ind[i][j]; k++) {
        bzero(vartable->variables[i][j][k], MAX_VAR_LEN);
    }
    vartable->ind[i][j] = 0;
}

// void print_vartable(Var_Table *vartable, int i, int j) {
//     printf(BMAG "debug merge: [%d,%d](%d) => " reset, i + 1, j + 1, vartable->ind[i][j]);
//     for (int k = 0; k < vartable->ind[i][j]; k++) {
//         printf(BYEL "%s " reset, vartable->variables[i][j][k]);
//     }
//     printf("\n");
// }

void move_merge_tiles(Board *board, Var_Table *vartable, int *dir, int merge, int opn) {
    int mode = (dir[0] + dir[1] < 0);

    int new_board[N][N][2];
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            new_board[i][j][0] = N;
            new_board[i][j][1] = N;
        }
    }

    for (int i = mode ? 0 : N - 1; mode ? i < N : i >= 0; mode? i++ : i--) {
        for (int j = mode ? 0 : N - 1; mode ? j < N : j >= 0; mode? j++ : j--) {
            if(board->tiles[i][j]) {
                if(merge) {
                    if(i - dir[0] >= 0 && i - dir[0] < N && j - dir[1] >= 0 && j - dir[1] < N) {
                        if(board->tiles[i][j] == board->tiles[i - dir[0]][j - dir[1]]) {
                            board->tiles[i][j] = do_operation(opn, board->tiles[i][j]);
                            board->tiles[i - dir[0]][j - dir[1]] = 0;
                            // print_vartable(vartable, i, j);
                            // print_vartable(vartable, i - dir[0], j - dir[1]);

                            if(opn == 2) destroy_tile(vartable, i, j);
                            else merge_tiles(vartable, i, j, i - dir[0], j - dir[1]);
                            destroy_tile(vartable, i - dir[0], j - dir[1]);

                            // print_vartable(vartable, i, j);
                            // print_vartable(vartable, i - dir[0], j - dir[1]);
                        }
                    }
                }
                else {
                    int y = j, x = i;
                    while(x + dir[0] >= 0 && x + dir[0] < N && y + dir[1] >= 0 && y + dir[1] < N && !board->tiles[x + dir[0]][y + dir[1]]) {
                        board->tiles[x + dir[0]][y + dir[1]] = board->tiles[x][y];
                        board->tiles[x][y] = 0;
                        x += dir[0];
                        y += dir[1];
                    }
                    new_board[x][y][0] = i;
                    new_board[x][y][1] = j;
                }
            }
        }
    }

    if(!merge) change_vartable(vartable, new_board); 
}

int do_operation(int opn, int x) {
    switch (opn)
    {
    case 1:
        return 2 * x;
    case 2:
        return 0;
    case 3:
        return x * x;
    case 4:
        return 1;
    }

    return -1;
}

void convert(int direction, int *dir) {
    switch (direction)
    {
    case 1:
        dir[0] = 0;
        dir[1] = -1;
        break;
    case 2:
        dir[0] = 0;
        dir[1] = 1;
        break;
    case 3:
        dir[0] = -1;
        dir[1] = 0;
        break;
    case 4:
        dir[0] = 1;
        dir[1] = 0;
        break;
    }
} 

void dir_str(int d, char *st) {
    bzero(st, 15);

    switch (d)
    {
    case 1:
        strcpy(st, "LEFT");
        break;
    case 2:
        strcpy(st, "RIGHT");
        break;
    case 3:
        strcpy(st, "UP");
        break;
    case 4:
        strcpy(st, "DOWN");
        break;
    }
}

void opn_str(int o, char *st) {
    bzero(st, 15);

    switch (o)
    {
    case 1:
        strcpy(st, "ADD");
        break;
    case 2:
        strcpy(st, "SUBTRACT");
        break;
    case 3:
        strcpy(st, "MULTIPLY");
        break;
    case 4:
        strcpy(st, "DIVIDE");
        break;
    }
}

void print_opn_dir(int d, int o) {
    char opn[15], dir[15];
    opn_str(o, opn);
    dir_str(d, dir);
    printf("%sING %s\n", opn, dir);
}

void make_move(Board *board, Var_Table *vartable, int direction, int opn) {
    dec(0);
    print_opn_dir(direction, opn);
    int dir[2];
    convert(direction, dir);

    move_merge_tiles(board,vartable , dir, 0, opn);
    move_merge_tiles(board,vartable , dir, 1, opn);
    move_merge_tiles(board,vartable , dir, 0, opn);

    random_tile(board);

    print_board(board);
    print_stderr_ok(board, vartable);
    dec(1);
}

void assign(Board *board, Var_Table *vartable, int val, int i, int j) {
    dec(0);
    if(!check_valid_co(i, j)) {
        dec(1);
        return;
    }
    if(val < 0) {
        print_error_bred();
        printf("Negative values cannot be assigned to a tile.\n");
        print_stderr(1);
        dec(1);
        return;
    }
    printf("ASSIGNING...\n");
    board->tiles[i - 1][j - 1] = val;
    if(val == 0) destroy_tile(vartable, i - 1, j - 1);
    print_board(board);
    print_stderr_ok(board, vartable);
    dec(1);
}

int query(Board *board, int i, int j, int print) {
    dec(0);
    if(!check_valid_co(i, j)) {
        dec(1);
        return -1;
    }
    if(print) printf("Value at" CYN " [%d, %d]" reset " is " CYN "%d" reset ".\n", i, j, board->tiles[i - 1][j - 1]);
    dec(1);
    return board->tiles[i - 1][j - 1];
}

void random_tile(Board *board) {
    int x,y;
    while(board->tiles[x = rand() % N][y = rand() % N]);

    board->tiles[x][y] = rand() % 2 ? 2 : 4;
}

// some other functions

void init_vartable(Var_Table *vartable) {
    for(int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            for (int k = 0; k < MAX_VAR; k++) {
                bzero(vartable->variables[i][j][k], MAX_VAR_LEN);
            }
        }
    }

    for(int i = 0; i < N; i++) {
        for(int j = 0; j < N; j++) {
            vartable->ind[i][j] = 0;
        }
    }
}

int check_valid_co(int i, int j) {
    if(i < 1 || i > N || j < 1 || j > N) {
        print_error_bred();
        printf("Tile position coordinates can only be in range " CYN "[1, 4]\n" reset);
        print_stderr(1);
        return 0;
    }
    return 1;
}

int check_valid_var(Var_Table *vartable, Board *board, char *name, int x, int y) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            for (int k = 0; k < vartable->ind[i][j]; k++) {
                if(strcmp(name, vartable->variables[i][j][k]) == 0) {
                    print_error_bred();
                    printf("%s is already stored as [%d, %d]\n", name, i + 1, j + 1);
                    print_stderr(1);
                    return 0;
                }
            }  
        }
    }

    if(board->tiles[x - 1][y - 1] == 0) {
        print_error_bred();
        printf("cannot assign variable to an empty tile.\n");
        print_stderr(1);
        return 0;
    }
    
    return 1;
}

void store_var(Var_Table *vartable, int i, int j, char *name) {
    i--; j--;
    bzero(vartable->variables[i][j][vartable->ind[i][j]], MAX_VAR_LEN);
    strcpy(vartable->variables[i][j][vartable->ind[i][j]++], name);
}

int access_var(Board *board, Var_Table *vartable, char *name) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            for (int k = 0; k < vartable->ind[i][j]; k++) {
                if(strcmp(name, vartable->variables[i][j][k]) == 0) {
                    return board->tiles[i][j];
                }
            }  
        }
    }
    
    return -1;
}

void make_var(Var_Table *vartable, Board *board, int i, int j, char *name) {
    dec(0);
    if(check_valid_co(i, j) && check_valid_var(vartable, board, name, i, j)) {
        store_var(vartable, i, j, name);
        printf(BYEL "SUCCESS! " reset "stored var %s as [%d, %d]\n", name, i, j);
    }
    else {
        dec(1);
        return;
    }
    print_stderr_ok(board, vartable);
    dec(1);
}

// error handling

void move_err(int err, int o, int d, char* x) {
    dec(0);

    char opn[15], dir[15];
    opn_str(o, opn);
    dir_str(d, dir);

    print_error_bred();

    switch (err)
    {
    case 1:
        printf("Expected a direction " CYN "(LEFT, RIGHT, UP or DOWN)" reset " after %s instead of " RED "%s.\n" reset, opn, x);
        break;
    case 2:
        printf("Expected a direction " CYN "(LEFT, RIGHT, UP or DOWN)" reset " after %s instead of " RED "%s" reset ". A full-stop is also required to end a command.\n", opn, x);
        break;
    case 3:
        printf("Direction should come after operation.\n\tDid you mean " CYN "\"%s %s.\"" reset "?\n", opn, dir);
        break;
    
    default:
        printf("wrong syntax\n");
        break;
    }

    print_stderr(1);
    dec(1);
}

void eol_err() {
    dec(0); 
    print_error_bred(); 
    printf("You need to end a command with a full-stop.\n"); 
    print_stderr(1);
    dec(1);
}

void asgn_err(int err, int x, int i, int j) {
    dec(0);
    print_error_bred();

    switch (err)
    {
    case 1:
        printf("expected " BCYN "ASSIGN %d TO %d,%d" reset ".\n", x, i, j);
        break;
    case 2:
        printf(BRED "syntax error\n\t" reset "Did you mean " BCYN "ASSIGN %d TO %d,%d." reset " ?\n", x, i, j);
    default:
        break;
    }
    print_stderr(1);
    dec(1);
}

void gen_err() {
    dec(0);
    printf(BRED "syntax error\n" reset);
    print_stderr(1);
}

void print_stderr(int err) {
    fprintf(stderr, reset "-1\n");
}

void print_stderr_ok(Board *board, Var_Table *vartable) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            fprintf(stderr, "%d ", board->tiles[i][j]);
        }
    }
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if(vartable->ind[i][j] > 0) fprintf(stderr, "%d,%d", i + 1, j + 1);
            for (int k = 0; k < vartable->ind[i][j]; k++) {
                if(k == vartable->ind[i][j] - 1) fprintf(stderr, "%s ", vartable->variables[i][j][k]);
                else fprintf(stderr, "%s,", vartable->variables[i][j][k]);
            }
        }
    }
    
    fprintf(stderr, "\n");
}

void var_asgn_err(int err) {
    dec(0);
    print_error_bred();

    switch (err)
    {
    case 1:
        printf("No, a keyword cannot be a variable name.\n");
        break;
    case 2:
        printf("Illegal variable name.\n");
        break;
    
    default:
        break;
    }

    print_stderr(1);
    dec(1);
}