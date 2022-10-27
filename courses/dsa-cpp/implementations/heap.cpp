#include<stdexcept>
#include <vector>

class HeapEmpty : public std::runtime_error {
public:
	explicit HeapEmpty(const std::string& message) : runtime_error(message) {};
};

template<typename T, typename C = std::less<T>> class Heap {
public:
	Heap(){}

	Heap(const std::initializer_list<T>& items) { push(items); }

	template<typename CIterator>
	Heap(CIterator start, CIterator end) { push(start, end); }

	void push(const std::initializer_list<T>& items) {
		for (auto& item : items) v.push_back(item);
		heapify();
	}

	template<typename CIterator> void push(CIterator first, CIterator last) { 
		while (first != last) { 
			v.push_back(*first);
			++first;
		}
		heapify();
	}

	void push(const T& item) {
		v.push_back(item);
		heapify_up();
	}

	T pop() throw(HeapEmpty) {
		T value = top();
		T last_value = last();
		v.pop_back();
		if (empty()) return value;
		v[0] = last_value;
		heapify_down();
		return value;
	}

	const T& top() const throw(HeapEmpty) {
		if (empty()) throw HeapEmpty("Invalid Operation: the heap is empty");
		return v[0];
	}

	bool empty() const { return v.empty(); }
	
	unsigned size() const { return v.size(); }
protected:

	int left_index(int index) const {
		int idx = 2 * index + 1;
		return idx < size() ? idx : -1;
	}

	int right_index(int index) const { 
		int idx = 2 * index + 2;
		return idx < size() ? idx : -1;
	}

	int parent_index(int index) const { 
		return index > 0 ? (index - 1) / 2 : -1;
	}

	bool has_left(int index) const { return left_index(index) != -1; }
	bool has_right(int index) const { return right_index(index) != -1; }
	bool has_parent(int index) const { return parent_index(index) != -1; }

	T& left(int index) throw (std::out_of_range){
		int idx = left_index(index);
		if (idx < 0) throw std::out_of_range("Invalid Operation: this node has no left child.");
		return v[idx];
	}
	
	T& right(int index) throw (std::out_of_range){
		int idx = right_index(index);
		if (idx < 0) throw std::out_of_range("Invalid Operation: this node has no right child.");
		return v[idx];
	}

	T& parent(int index) throw (std::out_of_range){
		int idx = parent_index(index);
		if (idx < 0) throw std::out_of_range("Invalid Operation: root node has no parent.");
		return v[idx];
	}

	T& last() throw (HeapEmpty) {
		if (empty()) throw HeapEmpty("Invalid Operation: the heap is empty");
		return v[size() - 1];
	}

	void heapify_down(int index = 0) {//O(logn)
		int end_index = size() / 2;
		if (index < 0 || index >= end_index) return;

		while (index < end_index) {
			int child_index = left_index(index);
			if (has_right(index) && heap_order(right(index), left(index))) {
				child_index = right_index(index);
			}
			if (heap_order(v[index], v[child_index])) return;
			swap(index, child_index);
			index = child_index;
		}
	}

	void heapify_up(int index = -1) { // O(logn)
		if (index == -1) index = size() - 1;
		if (index <= 0 || index >= size()) return;

		while (index > 0) {
			int p_index = parent_index(index);
			if (heap_order(v[p_index], v[index])) return;
			swap(p_index, index);
			index = p_index;
		}
	}

	void heapify() {
		// bottom-up approach is O(n)
		for (int i = (size() / 2) - 1; i >= 0; --i) {
			heapify_down(i);
		}
	}

	void swap(int idx1, int idx2) {
		T temp = v[idx1];
		v[idx1] = v[idx2];
		v[idx2] = temp;
	}

private:
	C heap_order {};
	std::vector<T> v{};
};