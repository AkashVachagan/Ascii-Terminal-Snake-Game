#include <stdio.h>
#include <ncurses.h>
#include <stdlib.h>
#include <time.h>

void initSnake(int row, int coln, int board[row][coln], int direction[row][coln], int head[2], int tail[2]);
void placeApple(int row, int coln, int board[row][coln]);
void displaySnake(int row, int coln, int board[row][coln]);
void initBoardDirection(int row, int coln, int board[row][coln], int direction[row][coln]);
void updateSnake(int row, int coln, int board[row][coln], int direction[row][coln]);

int main(){
    int row, coln, ch;
    srand(time(NULL));
    initscr();
    getmaxyx(stdscr, row, coln);
    halfdelay(5);
    noecho();

    int board[row][coln]; // 1 - right, 2 - down, 3 - left, 4 - up
    int direction[row][coln];
    int head[2] = {row/2, (coln/2) + 2};
    int tail[2] = {row/2, (coln/2) - 2};
    
    clear();
    initBoardDirection(row, coln, board, direction);
    initSnake(row, coln, board, direction, head, tail); // 1 - head, 2 - body, 3 - tail 
    placeApple(row, coln, board);
    displaySnake(row, coln, board);
    refresh();

    do{
        ch = getch();
        if (ch == ERR){
            updateSnake(row, coln, board, direction);
            // check if dead? => maybe updateSnake will do this??
        }
        else{
            // update direction
            // updateSnake
            // check if dead?
        }
        displaySnake(row, coln, board);
        refresh();
    } while(1);
    getch();
    endwin();

    return 0;
}

void updateSnake(int row, int coln, int board[row][coln], int direction[row][coln]){
    int newBoard[row][coln];
    for (int i = 0; i < row; i++){
        for (int j = 0; j < coln; j++){
            newBoard[i][j] = board[i][j];
        }
    }

    for (int i = 0; i < row; i++){
        for (int j = 0; j < coln; j++){
            if (board[i][j] == 1){
                switch(direction[i][j]){
                    case 1:
                        newBoard[i][j+1] = 1;
                        direction[i][j+1] = 1;
                        break;
                    case 2:
                        newBoard[i+1][j] = 1;
                        direction[i+1][j] = 2;
                        break;
                    case 3:
                        newBoard[i][j-1] = 1;
                        direction[i][j-1] = 3;
                        break;
                    case 4:
                        newBoard[i-1][j] = 1;
                        direction[i-1][j] = 4;
                        break;
                }
                newBoard[i][j] = 2;
            }
            if (board[i][j] == 3){
                newBoard[i][j] = 0;
                switch(direction[i][j]){
                    case 1:
                        newBoard[i][j+1] = 3;
                        break;
                    case 2:
                        newBoard[i+1][j] = 3;
                        break;
                    case 3:
                        newBoard[i][j-1] = 3;
                        break;
                    case 4:
                        newBoard[i-1][j] = 3;
                        break;
                }
                direction[i][j] = 0;
            }
        }
    }

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
    for (int i = 0; i < row; i++){
        for (int j = 0; j < coln; j++){
            if (board[i][j] > 0){
                // mvaddch(i, j, 'O'); // snake body
                mvaddch(i, j,'0' +  board[i][j]); // for development, remove later
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
        randomRow = rand() % row;
        randomColn = rand() % coln;
    }while (board[randomRow][randomColn] > 0);
    board[randomRow][randomColn] = -1;
}

