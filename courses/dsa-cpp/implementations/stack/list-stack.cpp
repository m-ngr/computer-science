#include "stack.hpp"
#include<list>

template <typename T> class ListStack : public Stack<T> {
public:
	ListStack() {};
	ListStack(const std::initializer_list<T>& init) : l{ init } {};

	virtual void push(const T& item) override { l.push_back(item); }

	void push(const std::initializer_list<T>& items) {
		for (const T& item: items) push(item);
	}

	virtual T pop() throw(StackEmpty) override {
		T value = top();
		l.pop_back();
		return value;
	}

	virtual const T& top() const throw(StackEmpty)override {
		if (empty()) throw StackEmpty("Invalid Operation: Stack is empty");
		return l.back();
	}

	virtual unsigned size() const override { return l.size(); }
	virtual bool empty() const override { return l.empty(); }
private:
	std::list<T> l{};
};