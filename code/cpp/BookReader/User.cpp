#include "User.h"
#include "Session.h"

Database UserSystem::database("users", User::INFO_LENGTH);
User UserSystem::user;
bool UserSystem::logged{false};

Database::Info User::get_info() const {
	Database::Info info(INFO_LENGTH);
	info[ID_IDX] = to_string(id);
	info[USERNAME_IDX] = username;
	info[PASSWORD_IDX] = password;
	info[EMAIL_IDX] = email;
	info[NAME_IDX] = name;
	info[ADMIN_IDX] = to_string(admin);
	info[CONFIRM_IDX] = to_string(confirm);
	return info;
}
bool User::set_info(const Database::Info& info) {
	if (info.size() != INFO_LENGTH || validate_info(info) == false) { return false; }
	id = stoi(info[ID_IDX]);
	username = info[USERNAME_IDX];
	password = info[PASSWORD_IDX];
	email = info[EMAIL_IDX];
	name = info[NAME_IDX];
	admin = bool(stoi(info[ADMIN_IDX]));
	confirm = bool(stoi(info[CONFIRM_IDX]));
	return true;
}
void User::clear_info() {
	id = 0;
	username = "";
	password = "";
	email = "";
	name = "";
	admin = false;
	confirm = false;
}
bool User::empty()const {
	if (id == 0) { return true; }
	return (username.empty() || password.empty() || email.empty() || name.empty());
}
bool User::set_username(const string& value) {
	if (validate_username(value)) {
		username = value;
		return true;
	}
	return false;
}
bool User::set_password(const string& value) {
	if (validate_password(value)) {
		password = value;
		return true;
	}
	return false;
}
bool User::set_email(const string& value) {
	if (validate_email(value)) {
		email = value;
		return true;
	}
	return false;
}
bool User::set_name(const string& value) {
	if (validate_name(value)) {
		name = value;
		return true;
	}
	return false;
}

bool User::validate_info(const Database::Info& info) const {
	if (!isNumber(info[ID_IDX])) { return false; }
	if (!isNumber(info[ADMIN_IDX])) { return false; }
	if (!isNumber(info[CONFIRM_IDX])) { return false; }
	return (validate_username(info[USERNAME_IDX]) && validate_password(info[PASSWORD_IDX])
		&& validate_email(info[EMAIL_IDX]) && validate_name(info[NAME_IDX]));
}
bool User::validate_info() const {
	return (validate_username(username) && validate_password(password) && validate_email(email) && validate_name(name));
}
bool User::validate_username(const string& value) const{
	if (value.size() < MIN_LENGTH) { return false; }
	if (!isalpha(value[0])) { return false; }
	for (const char& ch : value) { if (ch == Database::SYM) { return false; } }
	return true;
}
bool User::validate_password(const string& value) const{
	if (value.size() < MIN_LENGTH) { return false; }
	for (const char& ch : value) { if (ch == Database::SYM) { return false; } }
	return true;
}
bool User::validate_email(const string& value) const {
	if (value.size() < (2 * MIN_LENGTH)) { return false; }
	if (int(value.find('@')) == -1 || value.find('@') != value.rfind('@')) { return false; }
	for (const char& ch : value) { if (ch == Database::SYM) { return false; } }
	return true;
}
bool User::validate_name(const string& value) const{
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
Query User::signup_query() const {
	SimpleQuery sq1(USERNAME_IDX, username, true);
	SimpleQuery sq2(EMAIL_IDX, email, true);
	Query q(true);
	q.insert(sq1);
	q.insert(sq2);
	return q;
}
Query User::search_query() const {
	//should i check for empty values??
	SimpleQuery sq1(USERNAME_IDX, username, true);
	SimpleQuery sq2(EMAIL_IDX, email, true);
	Query q(true);
	q.insert(sq1);
	q.insert(sq2);
	return q;
}
//=================================== User Sys =============================================================
bool UserSystem::signup(const string& username, const string& password, const string& email, const string& name, bool admin) {
	User new_user;
	if (new_user.set_username(username) == false) { throw exception("Invalid Username."); }
	if (new_user.set_password(password) == false) { throw exception("Invalid Password."); }
	if (new_user.set_email(email) == false) { throw exception("Invalid Email."); }
	if (new_user.set_name(name) == false) { throw exception("Invalid Name."); }
	new_user.set_admin(admin);
	new_user.set_confirm(false);
	Database::Info info = new_user.get_info();
	return database.write(info, new_user.signup_query());
}
bool UserSystem::login(const string& username, const string& password) {
	if (user.set_username(username) == false) { throw exception("Invalid Username."); }
	if (user.set_password(password) == false) { throw exception("Invalid Password."); }
	Database::Result result = database.read(user.login_query());
	if (result.empty()) { return false; }
	logged =  user.set_info(result[0]);
	return logged;
}
void UserSystem::logout(){
	SessionSystem::close_session();
	user.clear_info();
	logged = false;
}

bool UserSystem::is_admin() {
	return (is_logged() && is_confirmed() && user.get_admin());
}
bool UserSystem::confirm_user(unsigned int u_id) {
	if(is_logged() == false){ throw exception("Please Login to continue."); }
	if (user.get_admin() == false) { throw exception("Only Admins can confirm new users."); }
	User new_user = search(u_id);
	return confirm_user(new_user);
}
bool UserSystem::confirm_user(const string& username) {
	if (is_logged() == false) { throw exception("Please Login to continue."); }
	if (user.get_admin() == false) { throw exception("Only Admins can confirm new users."); }
	User new_user = search(username);
	return confirm_user(new_user);
}
bool UserSystem::confirm_user_email(const string& email) {
	if (is_logged() == false) { throw exception("Please Login to continue."); }
	if (user.get_admin() == false) { throw exception("Only Admins can confirm new users."); }
	User new_user = search_email(email);
	return confirm_user(new_user);
}
bool UserSystem::confirm_user(User& new_user) {
	if (new_user.empty()) { throw exception("User Not Found."); }
	if (new_user.get_confirm()) { throw exception("Already Confirmed."); }
	new_user.set_confirm(true);
	return database.update(new_user.get_info());
}

//find user be username or email
User UserSystem::search(User& find_user) {
	Database::Result result = database.read(find_user.search_query());
	find_user.clear_info();
	if (result.empty()) { return find_user; } //empty user
	find_user.set_info(result[0]);
	return find_user;
}
User UserSystem::search(unsigned int u_id) {
	User find_user;
	Database::Info info = database.read(u_id);
	if (info.empty()) { return find_user; } //empty user
	find_user.set_info(info);
	return find_user;
}
User UserSystem::search(const string& username){
	User find_user;
	if (find_user.set_username(username) == false) { return find_user; } //empty user
	return search(find_user);
}
User UserSystem::search_email(const string& email) {
	User find_user;
	if (find_user.set_email(email) == false) { return find_user; } //empty user
	return search(find_user);
}
string UserSystem::get_name(bool show_username) {
	if (is_logged() == false) { return ""; }
	if (show_username == false) { return user.get_name(); }
	return (user.get_name() + " (@" + user.get_username() + ")");
}