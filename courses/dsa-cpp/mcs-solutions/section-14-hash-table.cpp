/**
* Mahmoud Elnagar - 13/11/2022
*/
#include<iostream>
#include <vector>
#include <algorithm>
#include<cassert>
#include<unordered_map>
#include<unordered_set>

using namespace std;

#pragma region Starter

int hash_num(int value, int n_ = 65407) {
	long long n = n_;
	return (value % n + n) % n;
}

int hash_lower_string(string str, int n) {
	long long nn = n;
	long long sum = 0;
	for (int i = 0; i < (int)str.size(); ++i)
		sum = (sum * 26 + str[i] - 'a') % nn;
	return sum % nn;
}

int hash_string(const string& str, int n, int a = 39) {
	int h = 0;
	for (int i = 0; i < str.size(); ++i) {
		h = h * a + str[i];
	}
	return (h & INT_MAX) % n;
}

struct PhoneEntry {
	const static int INTERNAL_LIMIT = 65407;
	string name;			// key
	string phone_number;	// data

	int hash() {
		return hash_string(name, INTERNAL_LIMIT);
	}

	PhoneEntry(string name, string phone_number) :
		name(name), phone_number(phone_number) {
	}

	void print() {
		cout << "(" << name << ", " << phone_number << ")  ";
	}
};

class PhoneHashTable {
private:
	int table_size;
	vector<vector<PhoneEntry>> table;
	// we can use others: e.g. list<PhoneEntry>
public:
	PhoneHashTable(int table_size) :
		table_size(table_size) {
		table.resize(table_size);
	}

	bool get(PhoneEntry& phone) {
		int idx = phone.hash() % table_size;

		for (int i = 0; i < (int)table[idx].size(); ++i) {
			if (table[idx][i].name == phone.name) {
				phone = table[idx][i];
				return true;
			}
		}
		return false;
	}

	void put(PhoneEntry phone) {
		int idx = phone.hash() % table_size;

		for (int i = 0; i < (int)table[idx].size(); ++i) {
			if (table[idx][i].name == phone.name) {
				table[idx][i] = phone; // exist => update
				return;
			}
		}
		table[idx].push_back(phone);
	}

	bool remove(PhoneEntry phone) {
		int idx = phone.hash() % table_size;
		for (int i = 0; i < (int)table[idx].size(); ++i) {
			if (table[idx][i].name == phone.name) {
				// Swap with last and remove last in O(1)
				swap(table[idx][i], table[idx].back());
				table[idx].pop_back();
				return true;
			}
		}
		return false;
	}

	void print_all() {
		for (int hash = 0; hash < table_size; ++hash) {
			if (table[hash].size() == 0)
				continue;

			cout << "Hash " << hash << ": ";
			for (int i = 0; i < (int)table[hash].size(); ++i)
				table[hash][i].print();
			cout << "\n";
		}
	}
};
#pragma endregion

#pragma region MCS: Problem Set 1 Solutions
//Problem #1: Small, upper and digits
int hash_alnum_string(string str, int n) {
	long long nn = n;
	long long sum = 0;
	for (int i = 0; i < (int)str.size(); ++i) {
		int value;
		if (isdigit(str[i])) {
			value = str[i] - '0'; //[0:9]
		}
		else if (islower(str[i])) {
			value = str[i] - 'a' + 10; //[10:35]
		}
		else if (isupper(str[i])) {
			value = str[i] - 'A' + 36; //[36:61]
		}
		else {
			throw exception("only hashing alnum strings");
		}
		sum = (sum * 62 + value) % nn;
	}
	return sum % nn;
}
//Problem #2: Folding for hashing
int hash_string_folding(string str, int n_ = 65407) {
	long long n = n_;
	long long sum = 0;
	for (int i = 0; i < str.size(); i += 4) {
		string block = str.substr(i, 4);
		sum += hash_string(block, n_);
		sum %= n;
	}
	return sum;
}
//Problem #3: Key based on multiple variables
struct SomeObject {
	const static int INTERNAL_LIMIT = 2147483647;
	string str1, str2;
	int number;

	int hash() {
		long long res = hash_string(str1 + str2, INTERNAL_LIMIT);
		res += hash_num(number, INTERNAL_LIMIT);
		return res % INTERNAL_LIMIT;
	}
};
//Problem #4: Rehashing 
//	[Implemented @../implementations/hash-map.cpp]
//Problem #5: Array of linked-list 
//	[Implemented @../implementations/hash-map.cpp]
#pragma endregion

