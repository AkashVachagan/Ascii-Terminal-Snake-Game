#include <stdio.h>
#include <ncurses.h>
#include <stdlib.h>
#include <time.h>

void initSnake(int row, int coln, int board[row][coln], int head[2], int tail[2], int* len);
void placeApple(int row, int coln, int board[row][coln]);
void displaySnake(int row, int coln, int board[row][coln]);
void initBoard(int row, int coln, int board[row][coln]);

int main(){
    int row, coln;
    srand(time(NULL));
    initscr();
    getmaxyx(stdscr, row, coln);
    halfdelay(20);
    noecho();

    int board[row][coln];
    int head[2] = {row/2, (coln/2) + 2};
    int tail[2] = {row/2, (coln/2) - 2};
    int direction = 0; // 0 - right, 1 - down, 2 - left, 3 - up
    int len = 0;
    
    clear();
    initBoard(row, coln, board);
    initSnake(row, coln, board, head, tail, &len);
    placeApple(row, coln, board);
    displaySnake(row, coln, board);
    refresh();
    getch();
    endwin();

    return 0;
}

void initBoard(int row, int coln, int board[row][coln]){
    for (int i = 0; i < row; i++){
        for (int j = 0; j < coln; j++){
            board[i][j] = 0;
        }
    }
}

void initSnake(int row, int coln, int board[row][coln], int head[2], int tail[2], int* len){
    int initRow = head[0];
    for (int i = head[1]; i >= tail[1]; i--){
        board[initRow][i] = ++(*len);
    }
}

void displaySnake(int row, int coln, int board[row][coln]){
    for (int i = 0; i < row; i++){
        for (int j = 0; j < coln; j++){
            if (board[i][j] > 0){
                mvaddch(i, j, 'O'); // snake body
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

