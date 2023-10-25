#pragma once
#include "Book.h"

class Session {
public:
	static const unsigned int INFO_LENGTH{ 5 };
	Session() {};
	Database::Info get_info()const;
	bool set_info(const Database::Info& info);
	bool validate_info(const Database::Info& info) const;
	void clear_info();
	bool empty()const;

	SimpleQuery book_query()const;
	SimpleQuery user_query()const;
	Query unique_query()const;

	//setters
	bool set_user(unsigned int value);
	bool set_book(unsigned int value);
	bool set_page(unsigned int value);
	void set_date(time_t value) { date = value; }
	//getters
	unsigned int get_id() const { return id; }
	unsigned int get_user() const { return user_id; }
	unsigned int get_book() const { return book_id; }
	unsigned int get_page() const { return page; }
	time_t get_date() const { return date; }
private:
	unsigned int id{};
	unsigned int user_id{};
	unsigned int book_id{};
	unsigned int page{};
	time_t date{};

	static const unsigned int ID_IDX{ 0 };
	static const unsigned int USER_IDX{ 1 };
	static const unsigned int BOOK_IDX{ 2 };
	static const unsigned int PAGE_IDX{ 3 };
	static const unsigned int DATE_IDX{ 4 };
};
class SessionSystem {
public:
	typedef vector<Session> SessionList;
	static bool create_session(unsigned int book_id);
	static bool open_session(unsigned int session_id);
	static void close_session();
	static bool remove_session(unsigned int session_id);
	static SessionList user_sessions();
	static string read_page() ;
	static string read_page(unsigned int page);
	static bool is_active(){ return active; }
	static unsigned int get_page() { return session.get_page(); }
private:
	static Database database;
	static Session session;
	static bool active;
};