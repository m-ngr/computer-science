/*
Turtle Graphics: Coded by Mahmoud Elnagar.
instructions:
 0 : Pen down.
-1 : Pen Up.
-2 : Turn right.
-3 : Turn left.
-4 : Print.
+n : Move n steps.
*/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define XPOS 0
#define YPOS 0
#define XSIZE 40
#define YSIZE 60
#define PROG_SIZE 1500

enum pen_state {PEN_UP,PEN_DOWN};
enum direction {DOWN,RIGHT,UP,LEFT};
void printFloor(int floor[XSIZE][YSIZE]);
void movePen(int floor[XSIZE][YSIZE], int *x , int *y, enum direction direct, int steps,enum pen_state pen);

int main(void){
    srand(time(NULL));
    //initialize.
    int floor [XSIZE][YSIZE] = {0};
    int x = XPOS , y = YPOS;
    enum pen_state pen = PEN_DOWN;
    enum direction direct = DOWN;
    //program array that do the movement
    int program [PROG_SIZE] = {0};
    //random program
    for (size_t i = 1; i < PROG_SIZE ; i++){
        program[i] = -3 + rand()%8;
    }
    program[0] = 0;
    program[PROG_SIZE - 1] = -4;
    //execute the program
    for (size_t i = 0 ; i < PROG_SIZE ; i++){
        switch (program[i]){
    case 0: //PEN_DOWN : Write.
        pen = PEN_DOWN;
        break;
    case -1: //PEN_UP : Don't Write.
        pen = PEN_UP;
        break;
    case -2: //Turn right.
        if(direct < 3){direct++;}else{direct = 0;}
        break;
    case -3: //Turn left.
        if(direct > 0){direct--;}else{direct = 3;}
        break;
    case -4: //Print the floor.
        printFloor(floor);
        break;
    default://Move n steps
        movePen(floor,&x,&y,direct,program[i],pen);
        }
    }
}
//move the pen from (x,y) a steps in the direction direct
void movePen(int floor[XSIZE][YSIZE], int *x , int *y, enum direction direct, int steps,enum pen_state pen){
    if (direct == DOWN){
        steps += *x;
        for(; *x < steps && *x < XSIZE ; ++*x){
           if(pen == PEN_DOWN){floor[*x][*y] = 1;}
        }
    }else if(direct == RIGHT){
        steps += *y;
        for(; *y < steps && *y < YSIZE ; ++*y ){
            if(pen == PEN_DOWN){floor[*x][*y] = 1;}
        }
    }else if(direct == UP){
        steps -= *x;
        for(; *x > steps && *x >= 0 ; --*x ){
            if(pen == PEN_DOWN){floor[*x][*y] = 1;}
        }
    }else{
        steps -= *y;
        for(; *y > steps && *y >= 0 ; --*y ){
            if(pen == PEN_DOWN){floor[*x][*y] = 1;}
        }
    }
    //ensure we still in the boundaries.
    if (*x < 0){*x = 0;}
    if (*y < 0){*y = 0;}
    if (*x > XSIZE - 1){*x = XSIZE - 1;}
    if (*y > YSIZE - 1){*y = YSIZE - 1;}
}
//print the floor.
void printFloor(int floor[XSIZE][YSIZE]){
    for (size_t i = 0 ; i < XSIZE ; i++){
        for (size_t j = 0 ; j < YSIZE ; j++){
            if (floor[i][j] == 1){
                printf("%c",'*');
            }else{
                printf("%c",' ');
            }
        }
        puts("");
    }
}
