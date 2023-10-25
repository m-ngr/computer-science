#include "AskSystem.h"
string print_username(unsigned int u_id);

Database::Info User::get_info() {
	Database::Info info(INFO_LENGTH);
	info[ID_IDX] = to_string(id);
	info[USERNAME_IDX] = username;
	info[PASSWORD_IDX] = password;
	info[EMAIL_IDX] = email;
	info[NAME_IDX] = name;
	info[ANON_IDX] = to_string(allow_anon);
	return info;
}

bool User::set_info(const Database::Info &info) {
	if (info.size() != INFO_LENGTH || validate_info(info) == false) { return false; }
	id = stoi(info[ID_IDX]);
	username = info[USERNAME_IDX];
	password = info[PASSWORD_IDX];
	email = info[EMAIL_IDX];
	name = info[NAME_IDX];
	allow_anon = stoi(info[ANON_IDX]);
	return true;
}

bool User::set_username(string& value) {
	if (validate_username(value)) {
		username = value;
		return true;
	}
	return false;
}
bool User::set_password(string& value) {
	if (validate_password(value)) {
		password = value;
		return true;
	}
	return false;
}
bool User::set_email(string& value) {
	if (validate_email(value)) {
		email = value;
		return true;
	}
	return false;
}
bool User::set_name(string& value) {
	if (validate_name(value)) {
		name = value;
		return true;
	}
	return false;
}

bool User::validate_info(const Database::Info &info) {
	if (!isNumber(info[ID_IDX])) { return false; }
	if (!isNumber(info[ANON_IDX])) { return false; }
	return (validate_username(info[USERNAME_IDX]) && validate_password(info[PASSWORD_IDX]) 
		&& validate_email(info[EMAIL_IDX]) && validate_name(info[NAME_IDX]));
}

bool User::validate_info() {
	return (validate_username() && validate_password() && validate_email() && validate_name());
}

bool User::validate_username(string value) {
	if (value.size() == 0) { value = username; }
	if (value.size() < MIN_LENGTH) { return false; }
	if (!isalpha(value[0])) { return false; }
	for (const char& ch : value) { if (ch == Database::SYM) { return false; } }
	return true;
}
bool User::validate_password(string value) {
	if (value.size() == 0) { value = password; }
	if (value.size() < MIN_LENGTH) { return false; }
	for (const char& ch : value) { if (ch == Database::SYM) { return false; } }
	return true;
}
bool User::validate_email(string value) {
	if (value.size() == 0) { value = email; }
	if (value.size() < (2 * MIN_LENGTH)) { return false; }
	if (int(value.find('@')) == -1 || value.find('@') != value.rfind('@')) { return false; }
	for (const char& ch : value) { if (ch == Database::SYM) { return false; } }
	return true;
}
bool User::validate_name(string value) {
	if (value.size() == 0) { value = name; }
	if (value.size() < MIN_LENGTH) { return false; }
	for (const char& ch : value) { if (ch == Database::SYM) { return false; } }
	return true;
}
void User::clear_info() {
	id = 0;
	username = "";
	password = "";
	email = "";
	name = "";
	allow_anon = true;
}

Query User::login_query() {
	SimpleQuery sq1(USERNAME_IDX, username, true);
	SimpleQuery sq2(PASSWORD_IDX, password, true);
	Query q(false);
	q.insert(sq1);
	q.insert(sq2);
	return q;
}

Query User::signup_query() {
	SimpleQuery sq1(USERNAME_IDX, username, true);
	SimpleQuery sq2(EMAIL_IDX, email, true);
	Query q(true);
	q.insert(sq1);
	q.insert(sq2);
	return q;
}

SimpleQuery User::search_username_query() {
	SimpleQuery sq(USERNAME_IDX, username, true);
	return sq;
}

Query User::profile_query() {
	SimpleQuery sq1(Question::TO_IDX, to_string(id), true);
	SimpleQuery sq2(Question::ANSWER_IDX, "", false);
	Query q(false);
	q.insert(sq1);
	q.insert(sq2);
	return q;
}

Query User::unanswered_query() {
	SimpleQuery sq1(Question::TO_IDX, to_string(id), true);
	SimpleQuery sq2(Question::ANSWER_IDX, "", true);
	Query q(false);
	q.insert(sq1);
	q.insert(sq2);
	return q;
}

SimpleQuery User::questions_sent_query() {
	SimpleQuery sq(Question::FROM_IDX, to_string(id), true);
	return sq;
}

Query User::feed_query() {
	SimpleQuery sq1(Question::TO_IDX, to_string(id), false);
	SimpleQuery sq2(Question::FROM_IDX, to_string(id), false);
	SimpleQuery sq3(Question::ANSWER_IDX, "", false);
	Query q(false);
	q.insert(sq1);
	q.insert(sq2);
	q.insert(sq3);
	return q;
}

//=============================Q===============================
Database::Info Question::get_info()const {
	Database::Info info(INFO_LENGTH);
	info[ID_IDX] = to_string(id);
	info[THREAD_IDX] = to_string(thread_id);
	info[THREAD_NUM_IDX] = to_string(thread_number);
	info[QUESTION_IDX] = question;
	info[ANSWER_IDX] = answer;
	info[FROM_IDX] = to_string(from_id);
	info[TO_IDX] = to_string(to_id);
	info[ANONYMOUS_IDX] = to_string(anonymous);
	return info;
}

