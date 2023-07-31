#include "Stack.h"
#include<vector>

template <typename T> class VectorStack : public Stack<T> {
public:
	VectorStack() {};
	explicit VectorStack(std::initializer_list<T> init) :v{init} {}

	virtual void push(const T& item) override { v.push_back(item); }
	
	void push(const std::initializer_list<T>& items){
		for (const T& item : items) push(item);
	}

	virtual T pop() throw(StackEmpty) override {
		T value = top();
		v.pop_back();
		return value;
	}

	virtual const T& top() const throw(StackEmpty) override {
		if (empty()) throw StackEmpty("Invalid Operation: Stack is empty");
		return v.back();
	}

	virtual unsigned size() const override { return v.size(); }
	virtual bool empty() const override { return v.empty(); }
private:
	std::vector<T> v{};
};