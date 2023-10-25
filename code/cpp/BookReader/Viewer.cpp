#include "Viewer.h"
SessionSystem::SessionList UserView::my_sessions;

void System::run() {
	vector<Function> menu;
	menu.push_back(Function("Login", &login_menu));
	menu.push_back(Function("Signup", &signup_menu));
	view_menu("Main", menu);
}
bool System::login_menu() {
	string username, password;
	while (UserSystem::is_logged() == false) {
		print_header("Login", "Enter your username and password.");
		username = read_string("Username");
		if (username == EXIT_CODE) { return CONTINUE; }
		password = read_string("Password");
		if (username == EXIT_CODE) { return CONTINUE; }
		try {
			UserSystem::login(username, password);
			if (UserSystem::is_logged() == false) { msg = "Failed to login"; }
		}catch (exception& e) {
			msg = e.what();
			UserSystem::logout();
		}
	}

	if (UserSystem::is_admin()) {
		AdminView::admin_menu();
	}else {
		UserView::user_menu();
	}
	UserSystem::logout();
	return CONTINUE;
}
bool System::signup_menu() {
	string username, password, email, name;
	bool admin = false, signedup = false;
	while (!signedup) {
		print_header("Signup", "Enter your account info ...", false);
		//read
		username = read_string("Username"); if (username == EXIT_CODE) { return CONTINUE; }
		password = read_string("Password"); if (password == EXIT_CODE) { return CONTINUE; }
		email = read_string("Email"); if (email == EXIT_CODE) { return CONTINUE; }
		name = read_string("Name", true); if (name == EXIT_CODE) { return CONTINUE; }
		admin = read_bool("Signup as admin?");
		//signup then login
		try {
			signedup = UserSystem::signup(username, password, email, name, admin);
			if (signedup) { UserSystem::login(username, password); }
		}catch (exception& e) {
			msg = e.what();
			UserSystem::logout();
		}
		if (!signedup) { msg = "Failed to signup"; }
	}
	if (UserSystem::is_admin()) {
		AdminView::admin_menu();
	}
	else {
		UserView::user_menu();
	}
	return CONTINUE;
}

bool UserView::user_menu() {
	my_sessions = SessionSystem::user_sessions();
	vector<Function> menu;
	menu.push_back(Function("My Sessions", &view_sessions));
	menu.push_back(Function("View Books", &view_books));
	menu.push_back(Function("New Session", &new_session));
	menu.push_back(Function("Open Session", &open_session));
	view_menu("Main", menu);
	return CONTINUE;
}
bool UserView::view_sessions() {
	string cmd;
	print_header("My Sessions");
	if (my_sessions.size() == 0) { cout << "No Sessions\n"; }
	for (int i = 0; i < my_sessions.size(); i++) {
		cout << "Session [" << (i + 1) << "]: " << print_book(my_sessions[i].get_book()) 
			<<"; Last opened: "<<print_date(my_sessions[i].get_date())<< endl;
	}
	cout << "Enter anything to Exit> ";
	cin >> cmd;
	return CONTINUE;
}
bool UserView::view_books() {
	string cmd;
	print_header("All Books");
	BookSystem::BookList list = BookSystem::all_books();
	if (list.size() == 0) { cout << "No Books\n"; }
	for (Book &book : list) {
		cout << print_book(book.get_id()) <<"; Added by "<< print_user(book.get_user())<< endl;
	}
	cout << "Enter anything to Exit> ";
	cin >> cmd;
	return CONTINUE;
}
bool UserView::new_session(){
	unsigned int book_id;
	bool added = false;
	while (!added) {
		print_header("New Session", "Enter Book Id.", false);
		//read
		book_id = read_int(INT_MAX, 0, "Book id");
		if (to_string(book_id) == EXIT_CODE) { return CONTINUE; }

		bool found = false;
		for (Session& session : my_sessions) {
			if (session.get_book() == book_id) {
				msg = "You Already have a session in this book"; found = true; break;
			}
		}

		if (found) { continue; }
		try {
			added = SessionSystem::create_session(book_id);
		}catch (exception& e) {
			msg = e.what();
			continue;
		}
		if (!added) { msg = "Failed to create new session"; continue; }
		msg = "The Session was created successfully.";
	}
	my_sessions = SessionSystem::user_sessions();
	return CONTINUE;
}
bool UserView::open_session(){
	unsigned int number;
	bool opened = false;
	if (my_sessions.size() == 0) { msg = "No Sessions for you to open."; }
	while (!opened) {
		print_header("Open Session", "Enter Session Number (Not id).", false);
		//read
		if (my_sessions.size() == 0) { read_string("Enter any to exit"); return CONTINUE; }
		number = read_int(my_sessions.size(), 0, "Session");
		if (to_string(number) == EXIT_CODE) { return CONTINUE; }
		try {
			opened = SessionSystem::open_session(my_sessions[number-1].get_id());
		}catch (exception& e) {
			msg = e.what();
			continue;
		}
		if (!opened) { msg = "Failed to open session"; continue; }
	}
	read_session();
	return CONTINUE;
}
void UserView::read_session(){
	string input;
	string content = SessionSystem::read_page();
	while (true) {
		print_header("Session");
		cout << content <<endl;
		cout << "(+) next page, (-) pervious page, (number) goto page \n";
		input = read_string("Goto");
		if (input == EXIT_CODE) { SessionSystem::close_session(); return; }
		if (input == "+") {
			content = SessionSystem::read_page(SessionSystem::get_page() + 1);
		}
		else if (input == "-") {
			content = SessionSystem::read_page(SessionSystem::get_page() - 1);
		}
		else if (isNumber(input)) {
			content = SessionSystem::read_page(stoi(input));
		}
	}
}

void AdminView::admin_menu(){
	vector<Function> menu;
	menu.push_back(Function("User Menu", &user_menu));
	menu.push_back(Function("Add Book", &add_books));
	view_menu("Admin Menu", menu);
}

bool AdminView::add_books(){
	string isbn, name;
	unsigned int pages;
	bool added = false;
	while (!added) {
		print_header("Add Book", "Enter Book info.", false);
		//read
		isbn = read_string("ISBN"); if (isbn == EXIT_CODE) { return CONTINUE; }
		name = read_string("Name", true); if (name == EXIT_CODE) { return CONTINUE; }
		pages = read_int(INT_MAX,1,"Pages");
		//signup then login
		try {
			added = BookSystem::add_book(name, isbn, pages);
		}catch (exception& e) {
			msg = e.what();
		}
		if (!added) { msg = "Failed to add the book"; continue; }
		msg = "The Book was added successfully.";
	}
	return CONTINUE;
}
