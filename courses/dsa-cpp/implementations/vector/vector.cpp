#include<initializer_list>
#include<stdexcept>

class VectorEmpty : public std::runtime_error {
public:
	explicit VectorEmpty(const std::string& message) :runtime_error(message) {};
};

template <typename T> class Vector {
public:
	using value_type = T;
	using size_type = unsigned;
	using iterator = T*;
	using const_iterator = const iterator;

#pragma region The Class Interface

	explicit Vector(size_type size = 0, const value_type& default_value = {}){
		resize(size, default_value);
	}

	~Vector() { destroy(); }

	Vector(const std::initializer_list<value_type>& init) { push_back(init); }

	Vector(const Vector& vector){
		if (&vector == this) return;
		reserve(vector.capacity());
		push_back(vector);
	}

	Vector(Vector&& vector) { move_vector(vector); }

	Vector& operator=(const Vector& vector) {
		if (&vector == this) return;
		clear();
		return push_back(vector);
	}

	Vector& operator=(Vector&& vector) {
		move_vector(vector);
		return *this;
	}

	bool operator==(const Vector& vector) {
		if (size() != vector.size()) return false;
		for (auto i = begin(), j = vector.begin(); i != end() && j != vector.end(); ++i, ++j) {
			if (*i != *j) return false;
		}
		return true;
	}

	bool operator!=(const Vector& vector) { return !(*this == vector); }

#pragma endregion

#pragma region The Standard Interface

	value_type& operator[] (size_type index) { return arr[index]; }
	const value_type& operator[] (size_type index) const { return arr[index]; }


	value_type& at(size_type index) throw (std::out_of_range){
		if (index >= size()) throw std::out_of_range("Invalid Operation: index is out of range");
		return arr[index];
	}
	
	const value_type& at(size_type index) const throw (std::out_of_range){
		if (index >= size()) throw std::out_of_range("Invalid Operation: index is out of range");
		return arr[index];
	}

	Vector& push_back(const Vector<value_type>& items) {
		reserve(size() + items.size());
		for (auto& item : items) push_back(item);
		return *this;
	};

	Vector& push_back(const std::initializer_list<value_type>& items) {
		reserve(size() + items.size());
		for (auto& item : items) push_back(item);
		return *this;
	};

	Vector& push_back(const value_type& item) {
		if (full()) reserve(std::max((unsigned)1, 2 * capacity()));
		arr[size()] = item;
		++length;
		return *this;
	};

	value_type pop_back() throw(VectorEmpty){
		value_type value = back();
		--length;
		return value;
	};

	const value_type& front() const throw(VectorEmpty){
		if (empty()) throw VectorEmpty("Invalid Operation: vector is empty");
		return arr[0];
	};

	const value_type& back() const throw(VectorEmpty){
		if (empty()) throw VectorEmpty("Invalid Operation: vector is empty");
		return arr[size() - 1];
	};

	size_type size() const { return length; };

	size_type capacity() const { return max_length; };
	
	bool empty() const { return size() == 0; };

	bool full() const { return size() == capacity(); };

	Vector& clear() {
		length = 0;
		return *this;
	}

	Vector& reserve(size_type new_capacity) {
		if (new_capacity > capacity()) set_capacity(new_capacity);
		return *this;
	};

	Vector& resize(size_type new_size, const value_type& default_value = {}) {
		if (new_size <= size()) { return *this; }
		reserve(new_size);
		if (default_value != value_type{}) {
			for (size_type i = size(); i < new_size; ++i) {
				arr[i] = default_value;
			}
		}
		length = new_size;
		return *this;
	}

	Vector& shrink() {
		set_capacity(size());
		return *this;
	};

	void swap(Vector& vector) {
		std::swap(length, vector.length);
		std::swap(max_length, vector.max_length);
		std::swap(arr, vector.arr);
	}

	Vector& assign(const std::initializer_list<value_type>& items) {
		clear();
		return push_back(items);
	}

#pragma endregion

#pragma region The Iterator Interface
	iterator begin() { return arr; }
	iterator end() { return arr + length; }

	const_iterator begin() const { return arr; }
	const_iterator end() const { return arr + length; }

	template<typename Iterator> void assign(Iterator first, Iterator last) {
		clear();
		while (first != last) {
			push_back(*first);
			++first;
		}
	}

	iterator insert(iterator pointer, const value_type& value) {
		return insert(pointer, { value });
	}

	iterator insert(iterator pointer, const std::initializer_list<value_type>& list) {
		if (empty() || pointer == end()) { push_back(list); return (end() - list.size()); }
		size_type index = pointer - begin();
		size_type last = size() - 1;
		resize(size() + list.size());
		//shift items beyond index
		for (auto i = last; i >= index; --i) {
			arr[i + list.size()] = arr[i];
		}
		//insert new items beyond index
		size_t i = index;
		for (const value_type& item : list) {
			arr[i] = item;
			++i;
		}
		return (begin() + index);
	}

	Vector& erase(iterator pointer) {
		return erase(pointer, pointer + 1);
	}

	Vector& erase(iterator from, iterator upto) throw (std::runtime_error){
		if (empty() || from == end() || upto < from) 
		{ throw std::runtime_error("Invalid Operation: accessing null pointer"); }

		size_type len = upto - from;
		
		if (upto != end()) {
			size_type index = upto - begin();
			for (auto i = index; i < size(); ++i) {
				arr[i - len] = arr[i];
			}
		}

		length -= len;
		return *this;
	}

	size_type index_of(const_iterator pointer) const {
		int index = pointer - begin();
		if (index < 0 || index >= size()) throw std::out_of_range("iterator not in range");
		return index;
	}

	iterator at_index(size_type index) {
		if (index < 0 || index >= size()) throw std::out_of_range("iterator not in range");
		return begin() + index;
	}

	const_iterator at_index(size_type index) const {
		if (index < 0 || index >= size()) throw std::out_of_range("iterator not in range");
		return begin() + index;
	}

#pragma endregion

#pragma region The Functional Interface

	template<typename F>
	Vector& filter_self(F& predicate) {
		return (*this = filter(predicate));
		return *this;
	}
	
	template<typename F>
	Vector filter(F& predicate) {
		Vector result;
		result.reserve(size());
		for (auto& item : *this) {
			if (predicate(item)) result.push_back(item);
		}
		return result;
	}

	template <typename T2>
	Vector<T2> map(T2(*transform) (const value_type&)) {
		Vector<T2> result;
		result.reserve(size());
		for (auto& item : *this) {
			result.push_back(transform(item));
		}
		return result;
	}

	template <typename T2>
	T2 reduce(const T2& init, T2(*accumulate) (const T2&, const value_type&)) {
		T2 result = init;
		for (auto& item : *this) {
			result = accumulate(result, item);
		}
		return result;
	}

	template <typename F>
	bool exists(F& predicate) {
		for (auto& item : *this) {
			if (predicate(item)) return true;
		}
		return false;
	}

	Vector& remove_all(const value_type& item) {
		auto pred = [=](auto i) {return i != item; };
		return filter_self(pred);
	}
#pragma endregion

private:
	value_type* arr {};
	size_type length{ 0 };
	size_type max_length{ 0 };

#pragma region Helper Functions
	void set_capacity(size_type new_capacity) {
		if (new_capacity == 0) return destroy();
		value_type* new_arr = new value_type[new_capacity]{};
		size_type len = length;
		for (size_type i = 0; i < len; ++i) new_arr[i] = arr[i];
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
	}

	void move_vector(Vector& vector) {
		if (&vector == this) return;
		destroy();
		arr = vector.arr;
		length = vector.length;
		max_length = vector.max_length;

		vector.arr = nullptr;
		vector.length = 0;
		vector.max_length = 0;
	}
#pragma endregion

};