#pragma once
#include <conio.h>
#include <windows.h>

/*  _kbhit(); // is keyboard hitted (is any key entered)
    _getch(); // get char and don't print it on console (once)
    system("cls"); //clear console
    gotoxy(x,y); // move cursor to x,y position
*/

void gotoxy(int x, int y);
int console_columns();
int console_rows();
void show_cursor(bool visible);