#include "DatabaseSystem.h"

bool isNumber(const string& str) {
	if (str.size() == 0) { return false; }
	for (const char& ch : str) {
		if (!isdigit(ch)) { return false; }
	}
	return true;
}

//==================SQ=================
bool SimpleQuery::evaluate(const Database::Info& info)const {
	if (info.size() < Database::MIN_INFO) { return false; }
	if (idx >= info.size()) { return false; }
	return ((identifier == info[idx]) == state);
}
//===================Q=================
bool Query::evaluate(const Database::Info& info)const {
	if (qList.size() == 0) { return false; }
	for (const SimpleQuery& q : qList) {
		if (any == q.evaluate(info)) { return any; }
	}
	return (!any);
}
//===================QS================
bool QuerySet::evaluate(const Database::Info& info)const {
	if (qSet.size() == 0) { return false; }
	for (const Query& q : qSet) {
		if (any == q.evaluate(info)) { return any; }
	}
	return (!any);
}
//==================DB=================
Database::Database(const string file_path, const unsigned int minimum_info) {
	if (file_path.size() == 0) { throw exception("Empty Database Path"); }
	path = file_path + ".txt";
	if (minimum_info < MIN_INFO) {
		min_info = MIN_INFO;
	}else {
		min_info = minimum_info;
	}
	load_id();
}

string Database::encode(const Info& info) {
	if (info.size() < min_info) { return {}; }
	string info_encoded;
	for (size_t i = 0; i < info.size(); i++) {
		if (i != 0) { info_encoded += SYM; }
		info_encoded += info[i];
	}
	return info_encoded;
}

Database::Info Database::decode(string info_encoded) {
	string token;
	stringstream strstm(info_encoded);
	Info info;
	while (getline(strstm, token, SYM)) {
		info.push_back(token);
	}
	return info;
}

bool Database::validate(const Info& info) {
	if (info.size() < min_info) { return false; }
	if (!isNumber(info[0])) { return false; }
	return true;
}

void Database::load_id() {
	id = 1;
	string info_encoded;
	Info info;
	database.open(path, READ);
	if (!database.fail()) {
		while (getline(database, info_encoded)) {
			info = decode(info_encoded);
			if (!validate(info)) { continue; }
			if (stoi(info[0]) >= id) { id = stoi(info[0]) + 1; }
		}
	}
	database.close();
}

bool Database::update(Info& info) {
	if (!validate(info)) { return false; }
	if (stoi(info[0]) >= id) { return false; }

	SimpleQuery query(0,info[0],true);
	if (remove(query).size() == 0) { return false; }

	string info_encoded = encode(info);
	database.open(path, APPEND);
	database << info_encoded << endl;
	if (database.fail()) { database.close(); return false; }
	database.close();
	return true;
}

unsigned int Database::update_all(unsigned int index, const string value) {
	if (index == 0) { return 0; }
	string info_encoded;
	Info info;
	unsigned int result = 0;
	string content;
	database.open(path, READ);
	if (database.fail()) { database.close(); return result; }
	while (getline(database, info_encoded)) {
		info = decode(info_encoded);
		if (!validate(info)) { continue; }
		if (index < info.size()) { info[index] = value; result++; }
		content += encode(info) + '\n';
	}
	database.close();
	database.open(path, OVERWRITE);
	database << content;
	if (database.fail()) { result = 0; }
	database.close();
	return result;
}

bool Database::write(Info& info) {
	if (info.size() < min_info) { return false; }
	info[0] = to_string(id);
	string info_encoded = encode(info);
	database.open(path, APPEND);
	database << endl << info_encoded;
	if (database.fail()) { database.close(); return false; }
	database.close();
	id++;
	return true;
}

Database::Info Database::read(unsigned int q_id) {
	if (q_id >= id) { return {}; }
	SimpleQuery id_query(0, to_string(q_id), true);
	Result qr = read(id_query, false);
	if (qr.size() == 0) { return {}; }
	return qr[0];
}

bool Database::remove(unsigned int q_id) {
	if (q_id >= id) { return false; }
	SimpleQuery id_query(0, to_string(q_id), true);
	if (remove(id_query).size() == 0) { return false; }
	return true;
}

