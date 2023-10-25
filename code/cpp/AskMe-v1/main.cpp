#include <iostream>
#include "AskSystem.h"


Database udb("users", User::INFO_LENGTH);
Database qdb("questions", Question::INFO_LENGTH);
User user;
const string EXIT_CODE = "0";
string msg;

void main_menu();
void login_menu();
void signup_menu();
void user_menu();
void ask_menu();
void ask_question();
void ask_thread();
void profile();
void my_question();
void feed();
void sent_questions();
void delete_question(); 
void answer_question();
bool delete_user();

int main() {
	main_menu();
}

void main_menu() {
	string cmd;
	int choice;
	const int CMD_NUM = 2;
	while (true) {
		system("cls");
		cout << "======================= ASK ME | Main =======================" << endl;
		cout << "1- Login \t 2- Signup" << endl;
		if (!msg.empty()) { cout << msg << endl; msg.clear(); }
		cout << "> ";

		cin >> cmd;
		if (!isNumber(cmd)) { msg = "Please Enter an Integer."; continue; }
		choice = stoi(cmd);
		if (choice > CMD_NUM) { msg = "Please Enter value from [1:" + to_string(CMD_NUM) + "]"; continue; }

		switch (choice) {
		case 1:
			login_menu();
			break;
		case 2:
			signup_menu();
			break;
		default:
			break;
		}
	}
}
void login_menu() {
	string cmd;
	while (true) {
		system("cls");
		cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		cout << "======================= ASK ME | Login =======================" << endl;
		cout << "Enter your username and password" << endl;
		if (!msg.empty()) { cout << msg << endl; msg.clear(); }

		cout << "Username> ";
		cin >> cmd;
		if (cmd == EXIT_CODE) { user.clear_info(); return; }
		if (!user.set_username(cmd)) { msg = "Invalid Username."; user.clear_info(); continue; }

		while (true) {
			cout << "Password> ";
			cin >> cmd;
			if (cmd == EXIT_CODE) { user.clear_info(); return; }
			if (user.set_password(cmd)) { break; }
			cout << "Invalid Password." << endl;
		}

		Database::Result result = udb.read(user.login_query());
		if(result.size() == 0){ msg = "Failed to Login, Check your username and password."; user.clear_info(); continue; }
		if(user.set_info(result[0]) == false) { msg = "Bad Info, Please contact us."; user.clear_info(); continue; }
		break;
	}
	user_menu();
}

void signup_menu() {
	string cmd;
	while (true) {
		system("cls");
		cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		cout << "======================= ASK ME | Signup =======================" << endl;
		cout << "Enter your account info ..." << endl;
		if (!msg.empty()) { cout << msg << endl; msg.clear(); }

		cout << "Username> ";
		cin >> cmd;
		if (cmd == EXIT_CODE) { user.clear_info(); return; }
		if (!user.set_username(cmd)) { msg = "Invalid Username."; user.clear_info(); continue; }

		while (true) {
			cout << "Password> ";
			cin >> cmd;
			if (cmd == EXIT_CODE) { user.clear_info(); return; }
			if (user.set_password(cmd)) { break; }
			cout << "Invalid Password." << endl;
		}

		while (true) {
			cout << "Email> ";
			cin >> cmd;
			if (cmd == EXIT_CODE) { user.clear_info(); return; }
			if (user.set_email(cmd)) { break; }
			cout << "Invalid Email Address." << endl;
		}

		while (true) {
			cout << "Full Name> ";
			cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			getline(cin, cmd);
			if (cmd == EXIT_CODE) { user.clear_info(); return; }
			if (user.set_name(cmd)) { break; }
			cout << "Invalid Full Name." << endl;
		}
		Database::Info info = user.get_info();
		if (!udb.write(info, user.signup_query())) { 
			msg = "Failed to Signup, your username and email must be unique."; user.clear_info(); continue; 
		}

		Database::Result result = udb.read(user.login_query());
		if (result.size() == 0) { msg = "Your account successfully signed up."; user.clear_info(); return; }
		if (user.set_info(result[0]) == false) { msg = "Your account successfully signed up."; user.clear_info(); return; }
		break;
	}
	user_menu();
}

