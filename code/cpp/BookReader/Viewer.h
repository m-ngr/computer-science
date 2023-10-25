#pragma once
#include "Session.h"
const bool TERMINATE = true;
const bool CONTINUE = false;

class System {
public:
	static void run();
private:
	static bool login_menu();
	static bool signup_menu();
};

class UserView {
public:
	static bool user_menu();
private:
	static SessionSystem::SessionList my_sessions;
	static bool view_sessions();
	static bool view_books();
	static bool new_session();
	static bool open_session();
	static void read_session();
};

class AdminView : UserView{
public:
	static void admin_menu();
private:
	static bool add_books();
};