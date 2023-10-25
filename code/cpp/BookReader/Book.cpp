#include "Book.h"

Database BookSystem::database("books", Book::INFO_LENGTH);
Book BookSystem::active_book;

Database::Info Book::get_info() const {
	Database::Info info(INFO_LENGTH);
	info[ID_IDX] = to_string(id);
	info[USER_IDX] = to_string(user_id);
	info[ISBN_IDX] = isbn;
	info[NAME_IDX] = name;
	info[PAGE_IDX] = to_string(pages);
	return info;
}
bool Book::set_info(const Database::Info& info) {
	if (info.size() != INFO_LENGTH || validate_info(info) == false) { return false; }
	id = stoi(info[ID_IDX]);
	user_id = stoi(info[USER_IDX]);
	isbn = info[ISBN_IDX];
	name = info[NAME_IDX];
	pages = stoi(info[PAGE_IDX]);
	return true;
}
void Book::clear_info() {
	id = 0;
	user_id = 0;
	isbn = "";
	name = "";
	pages = 0;
}
bool Book::empty()const {
	if (id == 0||pages == 0 ||user_id == 0) { return true; }
	return (isbn.empty() || name.empty());
}

//also used for search
SimpleQuery Book::isbn_query()const {
	SimpleQuery sq(ISBN_IDX, isbn, true);
	return sq;
}
SimpleQuery Book::user_query()const {
	SimpleQuery sq(USER_IDX, to_string(user_id), true);
	return sq;
}

bool Book::set_isbn(const string& value) {
	if (validate_isbn(value)) {
		isbn = value;
		return true;
	}
	return false;
}
bool Book::set_pages( unsigned int value) {
	if (value > 0) {
		pages = value;
		return true;
	}
	return false;
}
bool Book::set_user(unsigned int value) {
	if (value > 0) {
		user_id = value;
		return true;
	}
	return false;
}
bool Book::set_name(const string& value) {
	if (validate_name(value)) {
		name = value;
		return true;
	}
	return false;
}

bool Book::validate_info(const Database::Info& info) const {
	if (!isNumber(info[ID_IDX])) { return false; }
	if (!isNumber(info[USER_IDX])) { return false; }
	if (!isNumber(info[PAGE_IDX])) { return false; }
	return (validate_isbn(info[ISBN_IDX]) && validate_name(info[NAME_IDX]));
}
bool Book::validate_info() const {
	if (pages < 1) { return false; }
	if (user_id < 1) { return false; }
	return (validate_isbn(isbn) && validate_name(name));
}
bool Book::validate_isbn(const string& value) const {
	if (value.size() < MIN_LENGTH) { return false; }
	//should validate special characters that are not allowed in folder names
	for (const char& ch : value) { if (ch == Database::SYM) { return false; } }
	return true;
}
bool Book::validate_name(const string& value) const {
	if (value.size() < MIN_LENGTH) { return false; }
	for (const char& ch : value) { if (ch == Database::SYM) { return false; } }
	return true;
}

//=================================== Book Sys =============================================================
bool BookSystem::add_book(const string& name, const string& isbn, unsigned int pages) {
	if(UserSystem::is_admin() == false){ throw exception("Only Admins Can Add Books."); }
	Book new_book;
	if (new_book.set_name(name) == false) { throw exception("Invalid Name."); }
	if (new_book.set_isbn(isbn) == false) { throw exception("Invalid ISBN."); }
	if (new_book.set_pages(pages) == false) { throw exception("Invalid Number of Pages."); }
	new_book.set_user(UserSystem::user_id());
	Database::Info info = new_book.get_info();
	return database.write(info, new_book.isbn_query());
}
bool BookSystem::remove_book(const string& isbn) {
	if (UserSystem::is_admin() == false) { throw exception("Only Admins Can Remove Books."); }
	Book del_book;
	if (del_book.set_isbn(isbn) == false) { throw exception("Invalid ISBN."); }
	if (database.remove(del_book.isbn_query()).size() == 0) { return false; }
	return true;
}
bool BookSystem::remove_book(unsigned int b_id) {
	if (UserSystem::is_admin() == false) { throw exception("Only Admins Can Remove Books."); }
	return database.remove(b_id);
}
Book BookSystem::search(const string& isbn) {
	Book find_book;
	if (find_book.set_isbn(isbn) == false) { return find_book; } //empty user
	Database::Result result = database.read(find_book.isbn_query());
	find_book.clear_info();
	if (result.empty()) { return find_book; } //empty user
	find_book.set_info(result[0]);
	return find_book;
}
Book BookSystem::search(unsigned int b_id) {
	Book find_book;
	Database::Info info = database.read(b_id);
	if (info.empty()) { return find_book; } //empty user
	find_book.set_info(info);
	return find_book;
}
BookSystem::BookList BookSystem::search_name(const string& name)  {
	BookList book_list;
	if (name.empty()) { return book_list; } //empty list
	SimpleQuery all_query(0,"0", false); //id != 0
	Database::Result result = database.read(all_query, true); //read all books
	if (result.empty()) { return book_list; } //empty list : no books
	Book book;
	for (Database::Info& info : result) {
		if (book.set_info(info) == false) { continue; }
		if (search_string(book.get_name(), name)) {
			book_list.push_back(book);
		}
	}
	return book_list;
}
BookSystem::BookList BookSystem::admin_books()  {
	BookList book_list;
	if (UserSystem::is_admin() == false) { return book_list; } //empty list
	Book book;
	book.set_user(UserSystem::user_id());
	Database::Result result = database.read(book.user_query(), true);
	for (const Database::Info& info : result) {
		if (book.set_info(info)) {
			book_list.push_back(book);
		}
	}
	return book_list;
}
BookSystem::BookList BookSystem::all_books()  {
	BookList book_list;
	Book book;
	SimpleQuery all_query(0, "0", false); // id != 0
	Database::Result result = database.read(all_query, true);
	for (const Database::Info& info : result) {
		if (book.set_info(info)) {
			book_list.push_back(book);
		}
	}
	return book_list;
}
bool BookSystem::open_book(unsigned int book_id) {
	active_book = search(book_id);
	return (!active_book.empty());
}
bool BookSystem::open_book(const string& isbn) {
	active_book = search(isbn);
	return (!active_book.empty());
}
string BookSystem::read_page(unsigned int page) {
	if (UserSystem::is_logged() == false) { throw exception("Please Login to continue."); }
	if (is_active() == false) { throw exception("No Active Book to view."); }
	string content = "Page " + to_string(page) + "\n" + "This page is blank.";
	return content;
}