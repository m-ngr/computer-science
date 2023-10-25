#include "Functions.h"
using namespace std;
int Point::max_x{ console_columns() - 1 - INFO_WIDTH };
int Point::max_y{ console_rows() - 1 };
Snake snake;

void check_keys() {
    if (_kbhit()) {
        char ch = _getch();
        switch (ch) {
        case int(Direction::UP) :
            snake.set_direction(Direction::UP);
            break;
        case int(Direction::DOWN) :
            snake.set_direction(Direction::DOWN);
            break;
        case int(Direction::LEFT) :
            snake.set_direction(Direction::LEFT);
            break;
        case int(Direction::RIGHT) :
            snake.set_direction(Direction::RIGHT);
            break;
        case int(Direction::STOP) :
            snake.set_direction(Direction::STOP);
            break;
        }
    }
}

int main() {
    srand(time(0));
    show_cursor(false);
    snake.reset();

    while (snake.is_alive()) {
        check_keys();
        snake.move();
        snake.draw();
    }
    system("cls");
    cout << "You LOST";
    system("pause");
}