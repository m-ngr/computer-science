#include "Queue.h"

class QueueFull : public std::runtime_error {
public:
	explicit QueueFull(const std::string& message) : runtime_error(message) {};
};

template<typename T> class ArrayQueue : public Queue<T> {
public:

#pragma region The Class Interface
	explicit ArrayQueue(unsigned capacity = 10) {
		cap = capacity;
		arr = new T[cap]{};
	};

	explicit ArrayQueue(const std::initializer_list<T>& init)
		:ArrayQueue(init.size()) {
		enqueue(init);
	}

	~ArrayQueue() { delete[] arr; }

	ArrayQueue(const ArrayQueue& queue) { copy_queue(queue); }

	ArrayQueue(ArrayQueue&& queue) { move_queue(queue); }

	ArrayQueue& operator=(const ArrayQueue& queue) {
		copy_queue(queue);
		return *this;
	}

	ArrayQueue& operator=(ArrayQueue&& queue) {
		move_queue(queue);
		return *this;
	}

#pragma endregion

#pragma region The Standard Interface
	virtual void enqueue(const T& item) override {
		if (full()) throw QueueFull("Invalid Operation: queue is full");
		arr[last()] = item;
		++length;
	}

	virtual T dequeue() throw(QueueEmpty)override {
		T value = front();
		--length;
		first = next(first);
		return value;
	}

	virtual const T& front() const throw(QueueEmpty)override {
		if (empty()) throw QueueEmpty("Invalid Operation: queue is empty");
		return arr[first];
	}

	virtual unsigned size() const override { return length; }

	virtual bool empty() const override { return size() == 0; }
#pragma endregion

#pragma region The Extended Interface
	void enqueue(const std::initializer_list<T>& items) {
		for (const T& item : items) enqueue(item);
	}

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

	void copy_queue(const ArrayQueue& queue) {
		length = 0;
		set_capacity(queue.capacity());
		length = queue.size();
		for (auto i = 0; i < size(); ++i) arr[i] = queue.arr[i];
	}

	void move_queue(ArrayQueue& queue) {
		length = queue.length;
		cap = queue.cap;
		arr = queue.arr;
		first = queue.first;

		queue.length = 0;
		queue.cap = 0;
		queue.first = 0;
		queue.arr = nullptr;
	}

	unsigned advance(unsigned pos, unsigned steps) { return (pos + steps) % cap; }

	unsigned next(unsigned pos) { return advance(pos, 1); }

	unsigned last() { return advance(first, length); }
#pragma endregion

};