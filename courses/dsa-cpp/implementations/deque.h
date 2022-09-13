#pragma once
#include<stdexcept>
// Deque ADT

class DequeEmpty : public std::runtime_error {
public:
	explicit DequeEmpty(const std::string& message) : runtime_error(message) {};
};

template<typename T>class Deque {
public:
	virtual void enqueue_front(const T& item) = 0;
	virtual void enqueue_rear(const T& item) = 0;
	virtual T dequeue_front() throw(DequeEmpty) = 0;
	virtual T dequeue_rear() throw(DequeEmpty) = 0;

	virtual const T& front()const throw(DequeEmpty) = 0;
	virtual const T& rear() const throw(DequeEmpty) = 0;

	virtual bool empty()const = 0;
	virtual unsigned size()const = 0;
};