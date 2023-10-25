#include "User.h"
#include "AskSystem.h"

const string User::PATH{ "users" };
Database User::database(PATH, INFO_LENGTH);

Database::Info User::get_info() const {
	Database::Info info(INFO_LENGTH);
	info[ID_IDX] = to_string(id);
	info[USERNAME_IDX] = username;
	info[PASSWORD_IDX] = password;
	info[EMAIL_IDX] = email;
	info[NAME_IDX] = name;
	info[ANON_IDX] = to_string(allow_anon);
	return info;
}
bool User::set_info(const Database::Info& info) {
	if (info.size() != INFO_LENGTH || validate_info(info) == false) { return false; }
	id = stoi(info[ID_IDX]);
	username = info[USERNAME_IDX];
	password = info[PASSWORD_IDX];
	email = info[EMAIL_IDX];
	name = info[NAME_IDX];
	allow_anon = stoi(info[ANON_IDX]);
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
bool User::remove() {
	bool removed = database.remove(get_id());
	clear_info();
	return removed;
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

User User::search(string username) {
	User user;
	if (!user.set_username(username)) { throw exception("Invalid Username."); }
	Database::Result result = database.read(user.search_username_query());
	if (result.empty()) { throw exception("No Such Username.");}
	if (user.set_info(result[0]) == false) { throw exception("This username data may be corrupted."); }
	return user;
}
User User::search(unsigned u_id) {
	User user;
	Database::Info info = database.read(u_id);
	if (info.empty()) { throw exception("This User is no longer exist.");}
	if (user.set_info(info) == false) { throw exception("This username data may be corrupted."); }
	return user;
}
void User::login(string username, string password){
	if (set_username(username) == false) { throw exception("Invalid Username."); }
	if (set_password(password) == false) { throw exception("Invalid Password."); }
	Database::Result result = database.read(login_query());
	if (result.empty()) { throw exception("Failed to Login, Check your username and password.");}
	if (set_info(result[0]) == false) { throw exception("Bad Info, Please contact us.");}
}
void User::signup(string username, string password, string email, string name) {
	User user;
	if (user.set_username(username) == false) { throw exception("Invalid Username."); }
	if (user.set_password(password) == false) { throw exception("Invalid Password."); }
	if (user.set_email(email) == false) { throw exception("Invalid Email Address."); }
	if (user.set_name(name) == false) { throw exception("Invalid Full Name."); }
	Database::Info info = user.get_info();
	if (!database.write(info, user.signup_query())) {
		throw exception("Failed to Signup, your username and email must be unique.");
	}
}
string User::print_name(unsigned int u_id) {
	try {
		User user = search(u_id);
		return (user.get_name() + " (@" + user.get_username() + ")");
	}
	catch (...) {
		return "DELETED USER";
	}
}

bool User::validate_info(const Database::Info& info) const {
	if (!isNumber(info[ID_IDX])) { return false; }
	if (!isNumber(info[ANON_IDX])) { return false; }
	return (validate_username(info[USERNAME_IDX]) && validate_password(info[PASSWORD_IDX])
		&& validate_email(info[EMAIL_IDX]) && validate_name(info[NAME_IDX]));
}
bool User::validate_info() const {
	return (validate_username(username) && validate_password(password) && validate_email(email) && validate_name(name));
}
bool User::validate_username(const string& value) {
	if (value.size() < MIN_LENGTH) { return false; }
	if (!isalpha(value[0])) { return false; }
	for (const char& ch : value) { if (ch == Database::SYM) { return false; } }
	return true;
}
bool User::validate_password(const string& value){
	if (value.size() < MIN_LENGTH) { return false; }
	for (const char& ch : value) { if (ch == Database::SYM) { return false; } }
	return true;
}
bool User::validate_email(const string& value) {
	if (value.size() < (2 * MIN_LENGTH)) { return false; }
	if (int(value.find('@')) == -1 || value.find('@') != value.rfind('@')) { return false; }
	for (const char& ch : value) { if (ch == Database::SYM) { return false; } }
	return true;
}
bool User::validate_name(const string& value) {
	if (value.size() < MIN_LENGTH) { return false; }
	for (const char& ch : value) { if (ch == Database::SYM) { return false; } }
	return true;
}

Query User::login_query() const {
	SimpleQuery sq1(USERNAME_IDX, username, true);
	SimpleQuery sq2(PASSWORD_IDX, password, true);
	Query q(false);
	q.insert(sq1);
	q.insert(sq2);
	return q;
}
Query User::signup_query() const{
	SimpleQuery sq1(USERNAME_IDX, username, true);
	SimpleQuery sq2(EMAIL_IDX, email, true);
	Query q(true);
	q.insert(sq1);
	q.insert(sq2);
	return q;
}
SimpleQuery User::search_username_query() const{
	SimpleQuery sq(USERNAME_IDX, username, true);
	return sq;
}
Query User::profile_query() const {
	SimpleQuery sq1(Question::TO_IDX, to_string(id), true);
	SimpleQuery sq2(Question::ANSWER_IDX, "", false);
	Query q(false);
	q.insert(sq1);
	q.insert(sq2);
	return q;
}
Query User::unanswered_query() const {
	SimpleQuery sq1(Question::TO_IDX, to_string(id), true);
	SimpleQuery sq2(Question::ANSWER_IDX, "", true);
	Query q(false);
	q.insert(sq1);
	q.insert(sq2);
	return q;
}
SimpleQuery User::questions_sent_query() const {
	SimpleQuery sq(Question::FROM_IDX, to_string(id), true);
	return sq;
}
Query User::feed_query() const{
	SimpleQuery sq1(Question::TO_IDX, to_string(id), false);
	SimpleQuery sq2(Question::FROM_IDX, to_string(id), false);
	SimpleQuery sq3(Question::ANSWER_IDX, "", false);
	Query q(false);
	q.insert(sq1);
	q.insert(sq2);
	q.insert(sq3);
	return q;
}