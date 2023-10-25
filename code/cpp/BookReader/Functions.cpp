#include "Functions.h"
#include "User.h"
#include "Book.h"

string msg{};
bool isNumber(const string& str) {
	if (str.size() == 0) { return false; }
	for (const char& ch : str) {
		if (!isdigit(ch)) { return false; }
	}
	return true;
}
string lower_string(string str) {
	for (char& ch : str) {
		ch = tolower(ch);
	}
	return str;
}
vector<string> split_string(const string& str) {
	vector<string> words;
	string token;
	stringstream strstm(str);
	while (getline(strstm, token, ' ')) {
		if (!token.empty()) { words.push_back(token); }
	}
	return words;
}
bool search_string(const string& content, const string& search) {
	vector<string> search_words = split_string(lower_string(search));
	string token;
	stringstream strstm(lower_string(content));
	while (getline(strstm, token, ' ')) {
		if (!token.empty()) {
			for (const string& word : search_words) {
				if (word == token) { return true; }
			}
		}
	}
	return false;
}
time_t get_date() {
	time_t date; time(&date);
	return date;
}
string print_date(time_t date) {
	char buf[26];
	ctime_s(buf, 26, &date);
	return string(buf);
}

void print_message() {
	if (!msg.empty()) { cout << msg << endl; msg.clear(); }
}
void print_header(string title, string description, bool reset_input) {
	system("cls");
	if (reset_input) { cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); }
	if (UserSystem::is_logged()) { title = UserSystem::get_name(false) + " - " + title; }
	cout << "======================= Book Reader | " << title << " =======================" << endl;
	if (!description.empty()) { cout << description << endl; }
	print_message();
}

int read_int(int max, int min, string what) {
	string input;
	int number;
	while (true) {
		cout << what << "> ";
		cin >> input;
		if (!isNumber(input)) { cout << "Please Enter an Integer.\n"; continue; }
		number = stoi(input);
		if (number > max || number < min) {
			cout << "Please Enter value from [" << min << ":" << max << "]\n"; continue;
		}
		return number;
	}
}
string read_string(string what, bool line, unsigned min, unsigned max) {
	string input;
	if (max != 0 && max < min) { max = min; }
	while (true) {
		cout << what << "> ";
		if (line) {
			cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			getline(cin, input);
		}
		else {
			cin >> input;
		}
		if (input.size() < min) { cout << "Enter String of length " << min << " at least."; continue; }
		if (max != 0 && input.size() > max) { cout << "Enter String of length " << max << " maximum."; continue; }
		return input;
	}
}
bool read_bool(string what) {
	string input;
	while (true) {
		cout << what << " (y/n)> ";
		cin >> input;
		if (input == "y" || input == "yes" || input == "Y" || input == "YES") { return true; }
		if (input == "n" || input == "no" || input == "N" || input == "NO") { return false; }
		cout << "Invalid Input, Please Enter (y/n)." << endl;
	}
}

void print_menu(const vector<Function>& menu) {
	for (int i = 0; i < menu.size(); i++) {
		if (i != 0) { cout << "\t"; }
		cout << (i + 1) << "- " << menu[i].name;
	}
	cout << endl;
}
void view_menu(string title, vector<Function>& menu, string description) {
	int choice;
	while (true) {
		print_header(title, description, false);
		print_menu(menu);
		choice = read_int(menu.size());
		if (to_string(choice) == EXIT_CODE) { return; }
		if (menu[choice - 1].function()) { return; }
	}
}

string print_book(unsigned int book_id) {
	try {
		Book book = BookSystem::search(book_id);
		if(book.empty()) { return "DELETED BOOK"; }
		string str;
		str = "[" + to_string(book.get_id()) + "] " + book.get_name() +
			" (" + to_string(book.get_pages()) + " Pages), ISBN: " + book.get_isbn();
		return str;
	}catch (...) {
		return "DELETED BOOK";
	}
}

string print_user(unsigned int user_id) {
	try {
		User user = UserSystem::search(user_id);
		if (user.empty()) { return "DELETED USER"; }
		string str;
		str = user.get_name() + "(@" + user.get_username() + ")";
		return str;
	}
	catch (...) {
		return "DELETED USER";
	}
}