#include "Deque.h"
#include<vector>

template<typename T>class VectorDeque: public Deque<T> {
public:
	VectorDeque() {};
	VectorDeque(const std::initializer_list<T>& init) 
		:v{ init }, length{init.size()} {};

	void enqueue_front(const std::initializer_list<T>& items) {
		for (const T& item : items) enqueue_front(item);
	};

	void enqueue_rear(const std::initializer_list<T>& items) {
		for (const T& item : items) enqueue_rear(item);
	};

	virtual void enqueue_front(const T& item) override  {
		if (size() < v.size()) {
			first = prev(first);
			v[first] = item;
			++length;
		} else {
			normalize({item});
		}
	}

	virtual void enqueue_rear(const T& item) override {
		if (size() < v.size()) {
			v[last()] = item;
			++length;
		} else {
			normalize({}, {item});
		}
	}

	virtual T dequeue_front()throw(DequeEmpty)override {
		T value = front();
		first = next(first);
		--length;
		return value;
	}
	virtual T dequeue_rear()throw(DequeEmpty)override {
		T value = rear();
		--length;
		return value;
	}

	virtual const T& front()const throw(DequeEmpty)override {
		if (empty()) throw DequeEmpty("Invalid Operation: Deque is empty");
		return v[first];
	}

	virtual const T& rear()const throw(DequeEmpty)override {
		if (empty()) throw DequeEmpty("Invalid Operation: Deque is empty");
		return v[prev(last())];
	}

	virtual bool empty()const override { return size() == 0; }
	
	virtual unsigned size()const override { return length; }

private:
	std::vector<T> v;
	unsigned first{ 0 };
	unsigned length{ 0 };

	unsigned advance(unsigned pos, int steps) const {
		return (v.size() + pos + steps) % v.size();
	}

	unsigned next(unsigned pos) const { return advance(pos, 1); }
	unsigned prev(unsigned pos) const { return advance(pos, -1); }
	unsigned last() const { return advance(first, length); }

	void normalize(const std::vector<T>& front_items = {}, const std::vector<T>& back_items = {}) {
		int fs = front_items.size();
		int bs = back_items.size();

		if (first == 0 && fs == 0 && bs == 0) return;

		std::vector<T> new_v(2 * size() + fs + bs);
		// copy front_items
		for (int i = 0; i < fs; ++i) new_v[i] = front_items[i];
		// copy current deque items
		for (int i = fs; i < (fs + size()); ++i) {
			new_v[i] = v[first];
			first = next(first);
		}
		// copy back_items
		int k = fs + size();
		for (int i = k; i < (k + bs); ++i) {
			new_v[i] = back_items[i - k];
		}
		// update state
		v = new_v;
		first = 0;
		length = length + fs + bs;
	}
};