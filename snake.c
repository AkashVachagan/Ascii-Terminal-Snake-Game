#include <stdio.h>
#include <ncurses.h>
#include <stdlib.h>
#include <time.h>

int UPPER_ROW;
int BOTTOM_ROW;
int LEFT_COLN;
int RIGHT_COLN;

void initSnake(int row, int coln, int board[row][coln], int direction[row][coln], int head[2], int tail[2]);
void placeApple(int row, int coln, int board[row][coln]);
void displaySnake(int row, int coln, int board[row][coln]);
void initBoardDirection(int row, int coln, int board[row][coln], int direction[row][coln]);
void updateSnake(int row, int coln, int board[row][coln], int direction[row][coln]);
void updateDirection(int row, int coln, int board[row][coln], int direction[row][coln], int updateDir);
int checkDead(int row, int coln, int board[row][coln], int direction[row][coln]);
void initTitle(int row, int coln, int board[row][coln]);
void initBorder(int row, int coln, int board[row][coln]);
void showDeathScreen(int row, int coln, int board[row][coln]);

int main(){
    int row, coln, ch, isDead = 0;
    srand(time(NULL));
    initscr();
    getmaxyx(stdscr, row, coln);
    halfdelay(1); // 0.1 for seeing each move
    noecho();
    keypad(stdscr, TRUE);
    LEFT_COLN = 1;
    RIGHT_COLN = coln-2;
    UPPER_ROW = 18;
    BOTTOM_ROW = row-2;

    int board[row][coln]; // 1 - right, 2 - down, 3 - left, 4 - up
    int direction[row][coln];
    int head[2] = {row/2, (coln/2) + 2};
    int tail[2] = {row/2, (coln/2) - 2};
    
    clear();
    initTitle(row, coln, board);
    initBorder(row, coln, board);
    initBoardDirection(row, coln, board, direction);
    initSnake(row, coln, board, direction, head, tail); // 1 - head, 2 - body, 3 - tail 
    placeApple(row, coln, board);
    displaySnake(row, coln, board);
    refresh();

    do{
        initTitle(row, coln, board);
        initBorder(row, coln, board);
        ch = getch();
        if (ch != ERR){
            switch(ch){
                case KEY_RIGHT:
                    updateDirection(row, coln, board, direction, 1);
                    break;
                case KEY_DOWN:
                    updateDirection(row, coln, board, direction, 2);
                    break;
                case KEY_LEFT:
                    updateDirection(row, coln, board, direction, 3);
                    break;
                case KEY_UP:
                    updateDirection(row, coln, board, direction, 4);
                    break;
            }
        }
        isDead = checkDead(row, coln, board, direction);
        if (isDead){
            showDeathScreen(row, coln, board);
            break;
        }
        updateSnake(row, coln, board, direction);
        displaySnake(row, coln, board);
        refresh();
    } while(1);

    for (int i = 0; i < 15; i++){
        getch();
    }
    endwin();

    return 0;
}

void showDeathScreen(int row, int coln, int board[row][coln]){
    clear();
    const char* splash[] = {
"                                                        ....             .                  ..       ",
"   .xnnx.  .xx.                                     .xH888888Hx.        @88>              dF         ",
" .f``\"888X< `888.         u.      x.    .         .H8888888888888:      %8P              '88bu.      ",
" 8L   8888X  8888   ...ue888b   .@88k  z88u       888*\"\"\"?\"\"*88888X      .         .u    '*88888bu   ",
"X88h. `8888  X888k  888R Y888r ~\"8888 ^8888      'f     d8x.   ^%88k   .@88u    ud8888.    ^\"*8888N  ",
"'8888 '8888  X8888  888R I888>   8888  888R      '>    <88888X   '?8  ''888E` :888'8888.  beWE \"888L ",
" `*88>'8888  X8888  888R I888>   8888  888R       `:..:`888888>    8>   888E  d888 '88%\"  888E  888E ",
"   `! X888~  X8888  888R I888>   8888  888R              `\"*88     X    888E  8888.+\"     888E  888E ",
"  -`  X*\"    X8888 u8888cJ888    8888 ,888B .       .xHHhx..\"      !    888E  8888L       888E  888F ",
"   xH88hx  . X8888  \"*888*P\"    \"8888Y 8888\"       X88888888hx. ..!     888&  '8888c. .+ .888N..888  ",
" .*\"*88888~  X888X    'Y\"        `Y\"   'YP        !   \"*888888888\"      R888\"  \"88888%    `\"888*\"\"   ",
" `    \"8%    X888>                                       ^\"***\"`         \"\"      \"YP'        \"\"      ",
"    .x..     888f                                                                                    ",
"   88888    :88f                                                                                    ",
"   \"88*\"  .x8*~                                                                                     "
};

    int num_lines = sizeof(splash)/sizeof(splash[0]);

     for (int i = 0; i < num_lines; i++){
         mvprintw(i+15, 25, "%s", splash[i]);
     }

    refresh();
}