bool Question::set_info(const Database::Info& info) {
	if (info.size() != INFO_LENGTH || validate_info(info) == false) { return false; }
	id = stoi(info[ID_IDX]);
	thread_id = stoi(info[THREAD_IDX]);
	thread_number = stoi(info[THREAD_NUM_IDX]);
	question = info[QUESTION_IDX];
	answer = info[ANSWER_IDX];
	from_id = stoi(info[FROM_IDX]);
	to_id = stoi(info[TO_IDX]);
	anonymous = stoi(info[ANONYMOUS_IDX]);

	if (thread_id == 0) { thread_id = id; }

	return true;
}

bool Question::validate_info(const Database::Info& info)const {
	if (!isNumber(info[ID_IDX])) { return false; }
	if (!isNumber(info[THREAD_IDX])) { return false; }
	if (!isNumber(info[THREAD_NUM_IDX])) { return false; }
	if (!isNumber(info[FROM_IDX])) { return false; }
	if (!isNumber(info[TO_IDX])) { return false; }
	if (!isNumber(info[ANONYMOUS_IDX])) { return false; }
	return true;
}

void Question::clear_info() {
	id = 0;
	thread_id = 0;
	thread_number = 0;
	question = "";
	answer = "";
	from_id = 0;
	to_id = 0;
	anonymous = false;
}



string Question::print(bool show_sender, bool show_recevier, bool is_thread)const {
	string str;
	string sender, recevier;

	if (is_thread) { str = "    | "; }

	if (!show_sender) { sender = "You"; }
	else if (show_sender && anonymous) { sender = "Anonymous"; }
	else { sender = print_username(from_id); }

	if (show_recevier) { recevier = print_username(to_id); }

	str = str + sender + " asked " + recevier ;
	if (!show_sender && anonymous) { str += " Anonymously"; }
	str = str + " [" + to_string(id) + "] :\n";

	if (is_thread) { str = str + "    | "; }

	str = str + "\t" + "Question : " + question + "\n";
	if (is_thread) { str = str + "    | "; }
	if (answer.empty()) { str = str + "\t" + "[No Answer]"+"\n"; }
	else {
		str = str + "\t" + "Answer : " + answer + "\n";
	}
	return str;
}

bool Question::operator<(const Question& rhs) const{
	return thread_number < rhs.thread_number;
}

Query Question::ask_query() {
	if (thread_id == 0) {return {};}
	SimpleQuery sq1(THREAD_IDX, to_string(thread_id), true);
	SimpleQuery sq2(THREAD_NUM_IDX, to_string(thread_number), true);
	Query q(false);
	q.insert(sq1);
	q.insert(sq2);
	return q;
}

Query Question::thread_search_query() {
	if (thread_id == 0) { return {}; }
	SimpleQuery sq1(THREAD_IDX, to_string(thread_id), true);
	SimpleQuery sq2(ID_IDX, to_string(thread_id), true);
	Query q(true);
	q.insert(sq1);
	q.insert(sq2);
	return q;
}

//==========================TH=============================
Thread::Thread(const Question& question) {
	if (thread.insert(question).second == true) {
		id = question.thread_id;
		to_id = question.to_id;
	}
}

bool Thread::insert(Question& question) {
	if (id == 0 || to_id == 0 || thread.size() == 0) {
		id = question.thread_id;
		to_id = question.to_id;
	}else if (id != question.thread_id) { return false; }

	return thread.insert(question).second;
}

unsigned int Thread::last_thread() {
	if (thread.empty()) { return 0; }
	return thread.rbegin()->thread_number;
}

string Thread::print(bool show_sender, bool show_recevier)const {
	string str;
	bool is_tread = false;
	if (thread.size() > 1) {
		str = "Thread [" + to_string(id) + "]";
		if (show_recevier) { str = str + " To " + print_username(to_id); }
		str = str + "\n";
		is_tread = true;
	}
	int i = 0;
	for (const Question& question : thread) {
		i++;
		str += question.print(show_sender, show_recevier, is_tread);
		if (thread.size() == 1) {
			str = str + "------------------------------------------------------------------------------------------------" + "\n";
		}else if (thread.size() > i) {
			str = str + "    |-------------------------------------------------------------------------------------------" + "\n";
		}
	}
	if (thread.size() > 1) {
		str = str + "================================================================================================" + "\n";
	}
	return str;
}

void Thread::clear() {
	thread.clear();
	id = 0;
	to_id = 0;
}
//================================TS==========================
bool ThreadSet::insert(Question& question) {
	return threads[question.thread_id].insert(question);
}
bool ThreadSet::insert(Thread& thread) {
	if (threads.find(thread.get_id()) == threads.end()) {
		threads[thread.get_id()] = thread;
		return true;
	}
	return false;
}
string ThreadSet::print(bool show_sender, bool show_recevier)const {
	string str;
	for (const auto &thread : threads) {
		str += thread.second.print(show_sender,show_recevier);
	}
	return str;
}
void ThreadSet::clear() {
	threads.clear();
}