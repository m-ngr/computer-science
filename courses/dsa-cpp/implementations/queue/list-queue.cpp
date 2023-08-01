#include "queue.hpp"
#include<list>

template<typename T> class ListQueue : public Queue<T> {
public:
	ListQueue() {};

	explicit ListQueue(const std::initializer_list<T>& init) : l{init} {};

	void enqueue(const std::initializer_list<T>& items) {
		for (const T& item : items) enqueue(item);
	}
	
	virtual void enqueue(const T& item) override {
		l.push_back(item);
	}

	virtual T dequeue() throw(QueueEmpty)override {
		T value = front();
		l.pop_front();
		return value;
	}

	virtual const T& front() const throw(QueueEmpty)override {
		if (empty()) throw QueueEmpty("Invalid Operation: queue is empty");
		return l.front();
	}

	virtual unsigned size() const override { return l.size(); }
	virtual bool empty() const override { return l.empty(); }
private:
	std::list<T> l{};

};