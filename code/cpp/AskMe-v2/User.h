#pragma once
#include "DatabaseSystem.h"

class User {
public:
	User() {};

	Database::Info get_info()const;
	bool set_info(const Database::Info& info);
	bool validate_info(const Database::Info& info) const;
	void clear_info();
	bool remove();//need to remove questions as well

	Query login_query() const;
	Query signup_query() const;
	SimpleQuery search_username_query()const;

	Query profile_query()const;
	Query unanswered_query()const;
	SimpleQuery questions_sent_query()const;
	Query feed_query()const;
	//setters
	bool set_username(string& value);
	bool set_password(string& value);
	bool set_email(string& value);
	bool set_name(string& value);
	bool set_anonymous(bool allow) { allow_anon = allow; }
	//getters
	unsigned int get_id() const { return id; }
	string get_username()const { return username; }
	string get_password()const { return password; }//Not used
	string get_email()const { return email; }//Not used
	string get_name()const { return name; }
	bool get_anonymous()const { return allow_anon; }
	//operations
	void login(string username, string password);
	static User search(string username);
	static User search(unsigned u_id);
	static void signup(string username, string password, string email, string name);
	static string print_name(unsigned int u_id);
private:
	unsigned int id{};
	string username{};
	string password{};
	string email{};
	string name{};
	bool allow_anon{ true };

	static Database database;
	static const unsigned int MIN_LENGTH{ 4 };
	static const unsigned int INFO_LENGTH{ 6 };
	static const string PATH;

	static const unsigned int ID_IDX{ 0 };
	static const unsigned int USERNAME_IDX{ 1 };
	static const unsigned int PASSWORD_IDX{ 2 };
	static const unsigned int EMAIL_IDX{ 3 };
	static const unsigned int NAME_IDX{ 4 };
	static const unsigned int ANON_IDX{ 5 };

	bool validate_info() const;
	static bool validate_username(const string& value);
	static bool validate_password(const string& value);
	static bool validate_email(const string& value);
	static bool validate_name(const string& value);
};