void initBorder(int row, int coln, int board[row][coln]){
    for (int i = UPPER_ROW-1; i <= BOTTOM_ROW+1; i++){
        mvaddch(i, LEFT_COLN-1, '|');
        mvaddch(i, RIGHT_COLN+1, '|');
    }

    for (int i = 0; i < coln; i++){
        mvaddch(UPPER_ROW-1, i, '-');
        mvaddch(BOTTOM_ROW+1, i, '-');
    }
}

void initTitle(int row, int coln, int board[row][coln]){
    const char* splash[] = {
"     ...               ..    .         s                                                                                  ",
"   .x888888hx    : x .d88\"    @88>      :8      .uef^\"                                                                      ",
"  d88888888888hxx   5888R     %8P      .88    :d88E                     .u    .                        uL   ..              ",
" 8\" ... `\"*8888%`   '888R      .      :888ooo `888E            .u     .d88B :@8c              .u     .@88b  @88R      .u    ",
"!  \"   ` .xnxx.      888R    .@88u  -*8888888  888E .z8k    ud8888.  =\"8888f8888r          ud8888.  '\"Y888k/\"*P    ud8888.  ",
"X X   .H8888888%:    888R   ''888E`   8888     888E~?888L :888'8888.   4888>'88\"         :888'8888.    Y888L     :888'8888. ",
"X 'hn8888888*\"   >   888R     888E    8888     888E  888E d888 '88%\"   4888> '           d888 '88%\"     8888     d888 '88%\" ",
"X: `*88888%`     !   888R     888E    8888     888E  888E 8888.+\"      4888>             8888.+\"        `888N    8888.+\"    ",
"'8h.. ``     ..x8>   888R     888E   .8888Lu=  888E  888E 8888L       .d888L .+      .   8888L       .u./\"888&   8888L      ",
" `88888888888888f   .888B .   888&   ^%888*    888E  888E '8888c. .+  ^\"8888*\"     .@8c  '8888c. .+ d888\" Y888*\" '8888c. .+ ",
"  '%8888888888*\"    ^*888%    R888\"    'Y\"    m888N= 888>  \"88888%       \"Y\"      '%888\"  \"88888%   ` \"Y   Y\"     \"88888%   ",
"     ^\"****\"\"`        \"%       \"\"              `Y\"   888     \"YP'                   ^*      \"YP'                    \"YP'    ",
"                                                    J88\"                                                                    ",
"                                                    @%                                                                      ",
"                                                  :\"                                                                       "
};
 
     int num_lines = sizeof(splash)/sizeof(splash[0]);

     for (int i = 0; i < num_lines; i++){
         mvprintw(i+1, 13, "%s", splash[i]);
     }

    refresh();
}

int checkDead(int row, int coln, int board[row][coln], int direction[row][coln]){
    for (int i = UPPER_ROW; i <= BOTTOM_ROW; i++){
        for (int j = LEFT_COLN; j <= RIGHT_COLN; j++){
            if (board[i][j] == 1){
                switch (direction[i][j]){
                    case 1:
                        if (j == RIGHT_COLN){
                            if (board[i][LEFT_COLN] > 0) return 1;
                        }
                        else{
                            if (board[i][j+1] > 0) return 1;
                        }
                        break;
                    case 2:
                        if (i == BOTTOM_ROW){
                            if (board[UPPER_ROW][j] > 0) return 1;
                        }
                        else{
                            if (board[i+1][j] > 0) return 1;
                        }
                        break;
                    case 3:
                        if (j == LEFT_COLN){
                            if (board[i][RIGHT_COLN] > 0) return 1;
                        }
                        else{
                            if (board[i][j-1] > 0) return 1;
                        }
                        break;
                    case 4:
                        if (i == UPPER_ROW){
                            if (board[BOTTOM_ROW][j] > 0) return 1;
                        }
                        else{
                            if (board[i-1][j] > 0) return 1;
                        }
                        break;
                }
            }
        }
    }
    return 0;
}

void updateDirection(int row, int coln, int board[row][coln], int direction[row][coln], int updateDir){
    for (int i = UPPER_ROW; i <= BOTTOM_ROW; i++){
        for (int j = LEFT_COLN; j <= RIGHT_COLN; j++){
            if (board[i][j] == 1){
                direction[i][j] = updateDir;
                return;
            }
        }
    }
}

