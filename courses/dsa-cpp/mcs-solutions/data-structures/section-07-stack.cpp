/**
* Mahmoud Elnagar - 04/09/2022
*/
#include<iostream>
#include<vector>
#include<stack>
using namespace std;

#pragma region MCS: Problem Set 1 Solutions
//Problem #2: Reverse subwords
string reverse_subwords(string line) {
	string reversed;
	stack<char> s;
	for (auto i = 0; i < line.size(); i++) {
		if (line[i] == ' ') {
			while (!s.empty()){
				reversed += s.top();
				s.pop();
			}
			reversed += " ";
		}else {
			s.push(line[i]);
		}
	}
	while (!s.empty()) {
		reversed += s.top();
		s.pop();
	}
	return reversed;
}
//Problem #3: Reverse a number using stack
int reverse_num(int num) {
	stack<int> s;
	while (num > 0) {
		s.push(num % 10);
		num = num / 10;
	}
	int m = 1;
	while (!s.empty()){
		num += s.top() * m;
		s.pop();
		m *= 10;
	}
	return num;
}
//Problem #4: Valid Parentheses
bool is_valid(string str) {
	stack<char> s;
	for (char c : str) {
		if (c == '(' || c == '[' || c == '{') {
			s.push(c);
		}
		else {
			if (s.empty()) return false;
			char lp = '(';
			if (c == ']') lp = '[';
			if (c == '}') lp = '{';
			//
			if (lp != s.top()) return false;
			s.pop();
		}
	}
	return s.empty();
}
//Problem #5: Remove All Adjacent Duplicates In String
string remove_duplicates(string str) {
	stack<char> s;
	string output;
	for (char c : str) {
		if (s.empty() || s.top() != c) {
			s.push(c);
		}else {
			s.pop();
		}
	}

	while (!s.empty()){
		output = s.top() + output;
		s.pop();
	}
	return output;
}
#pragma endregion

#pragma region MCS: Problem Set 2 Solutions
//Problem #1: Asteroid Collision
vector<int> asteroid_collide(const vector<int>& asteroids) {
	stack<int> s;
	vector<int> output;

	for (int i = (asteroids.size() - 1); i >= 0; --i) {
		if (s.empty() || s.top() > 0 || asteroids[i] < 0) {
			s.push(asteroids[i]);
			continue;
		}

		int sum = s.top() + asteroids[i];

		if (sum >= 0) s.pop();
		if (sum > 0) s.push(asteroids[i]);
	}

	while (!s.empty()){
		output.push_back(s.top());
		s.pop();
	}

	return output;
}
//Problem #2: Insert at the bottom
template <typename T> stack<T>& insert_bottom(stack<T>& s, const T& item) {
	if (s.empty()) {
		s.push(item);
		return s;
	}else {
		T top = s.top();
		s.pop();
		insert_bottom(s, item);
		s.push(top);
	}
}
//Problem #3: Reverse Stack
template <typename T> stack<T>& reverse_stack(stack<T>& s) {
	if (s.empty()) {
		return s;
	}
	else {
		T top = s.top();
		s.pop();
		reverse_stack(s);
		insert_bottom(s, top);
	}
}
//Problem #4: Manual System Stack
struct Call {
	Call(vector<int> ins, int out = -1, bool done = false)
		: inputs{ ins }, output{ out }, computed{ done }{};

