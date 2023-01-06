/**
* Mahmoud Elnagar - 06/09/2022
*/
#include<iostream>
#include<vector>
#include<queue>
#include<stack>
using namespace std;

#pragma region MCS: Problem Set 1 Solutions
//Problem #1: Deque
template<typename T>class Deque {
public:
	void enqueue_front(const T& item) {
		if (size() < v.size()) {
			f = prev(f);
			v[f] = item;
		}
		else {
			normalize();
			v.push_back(item);
			f = r = n ;
		}
		++n;
	}
	void enqueue_rear(const T& item) {
		if (size() < v.size()) {
			v[r] = item;
			r = next(r);
		} else {
			normalize();
			v.push_back(item);
		}
		++n;
	}
	T dequeue_front() {
		if (empty()) throw exception("NO");
		T value = v[f];
		f = next(f);
		--n;
		return value;
	}
	T dequeue_rear() {
		if (empty()) throw exception("NO");
		r = prev(r);
		--n;
		return v[r];
	}

	const T& front()const {
		if (empty()) throw exception("NO");
		return v[f];
	}
	const T& rear()const {
		if (empty()) throw exception("NO");
		return v[prev(r)];
	}

	bool empty()const { return size() == 0; }
	unsigned size()const { return n; }
private:
	vector<T> v;
	unsigned f{ 0 };
	unsigned r{ 0 };
	unsigned n{ 0 };

	unsigned next(unsigned pos) {
		return (pos + 1) % v.size();
	}
	unsigned prev(unsigned pos) {
		int p = pos - 1;
		if (p < 0) p = v.size() - 1;
		return p;
	}

	void normalize() {
		if (f == 0 && r == 0) return;
		if (!empty()) {
			vector<T> new_v;
			new_v.reserve(2 * v.size());
			while (!empty()) new_v.push_back(dequeue_front());
			v = new_v;
			n = v.size();
		}
		f = r = 0;
	}
};
//Problem #2: Implement a stack using a single queue
template<typename T>class Stack {
public:
	void push(const T& item) { //O(n)
		int ops = size();
		q.push(item);
		for(int i = 0; i < ops; ++i) {
			q.push(q.front());
			q.pop();
		}
	}
	T pop() {
		T value = top();
		q.pop();
		return value;
	}
	const T& top()const { return q.front(); };
	bool empty() const { return q.empty(); };
	unsigned size() const { return q.size(); };
private:
	queue<T> q;
};
//Problem #3: Queue using 2 Stack: O(1) dequeue
template<typename T>class QueueD1 {
public:
	void enqueue(const T& item) {
		while (!sa.empty()) { sb.push(sa.top()); sa.pop(); }
		sa.push(item);
		while (!sb.empty()) { sa.push(sb.top()); sb.pop(); }
	}

	T dequeue() {
		T value = front();
		sa.pop();
		return value;
	}

	const T& front()const {
		if (empty()) throw exception("NO");
		return sa.top();
	}

	bool empty()const { return sa.empty(); }
	unsigned size()const { return sa.size(); }
private:
	stack<T> sa, sb;
};
//Problem #4: Queue using 2 Stack: O(1) enqueue
template<typename T>class QueueE1 {
public:
	void enqueue(const T& item) {
		sa.push(item);
	}

	T dequeue() {
		T value = front();
		sb.pop();
		return value;
	}

	const T& front() {
		if (empty()) throw exception("NO");
		if (sb.empty()) {
			while (!sa.empty()) { sb.push(sa.top()); sa.pop(); }
		}
		return sb.top();
	}

	bool empty()const { return size() == 0; }
	unsigned size()const { return sa.size() + sb.size(); }
private:
	stack<T> sa, sb;
};
#pragma endregion

#pragma region MCS: Problem Set 2 Solutions
//Problem #1: Priority Queue
template<typename T>class PriorityQueue {
public:

	explicit PriorityQueue(unsigned priorties = 3) : v{ priorties } {};

	void enqueue(const T& item, unsigned priority) {
		v[v.size() - priority].push(item);
	}

	T dequeue() {
		T value = front();
		for (auto& q : v) {
			if (!q.empty()) {
				q.pop();
				return value;
			}
		}
	}
	const T& front() {
		if (empty()) throw exception("NO");
		for (auto& q : v) {
			if (!q.empty()) return q.front();
		}
	}

	bool empty() const { return size() == 0; }
	unsigned size() const {
		unsigned s = 0;
		for (auto& q : v) {
			s += q.size();
		}
		return s;
	};
private:
	vector<queue<T>>v{3};
};
//Problem #3: Sum of last K numbers (stream)
class Last_k_numbers_sum_stream {
private:
	int k;
	int sum{};
	queue<int> q;

public:
	Last_k_numbers_sum_stream(int k) :k(k) {};

	int next(int new_num) {
		if (q.size() == k) {
			sum -= q.front();
			q.pop();
		}
		sum += new_num;
		q.push(new_num);
		return sum;
	}
};
#pragma endregion
