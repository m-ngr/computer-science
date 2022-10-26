/**
* Mahmoud Elnagar - 26/10/2022
*/
#include<iostream>
#include <cassert>
#include<vector>
#include<queue>
using namespace std;

#pragma region Helpers

class MinHeap {
protected:
	int* array{ };
	int size{ };
	int capacity{ 1000 };

	int left(int node) {
		int p = 2 * node + 1;
		if (p >= size)
			return -1;
		return p;
	}
	int right(int node) {
		int p = 2 * node + 2;
		return p >= size ? -1 : p;
	}
	int parent(int node) {
		return node == 0 ? -1 : (node - 1) / 2;
	}

	void heapify_up(int child_pos) {
		// stop when parent is smaller (or no parent)
		int par_pos = parent(child_pos);
		if (child_pos == 0 || array[par_pos] < array[child_pos])
			return;

		swap(array[child_pos], array[par_pos]);
		heapify_up(par_pos);
	}

	void heapify_down(int parent_pos) {	// O(logn)
		int child_pos = left(parent_pos);
		int right_child = right(parent_pos);

		if (child_pos == -1) // no children
			return;
		// is right smaller than left?
		if (right_child != -1 && array[right_child] < array[child_pos])
			child_pos = right_child;

		if (array[parent_pos] > array[child_pos]) {
			swap(array[parent_pos], array[child_pos]);
			heapify_down(child_pos);
		}
	}

	void heapify() {	// O(n)
		for (int i = size / 2 - 1; i >= 0; --i)
			heapify_down(i);
	}

public:
	MinHeap() {
		array = new int[capacity] {};
		size = 0;
	}

	MinHeap(const vector<int>& v) {
		assert((int)v.size() <= capacity);
		array = new int[capacity] {};
		size = v.size();

		for (int i = 0; i < (int)v.size(); ++i)
			array[i] = v[i];

		heapify();
	}

	~MinHeap() {
		if (array) delete[] array;
		array = nullptr;
	}

	MinHeap& operator=(MinHeap&& heap) {
		array = heap.array;
		size = heap.size;
		capacity = heap.capacity;
		heap.array = nullptr;
		heap.size = 0;
		return *this;
	}

	void push(int key) {
		assert(size + 1 <= capacity);
		array[size++] = key;
		heapify_up(size - 1);
	}

	void pop() {
		assert(!isempty());
		array[0] = array[--size];
		heapify_down(0);
	}

	int top() {
		assert(!isempty());
		return array[0];
	}

	bool isempty() {
		return size == 0;
	}
	// Problem Set 1 -> Problem 3
	void print_less_than(int val, int parent_pos = 0) {
		if (parent_pos < 0) return;
		if (array[parent_pos] < val) {
			cout << array[parent_pos] << " ";
			print_less_than(val, left(parent_pos));
			print_less_than(val, right(parent_pos));
		}
	}
};

/*
* heap_up(node_value, parent_value):
	- should return true if the node should be heapified up.
	- should return false to stop heapifing.
*/
void heapify_up(int* p, int n, int i, bool (*heap_up) (int, int)) {
	if (i <= 0 || i >= n) return;

	while (i > 0) {
		int pi = (i - 1) / 2;
		if (!heap_up(p[i], p[pi])) return;
		swap(p[i], p[pi]);
		i = pi;
	}
}
/*
* heap_down(left_value, right_value):
	- should return true to choose the right child for heapifying.
	- should return false to choose the left child for heapifying.
======================= other case ===============================
* heap_down(node_value, child_value):
	- should return true if the node should be heapified down.
	- should return false to stop heapifing.
*/
void heapify_down(int* p, int n, int i, bool (*heap_down) (int, int)) {
	int end_index = n / 2;
	if (i < 0 || i >= end_index) return;

	while (i < end_index) {
		int li = i * 2 + 1;
		int ri = li + 1;
		if (ri < n && heap_down(p[li], p[ri])) li = ri;
		if (!heap_down(p[i], p[li])) return;
		swap(p[i], p[li]);
		i = li;
	}
}
/*
* heap_down(left_value, right_value):
	- should return true to choose the right child for heapifying.
	- should return false to choose the left child for heapifying.
======================= other case ===============================
* heap_down(node_value, child_value):
	- should return true if the node should be heapified down.
	- should return false to stop heapifing.
*/
void heapify(int* p, int n, bool (*heap_down) (int, int)) {
	int end_index = n / 2;
	for (int i = (n / 2) - 1; i >= 0; --i) {
		heapify_down(p, n, i, heap_down);
	}
}

#pragma endregion

