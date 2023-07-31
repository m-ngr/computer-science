#include<stdexcept>
#pragma once
// Stack ADT

class StackEmpty : public std::runtime_error {
public:
	explicit StackEmpty(const std::string& message) : runtime_error(message) {};
};

template<typename T> class Stack {
public:
	virtual void push(const T& item) = 0;
	virtual T pop() throw(StackEmpty) = 0;
	virtual const T& top() const throw(StackEmpty) = 0;
	virtual unsigned size() const = 0;
	virtual bool empty() const = 0;
};