#include "deque.hpp"

class DequeFull : public std::runtime_error {
public:
	explicit DequeFull(const std::string& message) : runtime_error(message) {};
};

template<typename T> class ArrayDeque : public Deque<T> {
public:

#pragma region The Class Interface
	explicit ArrayDeque(unsigned capacity = 10) {
		cap = capacity;
		arr = new T[cap]{};
	};

	explicit ArrayDeque(const std::initializer_list<T>& init)
		:ArrayDeque(init.size()) {
		enqueue_rear(init);
	}

	~ArrayDeque() { delete[] arr; }

	ArrayDeque(const ArrayDeque& deque) { copy_deque(deque); }

	ArrayDeque(ArrayDeque&& deque) { move_deque(deque); }

	ArrayDeque& operator=(const ArrayDeque& deque) {
		copy_deque(deque);
		return *this;
	}

	ArrayDeque& operator=(ArrayDeque&& deque) {
		move_deque(deque);
		return *this;
	}

#pragma endregion

#pragma region The Standard Interface
	virtual void enqueue_front(const T& item) override {
		if (full()) throw DequeFull("Invalid Operation: deque is full");
		first = prev(first);
		arr[first] = item;
		++length;
	}

	virtual void enqueue_rear(const T& item) override {
		if (full()) throw DequeFull("Invalid Operation: deque is full");
		arr[last()] = item;
		++length;
	}

	virtual T dequeue_front()throw(DequeEmpty) override {
		T value = front();
		first = next(first);
		--length;
		return value;
	}

	virtual T dequeue_rear()throw(DequeEmpty)override {
		T value = rear();
		--length;
		return value;
	}

	virtual const T& front()const throw(DequeEmpty)override {
		if (empty()) throw DequeEmpty("Invalid Operation: Deque is empty");
		return arr[first];
	}

	virtual const T& rear()const throw(DequeEmpty)override {
		if (empty()) throw DequeEmpty("Invalid Operation: Deque is empty");
		return arr[prev(last())];
	}

	virtual bool empty()const override { return size() == 0; }

	virtual unsigned size()const override { return length; }
#pragma endregion

#pragma region The Extended Interface
	void enqueue_front(const std::initializer_list<T>& items) {
		for (const T& item : items) enqueue_front(item);
	};

	void enqueue_rear(const std::initializer_list<T>& items) {
		for (const T& item : items) enqueue_rear(item);
	};

	unsigned capacity() const { return cap; }

	bool full() const { return size() == capacity(); }

	void reserve(unsigned new_capacity) {
		if (new_capacity <= capacity()) return;
		set_capacity(new_capacity);
	}

	void shrink() { set_capacity(size()); }
#pragma endregion

private:
	T* arr{};
	unsigned cap{ 0 };
	unsigned length{ 0 };
	unsigned first{ 0 };

#pragma region Helpers
	void set_capacity(unsigned new_capacity) {
		T* new_arr = new T[new_capacity]{};
		for (auto i = 0; i < size(); ++i) {
			new_arr[i] = arr[first];
			first = next(first);
		}
		if (arr) delete[] arr;
		arr = new_arr;
		cap = new_capacity;
		first = 0;
	}

	void copy_deque(const ArrayDeque& deque) {
		length = 0;
		set_capacity(deque.capacity());
		length = deque.size();
		for (auto i = 0; i < size(); ++i) arr[i] = deque.arr[i];
	}

	void move_deque(ArrayDeque& deque) {
		length = deque.length;
		cap = deque.cap;
		arr = deque.arr;
		first = deque.first;

		deque.length = 0;
		deque.cap = 0;
		deque.first = 0;
		deque.arr = nullptr;
	}

	unsigned advance(unsigned pos, int steps) const {
		return (cap + pos + steps) % cap;
	}
	unsigned next(unsigned pos) const { return advance(pos, 1); }
	unsigned prev(unsigned pos) const { return advance(pos, -1); }
	unsigned last() const { return advance(first, length); }

#pragma endregion

};