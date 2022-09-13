#include "Deque.h"
#include <list>

template <typename T> class ListDeque : public Deque<T> {
public:
	ListDeque() {};
	ListDeque(const std::initializer_list<T>& init) :l{ init } {};

	void enqueue_front(const std::initializer_list<T>& items) {
		for (const T& item : items) enqueue_front(item);
	};

	void enqueue_rear(const std::initializer_list<T>& items) {
		for (const T& item : items) enqueue_rear(item);
	};

	virtual void enqueue_front(const T& item) override {
		l.push_front(item);
	};

	virtual void enqueue_rear(const T& item) override {
		l.push_back(item);
	};

	virtual T dequeue_front() throw(DequeEmpty)override {
		T value = front();
		l.pop_front();
		return value;
	};

	virtual T dequeue_rear() throw(DequeEmpty)override {
		T value = rear();
		l.pop_back();
		return value;
	};

	virtual const T& front()const throw(DequeEmpty)override {
		if (empty()) throw DequeEmpty("Invalid Operation: deque is empty");
		return l.front();
	};

	virtual const T& rear()const throw(DequeEmpty) override { 
		if (empty()) throw DequeEmpty("Invalid Operation: deque is empty");
		return l.back(); 
	};

	virtual bool empty()const override { return l.empty(); };
	virtual unsigned size()const override { return l.size(); };
private:
	std::list<T> l{};
};