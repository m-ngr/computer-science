/**
* Mahmoud Elnagar - 20/09/2022
*/

#include "SparseArray.cpp"

class SparseMatrix {
public:
	explicit SparseMatrix(unsigned rows, unsigned columns) : rows{ rows }, cols{ columns }{};

	void set(unsigned row, unsigned column, int value) {
		if (row >= rows || column >= cols) throw out_of_range("location is out of range");
		// matrix is empty (no rows)
		if (mtrx.empty()) { return mtrx.push_back(newEntry(row, column, value));}
		// check last element
		auto last = mtrx.end(); --last;
		if (last->index < row) { return mtrx.push_back(newEntry(row, column, value)); }
		if (last->index == row) { return last->value.set(column, value); }
		//check elements from begin to end
		for (auto it = mtrx.begin(); it != mtrx.end(); ++it) {
			if (it->index == row) {
				return it->value.set(column, value);
			}
			else if (it->index > row) {
				mtrx.insert(it, newEntry(row, column, value));
				return;
			}
		}
	}

	int get(unsigned row, unsigned column) const {
		if (row >= rows || column >= cols) throw out_of_range("location is out of range");
		for (auto& item : mtrx) {
			if (item.index == row) return item.value.get(column);
			if (item.index > row) break;
		}
		return 0;
	};

	void print() const {
		for (auto& item : mtrx) item.value.print();
	};

	void print_full() const {
		unsigned i = 0;

		for (auto& item : mtrx) {
			if (item.index > i) printEmptyRow(item.index - i);
			i = item.index + 1;
			item.value.print_full();
		}

		printEmptyRow(rows - i);
	};

	void add(const SparseMatrix& sm) {
		auto it1 = mtrx.begin();
		auto it2 = sm.mtrx.begin();

		while (it1 != mtrx.end() && it2 != sm.mtrx.end()) {
			if (it1->index == it2->index) {
				it1->value.add(it2->value);
				++it1; ++it2;
			}
			else if (it1->index > it2->index) {
				mtrx.insert(it1, *it2);
				++it2;
			}
			else {
				++it1;
			}
		}

		while (it2 != sm.mtrx.end() && it2->index < rows) {
			mtrx.push_back(*it2);
			++it2;
		}
	};
private:
	unsigned rows{0};
	unsigned cols{0};
	list<Entry<SparseArray>> mtrx;

	Entry<SparseArray> newEntry(unsigned row, unsigned column, int value) {
		SparseArray new_row(cols);
		new_row.set(column, value);
		return Entry(row, new_row);
	}

	void printEmptyRow(unsigned n = 1) const {
		for (auto r = 0; r < n; ++r) {
			for (auto i = 0; i < cols; ++i) cout << "0 ";
			cout << endl;
		}
	}
};