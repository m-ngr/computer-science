#pragma once
#include "DatabaseSystem.h"

class User {
public:
	static const unsigned int INFO_LENGTH{ 7 };
	User() {};
	Database::Info get_info()const;
	bool set_info(const Database::Info& info);
	bool validate_info(const Database::Info& info) const;
	void clear_info();
	bool empty()const;

	Query login_query() const;
	Query signup_query() const;
	Query search_query()const;

	//setters
	bool set_username(const string& value);
	bool set_password(const string& value);
	bool set_email(const string& value);
	bool set_name(const string& value);
	void set_admin(bool value) { admin = value; }
	void set_confirm(bool value) { confirm = value; }
	//getters
	unsigned int get_id() const { return id; }
	string get_username()const { return username; }
	string get_password()const { return password; }
	string get_email()const { return email; }
	string get_name()const { return name; }
	bool get_admin()const { return admin; }
	bool get_confirm()const { return confirm; }
private:
	unsigned int id{};
	string username{};
	string password{};
	string email{};
	string name{};
	bool admin{false};
	bool confirm{false};

	static const unsigned int MIN_LENGTH{ 4 };
	

	static const unsigned int ID_IDX{ 0 };
	static const unsigned int USERNAME_IDX{ 1 };
	static const unsigned int PASSWORD_IDX{ 2 };
	static const unsigned int EMAIL_IDX{ 3 };
	static const unsigned int NAME_IDX{ 4 };
	static const unsigned int ADMIN_IDX{ 5 };
	static const unsigned int CONFIRM_IDX{ 6 };

	bool validate_info() const;
	bool validate_username(const string& value)const;
	bool validate_password(const string& value)const;
	bool validate_email(const string& value)const;
	bool validate_name(const string& value)const;
};

class UserSystem {
public:
	static bool signup(const string& username, const string& password, const string& email, const string& name, bool admin);
	static bool login(const string& username, const string& password);
	static void logout();

	static bool is_logged() { return logged; }
	static bool is_confirmed() { return user.get_confirm(); }
	static bool is_admin();
	static unsigned user_id() { return user.get_id(); }
	static string get_name(bool show_username = false);
	static bool confirm_user(unsigned int u_id);
	static bool confirm_user(const string& username);
	static bool confirm_user_email(const string& email);

	static User search(const string& username);
	static User search(unsigned int u_id);
	static User search_email(const string& email);
private:
	static Database database;
	static User user;
	static bool logged;

	static bool confirm_user(User& new_user);
	static User search(User& find_user);
};
