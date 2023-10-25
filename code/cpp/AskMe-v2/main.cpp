#include <iostream>
#include "AskSystem.h"
#include "User.h"
const string EXIT_CODE = "0";

Database qdb("questions", Question::INFO_LENGTH);

User user;
string msg;
//Dialog Functions
void main_menu();
bool login_menu();
bool signup_menu();
void user_menu();
bool ask_menu();
bool ask_question();
bool ask_thread();
bool profile();
bool my_question();
bool feed();
bool sent_questions();
bool delete_question();
bool answer_question();
bool delete_user();

int main() {
	main_menu();
}

void print_header(string title, string description = "", bool reset_input = true) {
	system("cls");
	if (reset_input) { cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); }
	cout << "======================= ASK ME | " << title << " =======================" << endl;
	if(!description.empty()){ cout << description << endl; }
	if (!msg.empty()) { cout << msg << endl; msg.clear(); }
}

int read_int(int max, int min = 0, string what = "") {
	string input;
	int number;
	while (true) {
		cout <<what<< "> ";
		cin >> input;
		if (!isNumber(input)) { cout << "Please Enter an Integer.\n"; continue; }
		number = stoi(input);
		if (number > max || number < min) { 
			cout << "Please Enter value from [" << min << ":" << max << "]\n"; continue;
		}
		return number;
	}
}
string read_string(string what = "",bool line = false, unsigned min = 0, unsigned max = 0) {
	string input;
	if (max != 0 && max < min) { max = min; }
	while (true) {
		cout <<what<< "> ";
		if (line) {
			cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			getline(cin, input);
		}else {
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
		cout << what<<" (y/n)> ";
		cin >> input;
		if (input == "y" || input == "yes" || input == "Y" || input == "YES") { return true; }
		if (input == "n" || input == "no" || input == "N" || input == "NO") { return false; }
		cout << "Invalid Input, Please Enter (y/n)." << endl;
	}
}

struct Function {
	Function(string f_name, bool (*f_ptr)()) :name{ f_name }, function{ f_ptr }{}
	string name;
	bool (*function)();
};

void print_menu(const vector<Function> &menu) {
	for (int i = 0; i < menu.size(); i++) {
		if (i != 0) { cout << "\t"; }
		cout << (i + 1) << "- " << menu[i].name;
	}
	cout << endl;
}

void invoker(string title,vector<Function>& menu) {
	int choice;
	while (true) {
		print_header(title,"" , false);
		print_menu(menu);
		choice = read_int(menu.size());
		if (to_string(choice) == EXIT_CODE) { return; }
		if (menu[choice - 1].function()) { return; }
	}
}

void main_menu() {
	vector<Function> menu;
	menu.push_back(Function("Login", &login_menu));
	menu.push_back(Function("Signup", &signup_menu));
	invoker("Main", menu);
}

bool login_menu() {
	string username, password;
	while (true) {
		print_header("Login", "Enter your username and password.");
		username = read_string("Username");
		if (username == EXIT_CODE) { return false; }
		password = read_string("Password");
		if (username == EXIT_CODE) { return false; }
		try {
			user.login(username, password);
			break;
		}catch (exception& e) {
			msg = e.what();
			user.clear_info();
		}
	}
	user_menu();
	return false;
}

bool signup_menu() {
	string username, password, email, name;
	while (true) {
		print_header("Signup", "Enter your account info ...",false);
		//read
		username = read_string("Username"); if (username == EXIT_CODE) { return false; }
		password = read_string("Password"); if (password == EXIT_CODE) { return false; }
		email = read_string("Email"); if (email == EXIT_CODE) { return false; }
		name = read_string("Name",true); if (name == EXIT_CODE) { return false; }
		//signup then login
		try {
			User::signup(username, password, email, name);
			user.login(username, password);
			break;
		}catch (exception& e) {
			msg = e.what();
			user.clear_info();
		}
	}
	user_menu();
	return false;
}

void user_menu() {
	vector<Function> menu;
	menu.push_back(Function("Ask a Question", &ask_menu));
	menu.push_back(Function("My Profile", &profile));
	menu.push_back(Function("My Questions", &my_question));
	menu.push_back(Function("Questions I Send", &sent_questions));
	menu.push_back(Function("Answer a Question", &answer_question));
	menu.push_back(Function("Delete a Question", &delete_question));
	menu.push_back(Function("Feed", &feed));
	menu.push_back(Function("Delete user", &delete_user));
	invoker((user.get_name() + " - Main"), menu);
}

template<typename QueryType>
void print_question(QueryType query, bool show_sender, bool show_recevier) {
	Database::Result result = qdb.read(query,true);
	if (result.size() == 0) { cout << "No Questions" << endl; }
	Question quest;
	ThreadSet threads;
	for (auto info : result) {
		quest.set_info(info);
		threads.insert(quest);
	}
	cout << threads.print(show_sender, show_recevier);
}

bool profile() {
	string cmd;
	print_header((user.get_name() + " - Profile"));
	print_question(user.profile_query(), true, false);
	cout << "Enter anything to Exit> ";
	cin >> cmd;
	return false;
}

bool my_question() {
	string cmd;
	print_header((user.get_name() + " - My Questions"));
	print_question(user.unanswered_query(), true, false);
	cout << "Enter anything to Exit> ";
	cin >> cmd;
	return false;
}

bool sent_questions() {
	string cmd;
	print_header((user.get_name() + " - Questions sent"));
	print_question(user.questions_sent_query(), false, true);
	cout << "Enter anything to Exit> ";
	cin >> cmd;
	return false;
}

bool feed() {
	string cmd;
	print_header((user.get_name() + " - Feed"));
	print_question(user.feed_query(), true, true);
	cout << "Enter anything to Exit> ";
	cin >> cmd;
	return false;
}

bool remove_question(unsigned int q_id){
	SimpleQuery sq1(Question::TO_IDX, to_string(user.get_id()), true);
	Query q1(false);
	q1.insert(sq1);

	SimpleQuery sq2(Question::ID_IDX, to_string(q_id), true);
	SimpleQuery sq3(Question::THREAD_IDX, to_string(q_id), true);
	Query q2(true);
	q2.insert(sq2);
	q2.insert(sq3);

	QuerySet qs(false);
	qs.insert(q1);
	qs.insert(q2);

	if (qdb.remove(qs).size() == 0) { return false; }
	return true;
}

bool delete_question() {
	string cmd;
	int q_id;
	while (true) {
		system("cls");
		cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		cout << "======================= ASK ME | " << user.get_name() << " - Delete Question " << " =======================" << endl;
		cout << "Note: the question and all sub-questions will be deleted !"<<endl ;
		if (!msg.empty()) { cout << msg << endl; msg.clear(); }
		cout << "Question ID> ";

		cin >> cmd;
		if (cmd == EXIT_CODE) { return false; }
		if (!isNumber(cmd)) { msg = "Please Enter an Integer."; continue; }
		q_id = stoi(cmd);
		if(remove_question(q_id) == false){ msg = "No such question."; continue; }
		msg = "Question deleted successfully.";
		break;
	}
	return false;
}

bool update_question(unsigned int q_id) {
	string cmd;
	Database::Info info = qdb.read(q_id);
	if (info.size() == 0) { msg = "No such question."; return false; }
	Question quest;
	quest.set_info(info);
	if(quest.to_id != user.get_id()){ msg = "No such question for you to answer."; return false; }

	cout << "Question : " << quest.question << endl;
	if (quest.answer.empty()) { 
		cout << "[No Answer]" << endl; 
		cout << "Enter your Answer> ";
	}else {
		cout << "Answer : " << quest.answer << endl;
		cout << "Update your Answer> ";
	}
	//get answer
	while (true) {
		cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
		getline(cin, cmd);
		if (cmd == EXIT_CODE) { return false; }
		if (cmd.empty() == false) { quest.answer = cmd; break; }
		cout << "Please Enter an Answer." << endl;
		cout << "Answer> ";
	}
	info = quest.get_info();
	if (qdb.update(info) == false) { msg = "couldn't update your answer."; return false; }
	return true;
}

bool answer_question() {
	int q_id;
	while (true) {
		print_header((user.get_name() + " - Answer Question"), "Enter the question id to answer or update an answer.");
		q_id = read_int(INT_MAX, 0, "Question ID");
		if (to_string(q_id) == EXIT_CODE) { return false; }
		if (update_question(q_id) == false) {continue; }
		msg = "Question updated successfully.";
		break;
	}
	return false;
}

bool delete_user() {
	string cmd;
	system("cls");
	cout << "Are you sure that you want to delete your account ?" << endl;
	if (read_bool("Delete?") == false) { return false; }
	SimpleQuery sq(Question::TO_IDX,to_string(user.get_id()),true);
	qdb.remove(sq);
	if (user.remove()) { 
		msg = user.get_username() + " was deleted successfully.";
	}else {
		msg = user.get_username() + " couldn't be deleted.";
	}
	return true;
}

bool ask_menu() {
	vector<Function> menu;
	menu.push_back(Function("Ask New Question", &ask_question));
	menu.push_back(Function("Ask In Thread", &ask_thread));
	invoker((user.get_name() + " - Ask a Question"), menu);
	return false;
}

bool ask_question() {
	string cmd;
	User to_user;
	Question question;
	while (true) {
		print_header((user.get_name() + " - Ask New Question"), "Enter the question info...");
		//get : to_id
		cmd = read_string("To username");
		if (cmd == EXIT_CODE) { return false; }
		try {
			to_user = User::search(cmd);
		}catch (exception& e) {
			msg = e.what();
			continue;
		}
		question.to_id = to_user.get_id();
		//get : anonymous
		if (to_user.get_anonymous()) {
			cout << "This User allows Asking Anonymously, Do you want to ask anonymously? (y/n)" << endl;
			question.anonymous = read_bool("Ask Anonymously?");
		}else {
			cout << "This User doesn't allow asking anonymously, your username will be visible!" << endl;
			question.anonymous = false;
		}
		//get : Question
		question.question = read_string("Question",true,1);
		//Filling question info
		question.thread_id = 0;
		question.thread_number = 0;
		question.answer = "";
		question.from_id = user.get_id();
		//writing the question
		Database::Info info = question.get_info();
		if (!qdb.write(info)) {
			msg = "Failed to Ask, Try again."; 
			question.clear_info(); to_user.clear_info(); continue;
		}
		msg = "Question sent successfully.";
		break;
	}
	return false;
}

bool ask_thread() {
	string cmd;
	unsigned t_id;
	User to_user;
	Question question;
	while (true) {
		print_header((user.get_name() + " - Ask In Thread"), "Enter the question info...");
		//get : Thread ID
		t_id = read_int(0,1,"Thread ID");
		if (to_string(t_id) == EXIT_CODE) { return false; }
		Thread thread(t_id);

		Database::Result thread_result = qdb.read(thread.search_query(),true);
		if(thread_result.size() == 0){ msg = "No Such Thread."; continue; }
		for (auto& info : thread_result) {
			question.set_info(info);
			thread.insert(question);
		}
		//Get Question Info
		question.clear_info();
		question.thread_id = thread.get_id();
		question.thread_number = thread.last_thread() + 1;
		question.from_id = user.get_id();
		question.to_id = thread.get_user_id();
		question.answer = "";
		//Get user Info
		try {
			to_user = User::search(thread.get_user_id());
		}catch (exception& e) {
			msg = e.what();
			continue;
		}
		//get : anonymous
		if (to_user.get_anonymous()) {
			cout << "This User allows Asking Anonymously, Do you want to ask anonymously? (y/n)" << endl;
			question.anonymous = read_bool("Ask Anonymously?");
		}
		else {
			cout << "This User doesn't allow asking anonymously, your username will be visible!" << endl;
			question.anonymous = false;
		}
		//get : Question
		while (true) {
			cout << "Question> ";
			cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			getline(cin, cmd);
			if (cmd == EXIT_CODE) { return false; }
			if (cmd.empty() == false) { question.question = cmd; break; }
			cout << "Please Enter A Question." << endl;
		}
		//writing the question
		Database::Info info = question.get_info();
		if (!qdb.write(info)) {
			msg = "Failed to Ask, Try again.";
			question.clear_info(); to_user.clear_info(); continue;
		}
		msg = "Question sent successfully.";
		break;
	}
	return false;
}