template<typename QueryType> bool Database::write(Info& info, const QueryType& query) {
	if (info.size() < min_info) { return false; }
	if (exist(query)) { return false; }
	return write(info);
}
//template variations must be specified
template bool Database::write(Info& info, const SimpleQuery& query);
template bool Database::write(Info& info, const Query& query);
template bool Database::write(Info& info, const QuerySet& query);

template<typename QueryType> bool Database::update(Info& info, const QueryType& query) {
	if (!validate(info)) { return false; }
	if (stoi(info[0]) >= id) { return false; }

	bool updated = true;
	SimpleQuery id_query(0, info[0], true);
	Result org_info = remove(id_query);
	if (org_info.size() == 0) { return false; }

	if (exist(query)) { info = org_info[0]; updated = false; }

	string info_encoded = encode(info);
	database.open(path, APPEND);
	database << info_encoded << endl;
	if (database.fail()) { database.close(); return false; }
	database.close();
	return updated;
}
//template variations must be specified
template bool Database::update(Info& info, const SimpleQuery& query);
template bool Database::update(Info& info, const Query& query);
template bool Database::update(Info& info, const QuerySet& query);

template<typename QueryType> Database::Result Database::update_all(unsigned int index, const string value, const QueryType& query) {
	if (index == 0) { return {}; }
	string info_encoded;
	Info info;
	Result result;
	string content;
	database.open(path, READ);
	if (database.fail()) { database.close(); return result; }
	while (getline(database, info_encoded)) {
		info = decode(info_encoded);
		if (!validate(info)) { continue; }
		if (query.evaluate(info)) {
			if (index < info.size()) { info[index] = value; }
			result.push_back(info);
			content += encode(info) + '\n';
		}else {
			content += info_encoded + '\n';
		}
	}
	database.close();
	database.open(path, OVERWRITE);
	database << content;
	if (database.fail()) { result.clear(); }
	database.close();
	return result;
}
//template variations must be specified
template Database::Result Database::update_all(unsigned int index, const string value, const SimpleQuery& query);
template Database::Result Database::update_all(unsigned int index, const string value, const Query& query);
template Database::Result Database::update_all(unsigned int index, const string value, const QuerySet& query);

template<typename QueryType> Database::Result Database::read(const QueryType& query, bool all) {
	string info_encoded;
	Info info;
	Result result;
	database.open(path, READ);
	if (database.fail()) { database.close(); return result; } //Should I throw an execption?
	while (getline(database, info_encoded)) {
		info = decode(info_encoded);
		if (!validate(info)) { continue; }
		if (query.evaluate(info)) { 
			result.push_back(info);
			if (all == false) { database.close(); return result; }
		}
	}
	database.close();
	return result;
}
//template variations must be specified
template Database::Result Database::read(const SimpleQuery& query, bool all);
template Database::Result Database::read(const Query& query, bool all);
template Database::Result Database::read(const QuerySet& query, bool all);

template<typename QueryType> Database::Result Database::remove(const QueryType& query) {
	string info_encoded;
	Info info;
	Result result;
	string content;
	database.open(path, READ);
	if (database.fail()) { database.close(); return result; }
	while (getline(database, info_encoded)) {
		info = decode(info_encoded);
		if (!validate(info)) { continue; }
		if (query.evaluate(info)) { result.push_back(info); }
		else {
			content += info_encoded + '\n';
		}
	}
	database.close();
	database.open(path, OVERWRITE);
	database << content;
	if (database.fail()) {result.clear();}
	database.close();
	return result;
}
//template variations must be specified
template Database::Result Database::remove(const SimpleQuery& query);
template Database::Result Database::remove(const Query& query);
template Database::Result Database::remove(const QuerySet& query);

template<typename QueryType> bool Database::exist(const QueryType& query) {
	string info_encoded;
	Info info;
	database.open(path, READ);
	if (database.fail()) { database.close(); return false; }
	while (getline(database, info_encoded)) {
		info = decode(info_encoded);
		if (!validate(info)) { continue; }
		if (query.evaluate(info)) { database.close(); return true; }
	}
	database.close();
	return false;
}
//template variations must be specified
template bool Database::exist(const SimpleQuery& query);
template bool Database::exist(const Query& query);
template bool Database::exist(const QuerySet& query);