#pragma region MCS: Problem Set 2 Solutions
//Problem #1: Number of Distinct Substring
unordered_set<string> unique_substrings(const string& str) {
	unordered_set<string> s;
	for (int i = 0; i < str.size(); ++i) {
		for (int n = 1; n <= (str.size() - i); ++n) {
			s.insert(str.substr(i, n));
		}
	}
	return s;
}
int count_unique_substrings(const string& str) {
	return unique_substrings(str).size();
}
//Problem #2: Common substrings
int count_substrings_match(const string& str1, const string& str2) {
	auto s1 = unique_substrings(str1);
	auto s2 = unique_substrings(str2);
	int c = 0;
	for (auto x : s1) c += s2.count(x);
	return c;
}
//Problem #3: Unique Anagrams
int count_anagram_substrings(const string& str) {
	unordered_set<string> s;
	for (int i = 0; i < str.size(); ++i) {
		for (int n = 1; n <= (str.size() - i); ++n) {
			string sub = str.substr(i, n);
			sort(sub.begin(), sub.end());
			s.insert(sub);
		}
	}
	return s.size();
}
//Problem #4: Cycle in linked-list
struct Node {
	int data{ };
	Node* next{ };
	Node(int data) :
		data(data) {
	}
};
class LinkedList {
private:
	Node* head{ };
	Node* tail{ };
	int length = 0;
public:
	void print() {
		for (Node* cur = head; cur; cur = cur->next)
			cout << cur->data << " ";
		cout << "\n";
	}
	void insert_end(int value) {
		Node* item = new Node(value);

		if (!head)
			head = tail = item;
		else {
			tail->next = item;
			tail = item;
		}
		++length;
	}
	void create_cycle() {
		for (int i = 0; i < 4; ++i)
			insert_end(i);
		Node* now = tail;
		for (int i = 0; i < 3; ++i)
			insert_end(14 + i);
		tail->next = now;// cycle
	}
	void remove_cycle() {
		unordered_set<Node*> nodes;
		Node* curr = head;
		nodes.insert(curr);
		while(curr){
			if (nodes.count(curr->next)) {
				curr->next = nullptr;
				tail = curr;
				return;
			}
			else {
				nodes.insert(curr->next);
			}

			curr = curr->next;
		}
	}
};
//Problem #5: Quadratic Probing
class PhoneHashTableQ {
private:
	int table_size;
	vector<PhoneEntry*> table;
	// to mark a cell as deleted
	PhoneEntry* deleted{};
public:
	PhoneHashTableQ (int table_size) :
		table_size(table_size) {
		table.resize(table_size);
		deleted = new PhoneEntry("", "");
	} //  needs a destructor

	void rehash() {
		PhoneHashTableQ new_table(2 * table_size);
		for (auto ptr : table) {
			if (ptr && ptr != deleted) {
				new_table.put(*ptr);
				delete ptr;
			}
		}
		table = new_table.table;
		table_size = new_table.table_size;
	}

	void put(PhoneEntry phone) {
		int idx = phone.hash() % table_size;
		int org_idx = idx;
		// We max move table_size steps
		for (int step = 1; step <= table_size; ++step) {
			if (table[idx] == deleted || !table[idx]) {	// empty
				table[idx] = new PhoneEntry(phone.name, phone.phone_number);
				return;
			}
			else if (table[idx]->name == phone.name) {
				table[idx]->phone_number = phone.phone_number;
				return;	// update
			}
			idx = (org_idx + step * step)% table_size;	// move next
			if (idx == org_idx) break;
		}
		rehash();
		put(phone);
	}

	bool remove(PhoneEntry phone) {
		int idx = phone.hash() % table_size;
		int org_idx = idx;

		for (int step = 1; step <= table_size; ++step) {
			if (!table[idx])
				break;
			if (table[idx] != deleted && table[idx]->name == phone.name) {
				delete table[idx];
				table[idx] = deleted;
				return true;
			}
			idx = (org_idx + step * step) % table_size;	// move next
			if (idx == org_idx) break;
		}
		return false;
	}

	bool get(PhoneEntry& phone) {
		int idx = phone.hash() % table_size;
		int org_idx = idx;

		for (int step = 1; step <= table_size; ++step) {
			if (!table[idx])
				break;
			if (table[idx] != deleted && table[idx]->name == phone.name) {
				phone.phone_number = table[idx]->phone_number;
				return true;
			}
			idx = (org_idx + step * step) % table_size;	// move next
			if (idx == org_idx) break;
		}
		return false;
	}

	void print_all() {
		for (int hash = 0; hash < table_size; ++hash) {
			cout << hash << " ";
			if (table[hash] == deleted)
				cout << " X ";
			else if (!table[hash])
				cout << " E ";
			else
				table[hash]->print();
			cout << "\n";
		}
		cout << "******************\n";
	}
};
#pragma endregion

