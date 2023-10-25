#include "Functions.h"
//========== Point =============
Point::Point(int x_val, int y_val, char symbol, bool act) :x{ x_val }, y{ y_val }, sym{symbol}, active{ act } {
	if (x < 0) { x = 0; }
	if (y < 0) { y = 0; }
	if (x > max_x) { x = max_x; }
	if (y > max_y) { y = max_y; }
}
void Point::set_x(int value) {
	if (value < 0) {
		x = max_x;
	}
	else if (value > max_x) {
		x = 0;
	}
	else {
		if (x != value) { active = true; }
		x = value;
	}
}
void Point::set_y(int value) {
	if (value < 0) {
		y = max_y;
	}
	else if (value > max_y) {
		y = 0;
	}
	else {
		if (y != value) { active = true; }
		y = value;
	}
}
void Point::draw() {
	if (active) {
		gotoxy(x, y);
		std::cout << sym;
	}
}

bool operator == (Point& p1, Point& p2) {
	if (p1.active == false) { return false; }
	if (p2.active == false) { return false; }
	if ((p1.get_x() == p2.get_x()) && (p1.get_y() == p2.get_y())) {
		return true;
	}
	return false;
}

//============ Snake =================
void Snake::move() {
	if (direction == Direction::STOP) { return; }
	for (int i = points.size() - 1; i > 0; i--) {
		points[i].set_x(points[i - 1].get_x());
		points[i].set_y(points[i - 1].get_y());
	}
	switch (direction)
	{
	case Direction::UP:
		points[0].set_y(points[0].get_y() - 1);
		break;
	case Direction::DOWN:
		points[0].set_y(points[0].get_y() + 1);
		break;
	case Direction::LEFT:
		points[0].set_x(points[0].get_x() - 1);
		break;
	case Direction::RIGHT:
		points[0].set_x(points[0].get_x() + 1);
		break;
	}
	check_fruit();
	check_bombs();
}

void Snake::draw() {
	system("cls");

	for (Point &p : points) {
		p.draw();
	}
	fruit.draw();
	super_fruit.draw();
	for (Point& p : bombs) {
		p.draw();
	}
	// draw information panal
	for (int y = 0; y <= fruit.y_max(); y++) {
		gotoxy(fruit.x_max()+1,y);
		std::cout << '|';
	}
	static int x = fruit.x_max() + 5;
	static int y = fruit.y_max() / 4;
	gotoxy(x, y + 0); std::cout << "Level: " << level << std::endl;
	gotoxy(x, y + 1); std::cout << "Score to win: " << win_score << std::endl;
	gotoxy(x, y + 2); std::cout << "Score: " << score << std::endl;
	//
	Sleep(DELAY);
}

void Snake::grow(int n) {
	if (n < 0) { return; }
	Point last = points[points.size() - 1];
	last.active = false;
	for (int i = 0; i < n; i++) {
		points.push_back(last);
	}
}

void Snake::reset() {
	points.clear();
	points.push_back(Point(0, 0, SNAKE_SYMBOL,true));
	bombs.clear();
	for (int i = 0; i < bombs_num; i++) {
		bombs.push_back(random_point(BOMB_SYMBOL));
	}
	bomb_idx = 0;
	bombs_counter = 0;
	score = 0;
	counter = 0;
	fruit = random_point(FRUIT_SYMBOL);
	direction = Direction::STOP;
}

void Snake::set_direction(Direction dir) {
	switch (dir)
	{
	case Direction::STOP:
		direction = Direction::STOP;
		break;
	case Direction::UP:
		if (direction != Direction::DOWN) {
			direction = Direction::UP;
		}
		break;
	case Direction::DOWN:
		if (direction != Direction::UP) {
			direction = Direction::DOWN;
		}
		break;
	case Direction::LEFT:
		if (direction != Direction::RIGHT) {
			direction = Direction::LEFT;
		}
		break;
	case Direction::RIGHT:
		if (direction != Direction::LEFT) {
			direction = Direction::RIGHT;
		}
		break;
	default:
		break;
	}
}

bool Snake::is_alive() {
	for (int i = 1; i < points.size(); i++) {
		if (points[i] == points[0]) {
				return false;
		}
	}
	for (Point& p : bombs) {
		if (p == points[0]) {
			return false;
		}
	}
	return true;
}

Point Snake::random_point(char symbol) {
	Point pt;
	int rx = rand() % pt.x_max();
	int ry = rand() % pt.y_max();
	pt.set_x(rx);
	pt.set_y(ry);
	pt.active = true;
	pt.set_symbol(symbol);

	if (pt == fruit) { return random_point(symbol); }
	if (pt == super_fruit) { return random_point(symbol); }
	for (Point& p : points) {
		if (p == pt) {
			return random_point(symbol);
		}
	}
	for (Point& p : bombs) {
		if (p == pt) {
			return random_point(symbol);
		}
	}
	return pt;
}

void Snake::check_fruit() {
	if (points[0] == fruit) {
		grow(FRUIT_SCORE);
		score += FRUIT_SCORE;
		fruit = random_point(FRUIT_SYMBOL);
		level_up(); //check if level up
	}

	if (points[0] == super_fruit) {
		grow(SUPER_FRUIT_SCORE);
		score += SUPER_FRUIT_SCORE;
		super_fruit.active = false;
		level_up(); //check if level up
	}

	if (counter >= FRUIT_RATE) {
		super_fruit.active = false;
	}
	else if (counter <= 0) {
		super_fruit.active = true;
		super_fruit = random_point(SUPER_FRUIT_SYMBOL);
	}
	
	if (super_fruit.active == true) {
		counter++;
	}
	else {
		counter--;
	}
}

void Snake::check_bombs() {
	if (bombs.size() == 0) { return; }
	if (bombs_counter >= BOMB_RATE) {
		if (bomb_idx >= bombs.size()) { bomb_idx = 0; }
		bombs[bomb_idx] = random_point(BOMB_SYMBOL);
		bomb_idx++;
		bombs_counter = 0;
	}
	//can be added for robustness espacially if bombs_num has a public access
	/* 
	if (bombs.size() < bombs_num) {
		for (int i = bombs.size(); i < bombs_num; i++) {
			bombs.push_back(random_point(BOMB_SYMBOL));
		}
	}
	*/
	bombs_counter++;
}

void Snake::level_up() {
	if (score >= win_score) {
		level++;
		win_score = LEVEL_SCORE_UP * level;
		bombs_num = LEVEL_BOMB_UP * level;
		system("cls");
		std::cout << "LEVEL UP: " << level << std::endl;
		system("pause");
		reset();
	}
}