void user_menu() {
	string cmd;
	int choice;
	const int CMD_NUM = 8;
	while (true) {
		system("cls");
		cout << "======================= ASK ME | " << user.get_name() <<" - Main "<< " =======================" << endl;
		cout << "1- Ask a Question \t 2- My Profile \t 3- My Questions \t 4- Questions I Send" << endl
			<< "5- Answer a Question \t 6- Delete a Question \t 7- Feed \t 8- Delete user \t 0- Logout"<<endl;
		if (!msg.empty()) { cout << msg << endl; msg.clear(); }
		cout << "> ";

		cin >> cmd;
		if (cmd == EXIT_CODE) { user.clear_info(); return; }
		if (!isNumber(cmd)) { msg = "Please Enter an Integer."; continue; }
		choice = stoi(cmd);
		if (choice > CMD_NUM) { msg = "Please Enter value from [1:" + to_string(CMD_NUM) + "]"; continue; }

		switch (choice) {
		case 1:
			ask_menu();
			break;
		case 2:
			profile();
			break;
		case 3:
			my_question();
			break;
		case 4:
			sent_questions();
			break;
		case 5:
			answer_question();
			break;
		case 6:
			delete_question();
			break;
		case 7:
			feed();
			break; 
		case 8:
			if (delete_user()) { return; }
			break;
		default:
			break;
		}
	}
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

void profile() {
	string cmd;
	system("cls");
	cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	cout << "======================= ASK ME | " << user.get_name() << " - Profile " << " =======================" << endl;
	if (!msg.empty()) { cout << msg << endl; msg.clear(); }

	print_question(user.profile_query(), true, false);

	cout << "Enter anything to Exit> ";
	cin >> cmd;
}

void my_question() {
	string cmd;
	system("cls");
	cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	cout << "======================= ASK ME | " << user.get_name() << " - My Questions " << " =======================" << endl;
	if (!msg.empty()) { cout << msg << endl; msg.clear(); }

	print_question(user.unanswered_query(), true, false);

	cout << "Enter anything to Exit> ";
	cin >> cmd;
}

void sent_questions() {
	string cmd;
	system("cls");
	cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	cout << "======================= ASK ME | " << user.get_name() << " - Questions sent " << " =======================" << endl;
	if (!msg.empty()) { cout << msg << endl; msg.clear(); }

	print_question(user.questions_sent_query(), false, true);

	cout << "Enter anything to Exit> ";
	cin >> cmd;
}

void feed() {
	string cmd;
	system("cls");
	cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	cout << "======================= ASK ME | " << user.get_name() << " - Feed " << " =======================" << endl;
	if (!msg.empty()) { cout << msg << endl; msg.clear(); }

	print_question(user.feed_query(), true, true);

	cout << "Enter anything to Exit> ";
	cin >> cmd;
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

void delete_question() {
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
		if (cmd == EXIT_CODE) { return; }
		if (!isNumber(cmd)) { msg = "Please Enter an Integer."; continue; }
		q_id = stoi(cmd);
		if(remove_question(q_id) == false){ msg = "No such question."; continue; }
		msg = "Question deleted successfully.";
		break;
	}
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

void answer_question() {
	string cmd;
	int q_id;
	while (true) {
		system("cls");
		cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		cout << "======================= ASK ME | " << user.get_name() << " - Answer Question " << " =======================" << endl;
		cout << "Enter the question id to answer or update an answer." << endl;
		if (!msg.empty()) { cout << msg << endl; msg.clear(); }
		cout << "Question ID> ";

		cin >> cmd;
		if (cmd == EXIT_CODE) { return; }
		if (!isNumber(cmd)) { msg = "Please Enter an Integer."; continue; }
		q_id = stoi(cmd);
		if (update_question(q_id) == false) {continue; }
		msg = "Question updated successfully.";
		break;
	}
}

bool delete_user() {
	string cmd;
	system("cls");
	cout << "Are you sure that you want to delete your account ?" << endl;
	while (true) {
		cout << "Delete? (y/n)> ";
		cin >> cmd;
		if (cmd == EXIT_CODE) { return false; }
		if (cmd == "y" || cmd == "yes" || cmd == "Y" || cmd == "YES") { break; }
		if (cmd == "n" || cmd == "no" || cmd == "N" || cmd == "NO") { return false; }
		cout << "Invalid Input, Please Enter (y/n)." << endl;
	}
	SimpleQuery sq(Question::TO_IDX,to_string(user.get_id()),true);
	qdb.remove(sq);
	if (!udb.remove(user.get_id())) { msg = "couldn't delete the account"; return false; }
	msg = user.get_username() + " was deleted successfully.";
	user.clear_info();
	return true;
}

void ask_menu() {
	string cmd;
	int choice;
	const int CMD_NUM = 2;
	while (true) {
		system("cls");
		cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		cout << "======================= ASK ME | " << user.get_name() << " - Ask a Question " << " =======================" << endl;
		cout << "1- Ask New Question \t 2- Ask In Thread" << endl;
		if (!msg.empty()) { cout << msg << endl; msg.clear(); }
		cout << "> ";

		cin >> cmd;
		if (cmd == EXIT_CODE) { return; }
		if (!isNumber(cmd)) { msg = "Please Enter an Integer."; continue; }
		choice = stoi(cmd);
		if (choice > CMD_NUM) { msg = "Please Enter value from [1:" + to_string(CMD_NUM) + "]"; continue; }

		switch (choice) {
		case 1:
			ask_question();
			break;
		case 2:
			ask_thread();
			break;
		default:
			break;
		}
		break;
	}
}

void ask_question() {
	string cmd;
	User to_user;
	Question question;
	while (true) {
		system("cls");
		cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		cout << "======================= ASK ME | " << user.get_name() << " - Ask New Question " << " =======================" << endl;
		cout << "Enter the question info..." << endl;
		if (!msg.empty()) { cout << msg << endl; msg.clear(); }
		//get : to_id
		cout << "To username> ";
		cin >> cmd;
		if (cmd == EXIT_CODE) { return; }
		if (!to_user.set_username(cmd)) { msg = "Invalid Username."; to_user.clear_info(); continue; }
		Database::Result to_result = udb.read(to_user.search_username_query());
		if (to_result.size() == 0) { msg = "No Such Username."; to_user.clear_info(); continue; }
		if (to_user.set_info(to_result[0]) == false) { msg = "This username data may be corrupted."; to_user.clear_info(); continue; }
		question.to_id = to_user.get_id();
		//get : anonymous
		if (to_user.get_anonymous()) {
			cout << "This User allows Asking Anonymously, Do you want to ask anonymously? (y/n)" << endl;
			while (true) {
				cout << "Ask Anonymously? (y/n)> ";
				cin >> cmd;
				if (cmd == EXIT_CODE) { return; }
				if (cmd == "y" || cmd == "yes" || cmd == "Y" || cmd == "YES") { question.anonymous = true; break; }
				if (cmd == "n" || cmd == "no" || cmd == "N" || cmd == "NO") { question.anonymous = false; break; }
				cout << "Invalid Input, Please Enter (y/n)." << endl;
			}
		}else {
			cout << "This User doesn't allow asking anonymously, your username will be visible!" << endl;
			question.anonymous = false;
		}
		//get : Question
		while (true) {
			cout << "Question> ";
			cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			getline(cin, cmd);
			if (cmd == EXIT_CODE) { return; }
			if (cmd.empty() == false) { question.question = cmd; break; }
			cout << "Please Enter A Question." << endl;
		}
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
}

void ask_thread() {
	string cmd;
	User to_user;
	Question question;
	Thread thread;
	while (true) {
		system("cls");
		cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		cout << "======================= ASK ME | " << user.get_name() << " - Ask In Thread " << " =======================" << endl;
		cout << "Enter the question info..." << endl;
		if (!msg.empty()) { cout << msg << endl; msg.clear(); }
		//get : Thread ID
		cout << "Thread ID> ";
		cin >> cmd;
		if (cmd == EXIT_CODE) { return; }
		if (!isNumber(cmd) || stoi(cmd) < 1) { msg = "Thread ID must be number larger than zero."; continue; }
		question.thread_id = stoi(cmd);
		Database::Result thread_result = qdb.read(question.thread_search_query(),true);
		if(thread_result.size() == 0){ msg = "No Such Thread."; question.clear_info(); continue; }
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
		Database::Info to_info = udb.read(thread.get_user_id());
		if (to_info.size() == 0) { msg = "This User is no longer exist."; to_user.clear_info(); continue; }
		if (to_user.set_info(to_info) == false) { msg = "This username data may be corrupted."; to_user.clear_info(); continue; }

		//get : anonymous
		if (to_user.get_anonymous()) {
			cout << "This User allows Asking Anonymously, Do you want to ask anonymously? (y/n)" << endl;
			while (true) {
				cout << "Ask Anonymously? (y/n)> ";
				cin >> cmd;
				if (cmd == EXIT_CODE) { return; }
				if (cmd == "y" || cmd == "yes" || cmd == "Y" || cmd == "YES") { question.anonymous = true; break; }
				if (cmd == "n" || cmd == "no" || cmd == "N" || cmd == "NO") { question.anonymous = false; break; }
				cout << "Invalid Input, Please Enter (y/n)." << endl;
			}
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
			if (cmd == EXIT_CODE) { return; }
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
}

string print_username(unsigned int u_id) {
	Database::Info info = udb.read(u_id);
	if (info.size() == 0) { return "DELETED USER"; }
	User u;
	u.set_info(info);
	return (u.get_name() + " (@" + u.get_username() + ")");
}