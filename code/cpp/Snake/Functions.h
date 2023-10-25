#pragma once
#include<vector>
#include"Console_Funcs.h"
#include<iostream>

constexpr int BOMB_RATE = 50; //No. of moves to change bombs
constexpr int FRUIT_RATE = 100; //No. of moves to show/hide super fruit
constexpr int LEVEL_SCORE_UP = 10; //Score required per level
constexpr int LEVEL_BOMB_UP = 5; //No. of bombs per level
constexpr char SNAKE_SYMBOL = '*';
constexpr char FRUIT_SYMBOL = 'F';
constexpr char SUPER_FRUIT_SYMBOL = 'S';
constexpr char BOMB_SYMBOL = 'B';
constexpr int FRUIT_SCORE = 2;
constexpr int SUPER_FRUIT_SCORE = 10;
constexpr int DELAY = 50; //ms
constexpr int INFO_WIDTH = 30;

enum class Direction { STOP = 'q', UP = 'w', DOWN = 's', LEFT = 'a', RIGHT = 'd' };

class Point {
public:
	Point() {};
	Point(int x_val, int y_val, char symbol,bool act = false);

	void set_x(int value);
	void set_y(int value);

	int get_x()const { return x; }
	int get_y()const { return y; }
	int x_max()const { return max_x; }
	int y_max()const { return max_y; }

	void set_symbol(char symbol) { sym = symbol; }
	char get_symbol() { return sym; }

	void draw();

	bool active{ false }; //in-active points are neglected 
private:
	static int max_x; //static: one for all objects
	static int max_y; //static: one for all objects
	int x{ 0 };
	int y{ 0 };
	char sym{ '*' };
};
//a better design is to separate snake from fruit and bombs
//It is easy but will take time
class Snake {
public:
	bool is_alive();
	void move();
	void draw();
	void reset();
	void set_direction(Direction dir);
private:
	Point fruit{ 0,0 ,true };
	Point super_fruit{ 0,0,false };
	std::vector<Point> points{ Point(0,0, SNAKE_SYMBOL,true) };
	std::vector<Point> bombs;

	int counter{ 0 };
	int bombs_num{ LEVEL_BOMB_UP };
	int bomb_idx{ 0 };
	int bombs_counter{ 0 };

	int score{ 0 };
	int win_score{ LEVEL_SCORE_UP };
	int level{ 1 };

	Direction direction{ Direction::STOP };

	void grow(int n);
	Point random_point(char symbol);
	void check_fruit();
	void check_bombs();
	void level_up();
};