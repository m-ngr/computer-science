#include "Session.h"

Database SessionSystem::database("sessions", Session::INFO_LENGTH);
Session SessionSystem::session;
bool SessionSystem::active{false};

Database::Info Session::get_info() const {
	Database::Info info(INFO_LENGTH);
	info[ID_IDX] = to_string(id);
	info[USER_IDX] = to_string(user_id);
	info[BOOK_IDX] = to_string(book_id);
	info[PAGE_IDX] = to_string(page);
	info[DATE_IDX] = to_string(date);
	return info;
}
bool Session::set_info(const Database::Info& info) {
	if (info.size() != INFO_LENGTH || validate_info(info) == false) { return false; }
	id = stoi(info[ID_IDX]);
	user_id = stoi(info[USER_IDX]);
	book_id = stoi(info[BOOK_IDX]);
	page = stoi(info[PAGE_IDX]);
	date = stoll(info[DATE_IDX]);
	return true;
}
void Session::clear_info() {
	id = 0;
	user_id = 0;
	book_id = 0;
	page = 0;
	date = 0;
}
bool Session::empty()const {
	if (id == 0 || page == 0 || user_id == 0 || book_id == 0) { return true; }
	return false;
}

//also used for search
SimpleQuery Session::book_query()const {
	SimpleQuery sq(BOOK_IDX, to_string(book_id), true);
	return sq;
}
SimpleQuery Session::user_query()const {
	SimpleQuery sq(USER_IDX, to_string(user_id), true);
	return sq;
}
Query Session::unique_query()const {
	Query q(false);
	q.insert(user_query());
	q.insert(book_query());
	return q;
}
bool Session::set_page(unsigned int value) {
	if (value > 0) {
		page = value;
		return true;
	}
	return false;
}
bool Session::set_user(unsigned int value) {
	if (value > 0) {
		user_id = value;
		return true;
	}
	return false;
}
bool Session::set_book(unsigned int value) {
	if (value > 0) {
		book_id = value;
		return true;
	}
	return false;
}

bool Session::validate_info(const Database::Info& info) const {
	if (!isNumber(info[ID_IDX])) { return false; }
	if (!isNumber(info[USER_IDX])) { return false; }
	if (!isNumber(info[PAGE_IDX])) { return false; }
	if (!isNumber(info[BOOK_IDX])) { return false; }
	if (!isNumber(info[DATE_IDX])) { return false; }
	return true;
}

//=================================== Session Sys =============================================================
bool SessionSystem::create_session(unsigned int book_id) {
	if (UserSystem::is_logged() == false) { throw exception("Please Login to continue."); }
	Book book = BookSystem::search(book_id);
	if (book.empty()) { throw exception("Book Doesn't Exist."); }
	Session new_session;
	if (new_session.set_book(book_id) == false) { throw exception("Invalid Book."); }
	new_session.set_page(1);
	new_session.set_date(get_date());
	new_session.set_user(UserSystem::user_id());
	Database::Info info = new_session.get_info();
	return database.write(info, new_session.unique_query());
}
bool SessionSystem::open_session(unsigned int session_id) {
	if (UserSystem::is_logged() == false) { throw exception("Please Login to continue."); }
	Database::Info info = database.read(session_id);
	active = session.set_info(info);
	if (active) { BookSystem::open_book(session.get_book()); }
	return active;
}
void SessionSystem::close_session() {
	bool updated = true;
	if (is_active()) {
		session.set_date(get_date()); //update time
		updated = database.update(session.get_info());
	}
	BookSystem::close_book();
	session.clear_info();
	active = false;
	if(updated == false){ throw exception("Failed to update sesstion."); }
}
bool SessionSystem::remove_session(unsigned int session_id)  {
	if (UserSystem::is_logged() == false) { throw exception("Please Login to continue."); }
	if (session_id == session.get_id()) { close_session(); }
	return database.remove(session_id);
}
SessionSystem::SessionList SessionSystem::user_sessions()  {
	if (UserSystem::is_logged() == false) { throw exception("Please Login to continue."); }
	SessionList s_list;
	Session user_session;
	user_session.set_user(UserSystem::user_id());
	Database::Result result = database.read(user_session.user_query(), true);
	for (const Database::Info& info : result) {
		if (user_session.set_info(info)) {
			s_list.push_back(user_session);
		}
	}
	return s_list;

}
string SessionSystem::read_page() {
	if (UserSystem::is_logged() == false) { throw exception("Please Login to continue."); }
	if (is_active() == false) { throw exception("No Active Session to view."); }
	return BookSystem::read_page(session.get_page());
}
string SessionSystem::read_page(unsigned int page) {
	if (page == 0) { page = 1; }
	if (page > BookSystem::get_pages()) { page = BookSystem::get_pages(); }
	session.set_page(page);
	return read_page();
}