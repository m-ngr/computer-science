#pragma once
#include "DatabaseSystem.h"
#include <set>
#include<map>

class Question {
public:
	bool operator<(const Question& rhs) const;
	static const unsigned int INFO_LENGTH{ 8 };
	static const string PATH;

	Question() {};
	Database::Info get_info()const;
	bool set_info(const Database::Info& info);
	bool validate_info(const Database::Info& info)const;
	void clear_info();
	string print(bool show_sender, bool show_recevier, bool is_thread)const;

	Query ask_query()const;

	static const unsigned int ID_IDX{ 0 };
	static const unsigned int THREAD_IDX{ 1 };
	static const unsigned int THREAD_NUM_IDX{ 2 };
	static const unsigned int QUESTION_IDX{ 3 };
	static const unsigned int ANSWER_IDX{ 4 };
	static const unsigned int FROM_IDX{ 5 };
	static const unsigned int TO_IDX{ 6 };
	static const unsigned int ANONYMOUS_IDX{ 7 };
	unsigned int id{};
	unsigned int thread_id{};
	unsigned int thread_number{};
	string question{};
	string answer{};
	unsigned int from_id{};
	unsigned int to_id{};
	bool anonymous{ false };

	static Database database;
};

class Thread {
public:
	Thread(const Question& question);
	Thread(unsigned int thread_id = 0) : id{ thread_id } {};
	bool insert(Question& question);
	unsigned int last_thread();
	Query search_query()const;
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
