#pragma once
#include "DatabaseSystem.h"
#include <set>
#include<map>

class User {
public:
	static const unsigned int INFO_LENGTH{ 6 };

	User() {};

	Database::Info get_info();
	bool set_info(const Database::Info & info);
	bool validate_info(const Database::Info& info);
	bool validate_info();
	void clear_info();

	Query login_query();
	Query signup_query();
	SimpleQuery search_username_query();
	Query profile_query();
	Query unanswered_query();
	SimpleQuery questions_sent_query();
	Query feed_query();

	bool set_username(string &value);
	bool set_password(string& value);
	bool set_email(string& value);
	bool set_name(string& value);
	bool set_anonymous(bool allow) { allow_anon = allow; }

	unsigned int get_id() const { return id; }
	string get_username()const { return username; }
	string get_password()const { return password; }
	string get_email()const { return email; }
	string get_name()const { return name; }
	bool get_anonymous()const { return allow_anon; }

private:
	unsigned int id{};
	string username{};
	string password{};
	string email{};
	string name{};
	bool allow_anon{ true };

	static const unsigned int MIN_LENGTH{ 4 };

	static const unsigned int ID_IDX{ 0 };
	static const unsigned int USERNAME_IDX{ 1 };
	static const unsigned int PASSWORD_IDX{ 2 };
	static const unsigned int EMAIL_IDX{ 3 };
	static const unsigned int NAME_IDX{ 4 };
	static const unsigned int ANON_IDX{ 5 };

	bool validate_username( string value = "");
	bool validate_password( string value = "");
	bool validate_email( string value = "");
	bool validate_name( string value = "");
};

class Question {
public:
	static const unsigned int INFO_LENGTH{ 8 };
	Question() {};
	Database::Info get_info()const;
	bool set_info(const Database::Info& info);
	bool validate_info(const Database::Info& info)const;
	void clear_info();
	string print(bool show_sender, bool show_recevier, bool is_thread /*= false*/)const;
	
	Query ask_query();
	Query thread_search_query();

	unsigned int id{};
	unsigned int thread_id{};
	unsigned int thread_number{};
	string question{};
	string answer{};
	unsigned int from_id{};
	unsigned int to_id{};
	bool anonymous{false};

	static const unsigned int ID_IDX{ 0 };
	static const unsigned int THREAD_IDX{ 1 };
	static const unsigned int THREAD_NUM_IDX{ 2 };
	static const unsigned int QUESTION_IDX{ 3 };
	static const unsigned int ANSWER_IDX{ 4 };
	static const unsigned int FROM_IDX{ 5 };
	static const unsigned int TO_IDX{ 6 };
	static const unsigned int ANONYMOUS_IDX{ 7 };

	bool operator<(const Question& rhs) const;

};



class Thread {
public:
	Thread(const Question& question);
	Thread(unsigned int thread_id = 0) : id{ thread_id } {};
	bool insert(Question& question);
	unsigned int last_thread();

	string print(bool show_sender, bool show_recevier)const;
	unsigned int get_id()const { return id; }
	unsigned int get_user_id()const { return to_id; }
	void clear();
private:
	unsigned int id{};
	unsigned int to_id{};
	set<Question> thread;
};

class ThreadSet {
public:
	bool insert(Question& question);
	bool insert(Thread& thread);
	void clear();
	string print(bool show_sender, bool show_recevier)const;
private:
	map<unsigned int,Thread> threads;
};