#include "queue.hpp"
#include<vector>

template<typename T> class VectorQueue : public Queue<T> {
public:
	VectorQueue() {};

	explicit VectorQueue(const std::initializer_list<T>& init) 
		:v{ init }, length{init.size()} {};

	void enqueue(const std::initializer_list<T>& items) {
		for (const T& item : items) enqueue(item);
	}

	virtual void enqueue(const T& item) override {
		if (size() == v.size()) {
			normalize();
			v.push_back(item);
		}
		else {
			v[last()] = item;
		}
		++length;
	}

	virtual T dequeue() throw(QueueEmpty) override {
		T value = front();
		--length;
		first = next(first);
		return value;
	}

	virtual const T& front() const throw(QueueEmpty)override {
		if (empty()) throw QueueEmpty("Invalid Operation: queue is empty");
		return v[first];
	}

	virtual unsigned size() const override { return length; }
	virtual bool empty() const override { return size() == 0; }
private:
	std::vector<T> v{};
	unsigned length{ 0 };
	unsigned first{ 0 };

	unsigned advance(unsigned pos, unsigned steps) { return (pos + steps) % v.size(); }
	unsigned next(unsigned pos) { return advance(pos, 1); }
	unsigned last() { return advance(first, length); }

	void normalize() {
		if (first == 0) return;
		std::vector<T> new_v(size());
		for (auto i = 0; i < size(); ++i) {
			new_v[i] = v[first];
			first = next(first);
		}
		v = new_v;
		first = 0;
	}
};