#pragma once
#include "User.h"

class Book {
public:
	static const unsigned int INFO_LENGTH{ 5 };
	Book() {};
	Database::Info get_info()const;
	bool set_info(const Database::Info& info);
	bool validate_info(const Database::Info& info) const;
	void clear_info();
	bool empty()const;
	
	SimpleQuery isbn_query()const;
	SimpleQuery user_query()const;

	//setters
	bool set_isbn(const string& value);
	bool set_name(const string& value);
	bool set_pages(unsigned int value);
	bool set_user(unsigned int value);
	//getters
	unsigned int get_id() const { return id; }
	unsigned int get_user() const { return user_id; }
	string get_isbn()const { return isbn; }
	string get_name()const { return name; }
	unsigned int get_pages()const { return pages; }
private:
	unsigned int id{};
	unsigned int user_id{};
	string isbn{};
	string name{};
	unsigned int pages{};

	static const unsigned int MIN_LENGTH{ 4 };

	static const unsigned int ID_IDX{ 0 };
	static const unsigned int USER_IDX{ 1 };
	static const unsigned int ISBN_IDX{ 2 };
	static const unsigned int NAME_IDX{ 3 };
	static const unsigned int PAGE_IDX{ 4 };

	bool validate_info() const;
	bool validate_isbn(const string& value)const;
	bool validate_name(const string& value)const;
};
class BookSystem {
public:
	typedef vector<Book> BookList;
	static bool add_book(const string& name, const string& isbn, unsigned int pages);
	static bool remove_book(const string& isbn);
	static bool remove_book(unsigned int b_id);
	static BookList search_name(const string& name);
	static Book search(const string& isbn);
	static Book search(unsigned int b_id);
	static BookList admin_books();
	static BookList all_books();

	static bool open_book(unsigned int book_id);
	static bool open_book(const string& isbn);
	static unsigned get_pages() { return active_book.get_pages(); }
	static unsigned is_active() { return (!active_book.empty()); }
	static void close_book() { active_book.clear_info(); }
	static string read_page(unsigned int page);
private:
	static Database database;
	static Book active_book;
};