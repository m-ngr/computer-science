/*
    MAZE SOLVER CODED BY : MAHMOUD ELNAGAR.

    - TODO : MAZE GENERATOR.
*/
#include <stdio.h>

#define XMAZE 12
#define YMAZE 12

void solveMaze(const size_t xSize, const size_t ySize, char maze[xSize][ySize], size_t *x, size_t *y);
void printMaze(const size_t xSize, const size_t ySize, char maze [xSize][ySize]);

int main(void){
    //initialize.
    char maze[XMAZE][YMAZE] = {
        {'#','#','#','#','#','#','#','#','#','#','#','#'},
        {'#',' ',' ',' ','#',' ',' ',' ',' ',' ',' ','#'},
        {' ',' ','#',' ','#',' ','#','#','#','#',' ','#'},
        {'#','#','#',' ','#',' ',' ',' ',' ','#',' ','#'},
        {'#',' ',' ',' ',' ','#','#','#',' ','#',' ',' '},
        {'#','#','#','#',' ','#',' ','#',' ','#',' ','#'},
        {'#',' ',' ','#',' ','#',' ','#',' ','#',' ','#'},
        {'#','#',' ','#',' ','#',' ','#',' ','#',' ','#'},
        {'#',' ',' ',' ',' ',' ',' ',' ',' ','#',' ','#'},
        {'#','#','#','#','#','#',' ','#','#','#',' ','#'},
        {'#',' ',' ',' ',' ',' ',' ','#',' ',' ',' ','#'},
        {'#','#','#','#','#','#','#','#','#','#','#','#'},
        };
        //starting point of the maze.
        size_t x = 4, y = 11;
        //Solve and Print.
        solveMaze(XMAZE, YMAZE, maze, &x, &y);
        printMaze(XMAZE, YMAZE, maze);
}
//Solve the Maze Recursively.
void solveMaze(const size_t xSize, const size_t ySize, char maze[xSize][ySize], size_t *x, size_t *y){
    //mark current position as visited.
    maze[*x][*y] = '.';
    //search for next empty position.
    if(maze[*x][*y-1]==' '){
        --*y; //move left
    }else if(maze[*x][*y+1]==' '){
        ++*y; //move right
    }else if(maze[*x+1][*y]==' '){
        ++*x; //move down
    }else if(maze[*x-1][*y]==' '){
        --*x; //move up
    }else{//no empty position.
        //back track, mark current position as dead end.
        maze[*x][*y] = '*';
        //search for the back track.
        if(maze[*x][*y-1]=='.'){
            --*y; //move left
        }else if(maze[*x][*y+1]=='.'){
            ++*y; //move right
        }else if(maze[*x+1][*y]=='.'){
            ++*x; //move down
        }else if(maze[*x-1][*y]=='.'){
            --*x; //move up
        }
    }
    //mark new position as visited
    maze[*x][*y] = '.';
    // if you did't hit any wall then continue to solve.
    if (*y != 0 && *x != 0 && *y != ySize - 1 && *x != xSize - 1){
        solveMaze(xSize, ySize, maze, x, y);
    }
}
//print the maze.
void printMaze(const size_t xSize, const size_t ySize, char maze [xSize][ySize]){
    for (size_t x = 0 ; x < xSize ; x++){
        for(size_t y = 0 ; y < ySize ; y++){
            if (maze[x][y] == '*'){
                printf("%c ",' '); //replace back track with empty position.
            }else{
                printf("%c ",maze[x][y]);
            }
        }
        puts("");
    }
}