	vector<int> inputs{};
	int output{};
	bool computed{false};
};
int factorial_stack(int n) {
	stack<Call> cs;
	if (n <= 1) return 1;
	cs.push(Call({n}));

	while (!cs.empty()){
		if (cs.top().computed) {
			auto res = cs.top().output;
			cs.pop();
			if (cs.empty()) return res;
			cs.top().output = cs.top().inputs[0] * res;
			cs.top().computed = true;
		} else {
			if (cs.top().inputs[0] <= 1) {
				cs.top().output = 1;
				cs.top().computed = true;
			} else {
				cs.push(Call({cs.top().inputs[0] - 1 }));
			}
		}
	}
}
//Problem #5: Score of Parentheses
int paren_score1(const string& input) {
	stack<string> s;
	string term;
	int sum = 0;
	for (int i = 0; i < input.size(); ++i) {
		if (input[i] == '(') sum += 1;
		if (input[i] == ')') sum -= 1;
		term.push_back(input[i]);
		if (sum == 0) {
			s.push(term);
			term = "";
		}
	}
	int total = 0;
	while (!s.empty()) {
		term = s.top();
		s.pop();
		if (term == "()") {
			total += 1;
		}else {
			total += (2 * paren_score1(term.substr(1, term.size() - 2)));
		}
	}
	return total;
}
int paren_score2(const string& input) {
	int total = 0;
	stack<int> s;
	for (char c : input) {
		if (c == '(') {
			s.push(0);
		}else {
			int val = s.top(); s.pop();
			if (val == 0) {
				s.push(1);
			}else {
				while (s.top() != 0) {
					val += s.top();
					s.pop();
				}
				s.pop();
				s.push(2 * val);
			}
		}
	}

	while (!s.empty()){
		total += s.top();
		s.pop();
	}

	return total;
}
int paren_score3(const string& input) {
	stack<int> s;
	s.push(0); //final result
	for (char c : input) {
		if (c == '(') {
			s.push(0);
		}
		else {
			int val = s.top(); s.pop();
			if (val == 0) {
				val = 1;
			}
			else {
				val = val * 2;
			}

			int res = s.top() + val;
			s.pop(); s.push(res);
		}
	}

	return s.top();
}
//Problem #6: Next Greater Element
vector<int> next_greaters(const vector<int>& input) {
	vector<int> output(input.size());
	stack<int> s;
	for (int i = input.size() - 1; i >= 0; i--) {
		if (s.empty()) {
			output[i] = -1;
			s.push(input[i]);
		}
		else if (s.top() > input[i]){
			output[i] = s.top();
			s.push(input[i]);
		}else {
			s.pop();
			i++;
		}
	}
	return output;
}
#pragma endregion

#pragma region MCS: Problem Set 3 Solutions
//Problem #1: Infix to Postfix
int precedence(char op) {
	if (op == '+' || op == '-') return 1;
	if (op == '*' || op == '/') return 2;
	if (op == '^') return 3;
	return 0;
}
string to_postfix(const string& infix) {
	stack<char> s;
	string output;

	for (auto& c : infix) {
		if (isalnum(c)) { output += c; }
		else if (c == '(') { s.push(c); }
		else if (c == ')') {
			while (s.top() != '(') {
				output += s.top();
				s.pop();
			}
			s.pop();
		}
		else if (c == '^') { s.push(c); }
		else {
			while (!s.empty() && (precedence(c) <= precedence(s.top()))) {
				output += s.top();
				s.pop();
			}
			s.push(c);
		}
	}

	while (!s.empty()) {
		output += s.top();
		s.pop();
	}

	return output;
}
//Problem #2: Postfix Evaluation
double evaluate_postfix(const string& postfix) {
	stack<double> s;
	for (auto c : postfix) {
		if (isdigit(c)) { s.push(atoi(&c));}
		else {
			double b = s.top(); s.pop();
			double a = s.top(); s.pop();
			if (c == '+') s.push(a + b);
			else if (c == '-') s.push(a - b);
			else if (c == '*') s.push(a * b);
			else if (c == '/') s.push(a / b);
			else if (c == '^') s.push(pow(a, b));
		}
	}
	return s.top();
}
//Problem #3: Infix to Prefix
string to_prefix(const string& infix) {
	string output;
	stack<char> s;
	for (auto it = infix.rbegin(); it != infix.rend(); ++it) {
		char c = *it;
		if (isalnum(c)) { output = c + output; }
		else if (c == ')') { s.push(c); }
		else if (c == '(') {
			while (s.top() != ')') {
				output = s.top() + output;
				s.pop();
			}
			s.pop();
		}
		else if (!s.empty() && s.top() == '^' && c == '^') {
			output = c + output;
		}
		else {
			while (!s.empty() && precedence(c) < precedence(s.top())) {
				output = s.top() + output;
				s.pop();
			}
			s.push(c);
		}
	}

	while (!s.empty()) {
		output = s.top() + output;
		s.pop();
	}

	return output;
}
//Problem #5: Remove Expression Brackets
string remove_brackets(const string& input) {
	char last_sign = '+';
	bool reverse = false;
	string output;
	stack<char> s;

	for (auto c : input) {
		if (isdigit(c)) { output += c;}
		else if (c == '(') {
			s.push(last_sign);
			reverse = (last_sign == '-');
		}
		else if (c == ')') {
			s.pop();
			reverse = (!s.empty() && s.top() == '-');
		}
		else {
			char cop = c;
			if (reverse && c == '-') cop = '+';
			else if (reverse && c == '+') cop = '-';
			last_sign = cop;
			output += cop;
		}
	}
	return output;
}
#pragma endregion
