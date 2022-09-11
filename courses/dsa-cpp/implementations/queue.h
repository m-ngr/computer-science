#include<stdexcept>
#pragma once
// Queue ADT

class QueueEmpty : public std::runtime_error {
public:
	explicit QueueEmpty(const std::string& message) : runtime_error(message) {};
};

template<typename T> class Queue {
public:
	virtual void enqueue(const T& item) = 0;
	virtual T dequeue() throw(QueueEmpty) = 0;
	virtual const T& front() const throw(QueueEmpty) = 0;
	virtual unsigned size() const = 0;
	virtual bool empty() const = 0;
};