#pragma region MCS: Problem Set 1 Solution
//Problem #1: Max-Heap
template<typename T> class MaxHeap {
private:
	T* array{ };
	int size{ };
	int capacity{ 1000 };

	int left(int node) {
		int p = 2 * node + 1;
		if (p >= size)
			return -1;
		return p;
	}
	int right(int node) {
		int p = 2 * node + 2;
		return p >= size ? -1 : p;
	}
	int parent(int node) {
		return node == 0 ? -1 : (node - 1) / 2;
	}

	void heapify_up(int child_pos) {
		// stop when parent is smaller (or no parent)
		int par_pos = parent(child_pos);
		if (child_pos == 0 || array[par_pos] >= array[child_pos])
			return;

		swap(array[child_pos], array[par_pos]);
		heapify_up(par_pos);
	}

	void heapify_down(int parent_pos) {	// O(logn)
		int child_pos = left(parent_pos);
		int right_child = right(parent_pos);

		if (child_pos == -1) // no children
			return;
		// is right smaller than left?
		if (right_child != -1 && array[right_child] >= array[child_pos])
			child_pos = right_child;

		if (array[parent_pos] < array[child_pos]) {
			swap(array[parent_pos], array[child_pos]);
			heapify_down(child_pos);
		}
	}

	void heapify() {	// O(n)
		for (int i = size / 2 - 1; i >= 0; --i)
			heapify_down(i);
	}

public:
	MaxHeap() {
		array = new T[capacity] {};
		size = 0;
	}

	MaxHeap(const vector<T>& v) {
		assert((int)v.size() <= capacity);
		array = new T[capacity] {};
		size = v.size();

		for (int i = 0; i < (int)v.size(); ++i)
			array[i] = v[i];

		heapify();
	}

	~MaxHeap() {
		if (array) delete[] array;
		array = nullptr;
	}

	void push(T key) {
		assert(size + 1 <= capacity);
		array[size++] = key;
		heapify_up(size - 1);
	}

	void pop() {
		assert(!isempty());
		array[0] = array[--size];
		heapify_down(0);
	}

	const T& top() {
		assert(!isempty());
		return array[0];
	}

	bool isempty() {
		return size == 0;
	}
};
//Problem #2: Max-Heap (based on Min-Heap)
class MaxHeap2 {
public:
	MaxHeap2() {}

	MaxHeap2(const vector<int>& v) {
		vector<int> nv(v.size());
		for (int i = 0; i < (int)v.size(); ++i)
			nv[i] = -v[i];
		heap = MinHeap(nv);
	}

	void push(int key) {
		heap.push(-key);
	}

	void pop() {
		heap.pop();
	}

	int top() {
		return -heap.top();
	}

	bool isempty() {
		return heap.isempty();
	}
private:
	MinHeap heap;
};
//Problem #3: Find smaller values [Implemented at MinHeap.print_less_than()]
//Problem #4: Is Heap
bool is_heap_array(int* p, int n) {
	int end_index = n / 2;

	for (int i = 0; i < end_index; ++i) {
		int left_index = 2 * i + 1;
		if (p[i] > p[left_index]) return false;

		int right_index = left_index + 1;
		if (right_index >= n) return true;
		if (p[i] > p[right_index]) return false;
	}

	return true;
}
//Problem #5: Heap Sort
void heap_sort(int* p, int n) {
	auto maximize = [](int v, int c) {return v < c; };
	heapify(p, n, maximize);
	while (n--) {
		swap(p[0], p[n]);
		heapify_down(p, n, 0, maximize);
	}
}
#pragma endregion

#pragma region MCS: Problem Set 2 Solution
//Problem #1: Kth Smallest number (stream)
class KthNumberProcessor {
private:
	int k;
	priority_queue<int> q; //max-heap
public:
	KthNumberProcessor(int k) : k(k) {}

	int next(int new_num) {

		if (q.size() < k) {
			q.push(new_num);
		} else if (new_num < q.top()) {
			q.pop();
			q.push(new_num);
		}
		return q.top();
	}
};
//Problem #2: Priority Queue
class PriorityQueue {
public:
	void enqueue(int task_id, int priorty) {
		heap.push(Entry(priorty, task_id));
	}

	int dequeue() {
		auto res = heap.top();
		heap.pop();
		return res.value;
	}

	bool empty() { return heap.isempty(); }

private:

	class Entry {
	public:
		int key{};
		int value{};

		Entry(int key=0, int value=0) : key{ key }, value{ value }{};

		bool operator<(const Entry& e2) { return key < e2.key; }
		bool operator>(const Entry& e2) { return key > e2.key; }
		bool operator==(const Entry& e2) { return key == e2.key; }
		bool operator!=(const Entry& e2) { return key != e2.key; }
		bool operator<=(const Entry& e2) { return key <= e2.key; }
		bool operator>=(const Entry& e2) { return key >= e2.key; }
	};

	MaxHeap<Entry> heap;

};
//Problem #3: Binary Tree Special Traversal
struct BinaryTree {
	int value{};
	BinaryTree* left{};
	BinaryTree* right{};

	BinaryTree(int value = {}, BinaryTree* left = {}, BinaryTree* right = {})
		: value{ value }, left{ left }, right{ right }{};

	void level_order_traversal() {
		queue<BinaryTree*> q;
		priority_queue<int> pq;
		q.push(this);
		while (!q.empty()) {
			int lvlsz = q.size();
			
			while (lvlsz--){
				BinaryTree* node = q.front(); q.pop();
				pq.push(node->value);
				if (node->left) q.push(node->left);
				if (node->right) q.push(node->right);
			}

			while (!pq.empty()) {
				cout << pq.top()<<" ";
				pq.pop();
			}

			cout << endl;
		}
	}
	
	void level_order_traversal2() {
		/* use two max-heaps
			1- hold the nodes of the current level
			2- hold the nodes for the next level
		*/
		priority_queue<pair<int, BinaryTree*>> heap[2];
		int cur = 0;
		heap[cur].push(make_pair(value, this));
		while (!heap[cur].empty()) {
			int lvlsz = heap[cur].size();

			while (lvlsz--) {
				auto res = heap[cur].top(); heap[cur].pop();
				auto node = res.second;
				cout << res.first << " ";
				if (node->left) heap[!cur].push(make_pair(node->left->value, node->left));
				if (node->right) heap[!cur].push(make_pair(node->right->value, node->right));
			}

			cout << endl;
			cur = !cur;
		}
	}
};
#pragma endregion