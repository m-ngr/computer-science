#pragma once
#include <string>
#include <fstream>
#include <sstream>
#include <vector>

using namespace std;

bool isNumber(const string& str);

class Database {
public:
	typedef vector<string> Info;
	typedef vector<vector<string>> Result;

	static const char SYM{ '|' };
	static const unsigned int MIN_INFO = 2;

	Database() = delete; //delete default constructor
	Database(const string file_path, const unsigned int minimum_info = MIN_INFO);
	unsigned int minimum_info()const { return min_info; }

	string encode(const Info & info);
	Info decode(string info_encoded);

	bool write(Info& info);
	bool update(Info& info);
	unsigned int update_all(unsigned int index, const string value);
	Info read(unsigned int q_id);
	bool remove(unsigned int q_id);

	template<typename QueryType> bool write(Info& info,const QueryType& query);
	template<typename QueryType> bool update(Info& info, const QueryType& query);
	template<typename QueryType> Result update_all(unsigned int index, const string value ,const QueryType& query);
	template<typename QueryType> Result read(const QueryType& query, bool all = false);
	template<typename QueryType> Result remove(const QueryType& query);
	template<typename QueryType> bool exist(const QueryType& query);

private:
	fstream database;
	string path{};
	unsigned int id{};
	unsigned int min_info{MIN_INFO};

	//static const creates one constant for all class objects
	static const int APPEND = ios::in | ios::out | ios::app;
	static const int OVERWRITE = ios::in | ios::out | ios::trunc;
	static const int READ = ios::in;

	void load_id();
	bool validate(const Info& info);
};

class SimpleQuery {
public:
	SimpleQuery() = delete; //delete default constructor
	SimpleQuery(unsigned int _index, string _iden, bool _state = true) : idx{ _index }, identifier{ _iden }, state{ _state }{};
	bool evaluate(const Database::Info& info)const;
private:
	unsigned int idx{};
	string identifier{};
	bool state{ true };
};

class Query {
public:
	Query(bool _any = false) :any { _any } {};
	Query(const SimpleQuery& simple_q, bool _any = false) :any{ _any } { qList.push_back(simple_q); }
	Query(const vector<SimpleQuery>& q_list, bool _any = false) :qList{ q_list }, any{ _any } {}
	bool evaluate(const Database::Info& info)const;
	void insert(const SimpleQuery& simple_q) { qList.push_back(simple_q); }
private:
	bool any{ false };
	vector<SimpleQuery> qList;
};

class QuerySet {
public:
	QuerySet(bool _any = false) :any{ _any } {};
	QuerySet(const Query& q_list, bool _any = false) :any{ _any } { qSet.push_back(q_list); }
	QuerySet(const vector<Query>& q_set, bool _any = false) :qSet{ q_set }, any{ _any } {}
	bool evaluate(const Database::Info& info)const;
	void insert(const Query& query) { qSet.push_back(query); }
private:
	bool any{ false };
	vector<Query> qSet;
};
