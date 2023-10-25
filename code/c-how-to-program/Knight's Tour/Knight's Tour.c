/*
 Knight's Tour program by Mahmoud Elnagar.
*/
#include <stdio.h>
#define XSIZE 8
#define YSIZE 8
#define XPOS 0
#define YPOS 0
void printBoard(int board[XSIZE][YSIZE]);
int isAvailable(int board [XSIZE][YSIZE], int x, int y);
int moveKnight(int board [XSIZE][YSIZE], int *x, int *y, int num, int access [XSIZE][YSIZE]);
void reduceAccess(int board [XSIZE][YSIZE],int x, int y, int access [XSIZE][XSIZE],int moveX[],int moveY[]);
int lowestAccess(int board [XSIZE][YSIZE],int x, int y, int access [XSIZE][YSIZE], int moveX[],int moveY[]);

int main(void){
    //initialize
    int x = XPOS, y = YPOS ;
    int board [XSIZE][YSIZE] = {0};
    int lastSuccess = 0;
    int access [XSIZE][YSIZE] = {
        2,3,4,4,4,4,3,2,
        3,4,6,6,6,6,4,3,
        4,6,8,8,8,8,6,4,
        4,6,8,8,8,8,6,4,
        4,6,8,8,8,8,6,4,
        4,6,8,8,8,8,6,4,
        3,4,6,6,6,6,4,3,
        2,3,4,4,4,4,3,2};
    //move the knight 64 move.
    for (int i = 1 ; i <= 64 ; i++){
        if (moveKnight(board,&x,&y,i,access) == 1){
            lastSuccess = i ;
        }
    }
    //print results
    printBoard(board);
    printf("\nSuccessful Moves: %d\n", lastSuccess);
}
//is this place available
int isAvailable(int board [XSIZE][YSIZE], int x, int y){
    if (x < XSIZE && x >= 0 && y < YSIZE && y >= 0){ //coordinates check
        if (board[x][y] == 0){//value check
            return 1; //available
        }
    }
    return 0; //not available
}
//reduce Accessibility to keep track of lowest access.
void reduceAccess(int board [XSIZE][YSIZE],int x, int y, int access [XSIZE][XSIZE],int moveX[],int moveY[]){
    //reduce Accessibility for all places connected to this place (x,y)
    for (size_t i = 0 ; i < 8 ; i++){
        if (isAvailable(board, x + moveX[i], y + moveY[i]) != 0){
            --access[ x + moveX[i] ][ y + moveY[i] ];
        }
    }
}
//get best legal movement.
int lowestAccess(int board [XSIZE][YSIZE],int x, int y, int access [XSIZE][YSIZE], int moveX[],int moveY[]){
    int mode = -1; //if not found return -1 , illegal movement.
    int lowAccess = 10;
    for (size_t i = 0 ; i < 8 ; i++){
        if (isAvailable(board, x + moveX[i], y + moveY[i]) != 0){
            //search for smallest access number in legal movements.
            if (access [ x + moveX[i] ][ y + moveY[i] ] < lowAccess){
                mode = i;
                lowAccess = access [ x + moveX[i] ][ y + moveY[i] ];
            }
        }
    }
    return mode;//return the next legal movement.
}
//move the Knight
int moveKnight(int board [XSIZE][YSIZE], int *x, int *y, int num, int access [XSIZE][YSIZE]){
    //Knight movement Pattern
    int moveX[8] = {1,2,2,1,-1,-2,-2,-1};
    int moveY[8] = {2,1,-1,-2,-2,-1,1,2};
    //get the best legal movement.
    int mode = lowestAccess(board,*x,*y,access,moveX,moveY);
    if (mode != -1){
        *x += moveX[mode];
        *y += moveY[mode];
        board[*x][*y] = num;
        reduceAccess(board,*x,*y,access,moveX,moveY);
        return 1; //success.
    }else {
        return 0; //failed, No available places.
    }
}
//print the board.
void printBoard(int board[XSIZE][YSIZE]){
    for (size_t i = 0 ; i < XSIZE ; i++){
        for (size_t j = 0 ; j < YSIZE ; j++){
            if (board[i][j] == 0){
                printf("%2c\t",'*'); // empty slot
            }else{
                printf("%2d\t", board[i][j]);
            }
        }
        puts("\n");
    }
}
