/**
* Mahmoud Elnagar - 20/09/2022
*/

#include<list>
#include<iostream>

using namespace std;

template<typename T> class Entry {
public:
	explicit Entry(unsigned index = 0, const T& value = 0) : value{ value }, index{ index }{};
	T value{};
	unsigned index{ 0 };
};

class SparseArray {
public:
	
	explicit SparseArray(unsigned size) : capacity{ size } {};

	void set(unsigned index, int value) {
		if (index >= capacity) throw out_of_range("index is out of range");
		// list is empty
		if (lst.empty()) return lst.push_back(Entry(index, value));
		// check last element
		auto last = lst.end(); --last;
		if (last->index < index) return lst.push_back(Entry(index, value));
		if (last->index == index) { last->value = value; return; }
		//check elements from begin to end
		for (auto it = lst.begin(); it != lst.end(); ++it) {
			if (it->index == index) {
				it->value = value;
				return;
			}
			else if (it->index > index) {
				lst.insert(it, Entry(index, value));
				return;
			}
		}
		
	}

	int get(unsigned index) const {
		if (index >= capacity) throw out_of_range("index is out of range");
		for (auto& item : lst) {
			if (item.index == index) return item.value;
			if (item.index > index) break;
		}
		return 0;
	};

	void print() const {
		for (auto& item : lst) cout << item.value << " ";
		cout << endl;
	};

	void print_full() const {
		unsigned i = 0;

		for (auto& item : lst) {
			if (item.index > i) printZeros(item.index - i);
			i = item.index + 1;
			cout << item.value << " ";
		}

		printZeros(capacity - i);
		cout << endl;
	};

	void add(const SparseArray& sa) {
		auto it1 = lst.begin(); 
		auto it2 = sa.lst.begin();

		while (it1 != lst.end() && it2 != sa.lst.end()) {
			if (it1->index == it2->index) {
				it1->value += it2->value;
				++it1; ++it2;
			}
			else if (it1->index > it2->index) {
				lst.insert(it1, *it2);
				++it2;
			}
			else {
				++it1;
			}
		}

		while (it2 != sa.lst.end() && it2->index < capacity) {
			lst.push_back(*it2);
			++it2;
		}
	};
private:
	unsigned capacity{ 0 };
	list<Entry<int>> lst{};

	void printZeros(unsigned n) const {
		for (auto i = 0; i < n; ++i) cout << "0 ";
	}
};


