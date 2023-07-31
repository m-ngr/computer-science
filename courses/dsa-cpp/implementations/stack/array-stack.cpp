#include "stack.h"

class StackFull : public std::runtime_error {
public:
	explicit StackFull(const std::string& message) : runtime_error(message) {};
};

template <typename T> class ArrayStack : public Stack<T> {
public:

#pragma region The Class Interface
	explicit ArrayStack(unsigned capacity = 10) {
		cap = capacity;
		arr = new T[cap]{};
	}

	explicit ArrayStack(const std::initializer_list<T>& init)
		:ArrayStack(init.size()) {
		push(init);
	}

	~ArrayStack() { delete[] arr; }

	ArrayStack(const ArrayStack& stack) { copy_stack(stack); }

	ArrayStack(ArrayStack&& stack) { move_stack(stack); }

	ArrayStack& operator=(const ArrayStack& stack) {
		copy_stack(stack);
		return *this;
	}

	ArrayStack& operator=(ArrayStack&& stack) {
		move_stack(stack);
		return *this;
	}

#pragma endregion

#pragma region The Standard Interface
	virtual void push(const T& item) override {
		if (full()) throw StackFull("Invalid Operation: stack is full");
		arr[len] = item;
		++len;
	}

	virtual T pop() throw(StackEmpty)override {
		T value = top();
		--len;
		return value;
	}

	virtual const T& top() const throw(StackEmpty)override {
		if (empty()) throw StackEmpty("Invalid Operation: stack is empty");
		return arr[len - 1];
	}

	virtual unsigned size() const override { return len; }
	virtual bool empty() const override { return size() == 0; }
#pragma endregion

#pragma region The Extended Interface
	void push(const std::initializer_list<T>& items) {
		for (const T& item : items) push(item);
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
	T* arr {};
	unsigned len{ 0 };
	unsigned cap{ 0 };

#pragma region Helpers
	void set_capacity(unsigned new_capacity) {
		T* new_arr = new T[new_capacity]{};
		for (auto i = 0; i < size(); ++i) new_arr[i] = arr[i];
		if (arr) delete[] arr;
		arr = new_arr;
		cap = new_capacity;
	}

	void copy_stack(const ArrayStack& stack) {
		len = 0;
		set_capacity(stack.capacity());
		len = stack.size();
		for (auto i = 0; i < size(); ++i) arr[i] = stack.arr[i];
	}

	void move_stack(ArrayStack& stack) {
		len = stack.len;
		cap = stack.cap;
		arr = stack.arr;

		stack.len = 0;
		stack.cap = 0;
		stack.arr = nullptr;
	}
#pragma endregion
};