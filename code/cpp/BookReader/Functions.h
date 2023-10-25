#pragma once
#include<iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <ctime>

using namespace std;

extern string msg;
const string EXIT_CODE = "0";
struct Function {
	Function(string f_name, bool (*f_ptr)()) :name{ f_name }, function{ f_ptr }{}
	string name;
	bool (*function)();
};

bool isNumber(const string& str);
string lower_string(string str);
vector<string> split_string(const string& str);
bool search_string(const string& content, const string& search);
time_t get_date();
string print_date(time_t date);
void print_message();
void print_header(string title, string description = "", bool reset_input = true);
int read_int(int max = 0, int min = 0, string what = "");
string read_string(string what = "", bool line = false, unsigned min = 0, unsigned max = 0);
bool read_bool(string what);
void print_menu(const vector<Function>& menu);
void view_menu(string title, vector<Function>& menu, string description = "");
string print_book(unsigned int book_id);
string print_user(unsigned int user_id);