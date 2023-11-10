#include<initializer_list>
#include<stdexcept>

class CircularVectorEmpty : public std::runtime_error {
public:
	explicit CircularVectorEmpty(const std::string& message) :runtime_error(message) {};
};

template <typename T> class CircularVector {
public:
	using value_type = T;
	using size_type = unsigned;

#pragma region The Class Interface

	explicit CircularVector(size_type size = 0, const value_type& default_value = {}) {
		resize(size, default_value);
	}

	~CircularVector() { destroy(); }

	CircularVector(const std::initializer_list<value_type>& init) { push_back(init); }

	CircularVector(const CircularVector& vector) {
		if (&vector == this) return;
		reserve(vector.capacity());
		push_back(vector);
	}

	CircularVector(CircularVector&& vector) { move_vector(vector); }

	CircularVector& operator=(const CircularVector& vector) {
		if (&vector == this) return;
		clear();
		return push_back(vector);
	}

	CircularVector& operator=(CircularVector&& vector) {
		move_vector(vector);
		return *this;
	}

	bool operator==(const CircularVector& vector) {
		if (size() != vector.size()) return false;
		for (auto i = 0; i < size(); ++i) {
			if (operator[](i) != vector[i]) return false;
		}
		return true;
	}

	bool operator!=(const CircularVector& vector) { return !(*this == vector); }

#pragma endregion

#pragma region The Standard Interface

	value_type& operator[] (size_type index) { return arr[advance(first, index)]; }
	const value_type& operator[] (size_type index) const { return arr[advance(first, index)]; }


	value_type& at(size_type index) throw (std::out_of_range) {
		if (index >= size()) throw std::out_of_range("Invalid Operation: index is out of range");
		return arr[advance(first, index)];
	}

	const value_type& at(size_type index) const throw (std::out_of_range) {
		if (index >= size()) throw std::out_of_range("Invalid Operation: index is out of range");
		return arr[advance(first, index)];
	}

	CircularVector& push_back(const CircularVector<value_type>& items) {
		reserve(size() + items.size());
		for (auto i = 0; i < items.size(); ++i) push_back(items[i]);
		return *this;
	};

	CircularVector& push_back(const std::initializer_list<value_type>& items) {
		reserve(size() + items.size());
		for (auto& item : items) push_back(item);
		return *this;
	};

	CircularVector& push_back(const value_type& item) {
		if (full()) reserve(std::max((unsigned)1, 2 * capacity()));
		arr[rear()] = item;
		++length;
		return *this;
	};
	
	CircularVector& push_front(const CircularVector<value_type>& items) {
		reserve(size() + items.size());
		for (auto i = 0; i < items.size(); ++i) push_front(items[i]);
		return *this;
	};

	CircularVector& push_front(const std::initializer_list<value_type>& items) {
		reserve(size() + items.size());
		for (auto& item : items) push_front(item);
		return *this;
	};

	CircularVector& push_front(const value_type& item) {
		if (full()) reserve(std::max((unsigned)1, 2 * capacity()));
		first = prev(first);
		arr[first] = item;
		++length;
		return *this;
	};

	value_type pop_back() throw(CircularVectorEmpty) {
		value_type value = back();
		--length;
		return value;
	};

	value_type pop_front() throw(CircularVectorEmpty) {
		value_type value = front();
		first = next(first);
		--length;
		return value;
	};

	const value_type& front() const throw(CircularVectorEmpty) {
		if (empty()) throw CircularVectorEmpty("Invalid Operation: vector is empty");
		return arr[first];
	};

	const value_type& back() const throw(CircularVectorEmpty) {
		if (empty()) throw CircularVectorEmpty("Invalid Operation: vector is empty");
		return arr[last()];
	};

	size_type size() const { return length; };

	size_type capacity() const { return max_length; };

	bool empty() const { return size() == 0; };

	bool full() const { return size() == capacity(); };

	CircularVector& clear() {
		length = 0;
		first = 0;
		return *this;
	}

	CircularVector& reserve(size_type new_capacity) {
		if (new_capacity > capacity()) set_capacity(new_capacity);
		return *this;
	};

	CircularVector& resize(size_type new_size, const value_type& default_value = {}) {
		if (new_size <= size()) { return *this; }
		reserve(new_size);
		if (default_value != value_type{}) {
			for (size_type i = size(); i < new_size; ++i) {
				operator[](i) = default_value;
			}
		}
		length = new_size;
		return *this;
	}

	CircularVector& shrink() {
		set_capacity(size());
		return *this;
	};

	void swap(CircularVector& vector) {
		std::swap(length, vector.length);
		std::swap(max_length, vector.max_length);
		std::swap(arr, vector.arr);
		std::swap(first, vector.first);
	}

	CircularVector& assign(const std::initializer_list<value_type>& items) {
		clear();
		return push_back(items);
	}

#pragma endregion

#pragma region The Functional Interface

	template<typename F>
	CircularVector& filter_self(F& predicate) {
		return (*this = filter(predicate));
		return *this;
	}

	template<typename F>
	CircularVector filter(F& predicate) {
		CircularVector result;
		result.reserve(size());
		for (auto i = 0; i < size(); ++i) {
			if (predicate(operator[](i))) result.push_back(operator[](i));
		}
		return result;
	}

	template <typename T2>
	CircularVector<T2> map(T2(*transform) (const value_type&)) {
		CircularVector<T2> result;
		result.reserve(size());
		for (auto i = 0; i < size(); ++i) {
			result.push_back(transform(operator[](i)));
		}
		return result;
	}

	template <typename T2>
	T2 reduce(const T2& init, T2(*accumulate) (const T2&, const value_type&)) {
		T2 result = init;
		for (auto i = 0; i < size(); ++i) {
			result = accumulate(result, operator[](i));
		}
		return result;
	}

	template <typename F>
	bool exists(F& predicate) {
		for (auto i = 0; i < size(); ++i) {
			if (predicate(operator[](i))) return true;
		}
		return false;
	}

	CircularVector& remove_all(const value_type& item) {
		auto pred = [=](auto i) {return i != item; };
		return filter_self(pred);
	}
#pragma endregion

private:
	value_type* arr{};
	size_type length{ 0 };
	size_type max_length{ 0 };
	size_type first{ 0 };

#pragma region Helper Functions

	size_type advance(size_type pos, int steps) const {
		return (pos + steps + max_length) % max_length;
	}

	size_type next(size_type pos) const { return advance(pos, 1); }

	size_type prev(size_type pos) const { return advance(pos, -1); }
	
	size_type rear() const { return advance(first, length); }

	size_type last() const { return advance(first, length - 1); }

	void set_capacity(size_type new_capacity) {
		if (new_capacity == 0) return destroy();
		value_type* new_arr = new value_type[new_capacity]{};
		size_type len = length;
		for (size_type i = 0; i < len; ++i) new_arr[i] = operator[](i);
		destroy();
		arr = new_arr;
		length = len;
		max_length = new_capacity;
	}

	void destroy() {
		if (arr) delete[] arr;
		arr = nullptr;
		length = 0;
		max_length = 0;
		first = 0;
	}

	void move_vector(CircularVector& vector) {
		if (&vector == this) return;
		destroy();
		arr = vector.arr;
		length = vector.length;
		max_length = vector.max_length;
		first = vector.first;

		vector.arr = nullptr;
		vector.length = 0;
		vector.max_length = 0;
		vector.first = 0;
	}
#pragma endregion

};