void updateSnake(int row, int coln, int board[row][coln], int direction[row][coln]){
    int newBoard[row][coln];
    int eatingApple = 0;
    for (int i = 0; i < row; i++){
        for (int j = 0; j < coln; j++){
            newBoard[i][j] = board[i][j];
        }
    }

    for (int i = UPPER_ROW; i <= BOTTOM_ROW; i++){
        for (int j = LEFT_COLN; j <= RIGHT_COLN; j++){
            if (board[i][j] == 1){
                switch(direction[i][j]){
                    case 1:
                        if (j == RIGHT_COLN){
                            eatingApple = (newBoard[i][LEFT_COLN] == -1) ? 1 : 0;
                            newBoard[i][LEFT_COLN] = 1;
                            direction[i][LEFT_COLN] = 1;
                        }
                        else{
                            eatingApple = (newBoard[i][j+1] == -1) ? 1 : 0;
                            newBoard[i][j+1] = 1;
                            direction[i][j+1] = 1;
                        }
                        break;
                    case 2:
                        if (i == BOTTOM_ROW){
                            eatingApple = (newBoard[UPPER_ROW][j] == -1) ? 1 : 0;
                            newBoard[UPPER_ROW][j] = 1;
                            direction[UPPER_ROW][j] = 2;
                        }
                        else{
                            eatingApple = (newBoard[i+1][j] == -1) ? 1 : 0;
                            newBoard[i+1][j] = 1;
                            direction[i+1][j] = 2;
                        }
                        break;
                    case 3:
                        if (j == LEFT_COLN){
                            eatingApple = (newBoard[i][RIGHT_COLN] == -1) ? 1 : 0;
                            newBoard[i][RIGHT_COLN] = 1;
                            direction[i][RIGHT_COLN] = 3;
                        }
                        else{
                            eatingApple = (newBoard[i][j-1] == -1) ? 1 : 0;
                            newBoard[i][j-1] = 1;
                            direction[i][j-1] = 3;
                        }
                        break;
                    case 4:
                        if (i == UPPER_ROW){
                            eatingApple = (newBoard[BOTTOM_ROW][j] == -1) ? 1 : 0;
                            newBoard[BOTTOM_ROW][j] = 1;
                            direction[BOTTOM_ROW][j] = 4;
                        }
                        else{
                            eatingApple = (newBoard[i-1][j] == -1) ? 1 : 0;
                            newBoard[i-1][j] = 1;
                            direction[i-1][j] = 4;
                        }
                        break;
                }
                newBoard[i][j] = 2;
            }
            if (board[i][j] == 3 && eatingApple == 0){
                newBoard[i][j] = 0;
                switch(direction[i][j]){
                    case 1:
                        if (j == RIGHT_COLN){
                            newBoard[i][LEFT_COLN] = 3;
                        }
                        else{
                            newBoard[i][j+1] = 3;
                        }
                        break;
                    case 2:
                        if (i == BOTTOM_ROW){
                            newBoard[UPPER_ROW][j] = 3;
                        }
                        else{
                            newBoard[i+1][j] = 3;
                        }
                        break;
                    case 3:
                        if (j == LEFT_COLN){
                            newBoard[i][RIGHT_COLN] = 3;
                        }
                        else{
                            newBoard[i][j-1] = 3;
                        }
                        break;
                    case 4:
                        if (i == UPPER_ROW){
                            newBoard[BOTTOM_ROW][j] = 3;
                        }
                        else{
                            newBoard[i-1][j] = 3;
                        }
                        break;
                }
                direction[i][j] = 0;
            }
        }
    }

    if (eatingApple) placeApple(row, coln, newBoard);

    for (int i = 0; i < row; i++){
        for (int j = 0; j < coln; j++){
            board[i][j] = newBoard[i][j];
        }
    }
}

void initBoardDirection(int row, int coln, int board[row][coln], int direction[row][coln]){
    for (int i = 0; i < row; i++){
        for (int j = 0; j < coln; j++){
            board[i][j] = 0;
            direction[i][j] = 0;
        }
    }
}

void initSnake(int row, int coln, int board[row][coln], int direction[row][coln], int head[2], int tail[2]){
    int initRow = head[0];
    board[initRow][head[1]] = 1;
    direction[initRow][head[1]] = 1;
    for (int i = head[1]-1; i > tail[1]; i--){
        board[initRow][i] = 2;
        direction[initRow][i] = 1;
    }

    board[initRow][tail[1]] = 3; 
    direction[initRow][tail[1]] = 1;
}

void displaySnake(int row, int coln, int board[row][coln]){
    for (int i = UPPER_ROW; i <= BOTTOM_ROW; i++){
        for (int j = LEFT_COLN; j <= RIGHT_COLN; j++){
            if (board[i][j] > 0){
                mvaddch(i, j, 'O'); // snake body
            }
            else if (board[i][j] == 0){
                mvaddch(i, j, ' ');
            }
            else if (board[i][j] == -1){
                mvaddch(i, j, '@'); // apple
            }
        }
    }
}

void placeApple(int row, int coln, int board[row][coln]){
    // not truly random, may change later

    int randomRow, randomColn;
    do{
        randomRow = UPPER_ROW + rand() % (BOTTOM_ROW - UPPER_ROW + 1);
        randomColn = LEFT_COLN + rand() % (RIGHT_COLN - LEFT_COLN + 1);
    }while (board[randomRow][randomColn] > 0);
    board[randomRow][randomColn] = -1;
}

