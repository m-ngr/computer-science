#include<initializer_list>
#include<stdexcept>

template <typename T> class SLList {
	class Node; // private: Implementation Details
public:
	class Iterator;
	using value_type = T;
	using size_type = unsigned;

#pragma region The Class Interface
	SLList() {}

	~SLList() { remove_list(); }

	explicit SLList(size_type number, const value_type& default_value = {}) {
		for (auto i = 0; i < number; ++i) { push_back(default_value); }
	}
	
	SLList(const std::initializer_list<value_type>& init) { push_back(init); }

	SLList(const SLList& list) { push_back(list); }

	SLList(SLList&& list) { move_list(list); }

	SLList& operator=(const SLList& list) {
		remove_list();
		push_back(list);
		return *this;
	}

	SLList& operator=(SLList&& list) {
		move_list(list);
		return *this;
	}

	bool operator==(const SLList& list) {
		if (size() != list.size()) return false;
		for (auto i = begin(), j = list.begin(); i != end() && j != list.end(); ++i, ++j) {
			if (*i != *j) return false;
		}
		return true;
	}

	bool operator!=(const SLList& list) { return !(*this == list); }

#pragma endregion

#pragma region The Standard Interface
	SLList& push_back(const value_type& item) {
		if (empty()) {
			head = tail = create_node(item);
		}else {
			add_next(tail, item);
		}
		return *this;
	}

	SLList& push_back(const SLList& list) {
		for (auto& item : list) {
			push_back(item);
		}
		return *this;
	}

	SLList& push_back(const std::initializer_list<value_type>& init_list) {
		for (auto& item : init_list) {
			push_back(item);
		}
		return *this;
	}

	SLList& push_front(const value_type& item) {
		Node* newNode = create_node(item, head);
		if (empty()) tail = newNode;
		head = newNode;
		return *this;
	}

	SLList& push_front(const SLList& list) {
		for (auto& item : list) {
			push_front(item);
		}
		return *this;
	}

	SLList& push_front(const std::initializer_list<value_type>& init_list) {
		for (auto& item : init_list) {
			push_front(item);
		}
		return *this;
	}

	value_type pop_back() throw (std::runtime_error) {
		if (empty()) { throw std::runtime_error("Can't pop items from empty list"); }

		if (tail == head) {
			value_type result = tail->value;
			head = tail = remove_node(tail);
			return result;
		}else {
			tail = node_pre(tail);
			return remove_next(tail);
		}
	}

	value_type pop_front() throw (std::runtime_error) {
		if (empty()) { throw std::runtime_error("Can't pop items from empty list"); }
		value_type result = head->value;
		head = remove_node(head);
		if (!head) tail = nullptr;
		return result;
	}

	SLList& reverse() {
		Node* prev = nullptr, * curr = head;
		while (curr) {
			Node* next = curr->next;
			curr->next = prev;
			prev = curr;
			curr = next;
		}
		std::swap(head, tail);
		return *this;
	}

	const value_type& front() const throw(std::runtime_error){
		if (empty()) { throw std::runtime_error("Can't read values from empty list"); }
		return head->value;
	}

	const value_type& back() const throw(std::runtime_error) {
		if (empty()) { throw std::runtime_error("Can't read values from empty list"); }
		return tail->value;
	}

	bool empty()const { return (head == nullptr || tail == nullptr); }

	Iterator begin() { return Iterator(head); }

	Iterator end() { return Iterator(nullptr); }

#pragma endregion

#pragma region The Index Interface

	SLList& insert(size_type index, const value_type& item) {
		if (index == 0) return push_front(item);
		if (index == size()) return push_back(item);
		add_next(node_at(index - 1), item);
		return *this;
	}

	value_type remove(size_type index) {
		if (index == 0) return pop_front();
		return remove_next(node_at(index - 1));
	}

	size_type size() const { return length; }

	const value_type& at(size_type index) const { return node_at(index)->value; }

	value_type& at(size_type index) { return node_at(index)->value; }

	const value_type& operator[](size_type index) const { return begin()[index].value; }

	value_type& operator[](size_type index) { return begin()[index].value; }

#pragma endregion

#pragma region The Functional Interface

	template<typename F>
	SLList& filter_self(F& predicate) {
		Node* prev = nullptr, * curr = head;
		while (curr) {
			if (!predicate(curr->value)) {
				if (prev) {
					remove_next(prev);
					curr = prev->next;
				}
				else {
					pop_front();
					curr = head;
				}
			}
			else {
				prev = curr;
				curr = curr->next;
			}
		}
		return *this;
	}
	
	template<typename F>
	SLList filter(F& predicate) {
		SLList result;
		for (auto item : *this) {
			if (predicate(item)) result.push_back(item);
		}
		return result;
	}

	template <typename T2>
	SLList<T2> map(T2(*transform) (const value_type&)) {
		SLList<T2> result;
		for (auto item : *this) {
			result.push_back(transform(item));
		}
		return result;
	}

	template <typename T2>
	T2 reduce(const T2& init, T2(*accumulate) (const T2&, const value_type&)) {
		T2 result = init;
		for (auto item : *this) {
			result = accumulate(result, item);
		}
		return result;
	}

	template <typename F>
	bool exists(F& predicate) {
		for (auto item : *this) {
			if (predicate(item)) return true;
		}
		return false;
	}

	SLList& remove_all(const value_type& item) {
		auto pred = [=](auto i) {return i != item; };
		return filter_self(pred);
	}
#pragma endregion

	SLList& remove_first(const value_type& value) throw (std::exception){
		if (empty()) throw std::exception("Can't remove a value from empty list");

		if (head->value == value) {
			pop_front();
			return *this;
		}

		for (auto prev = head, curr = head->next; curr; prev = curr, curr = curr->next) {
			if (curr->value == value) {
				remove_next(prev);
				return *this;
			}
		}
	}

	SLList& remove_last(const value_type& value) throw (std::exception) {
		if (empty()) throw std::exception("Can't remove a value from empty list");
		
		if (tail->value == value) { 
			pop_back(); 
			return *this;
		}

		Node* last = tail;

		for (Node* curr = head, *prev = nullptr; curr; prev = curr, curr = curr->next) {
			if (curr->value == value) last = prev;
		}

		if (last == nullptr) pop_front();
		else if (last != tail) remove_next(last);

		return *this;
	}

private:
	Node* head{};
	Node* tail{};
	size_type length{ 0 };

#pragma region Helpers
	void move_list(SLList& list) {
		length = list.length;
		head = list.head;
		tail = list.tail;

		list.length = 0;
		list.head = nullptr;
		list.tail = nullptr;
	}
	
	void remove_list() {
		while (head) {
			Node* tmp = head;
			head = head->next;
			delete tmp;
		}
		length = 0;
		tail = head = nullptr;
	}

	Node* create_node(const value_type& value = {}, Node* next = {}) {
		Node* newNode = new Node{ value, next };
		++length;
		return newNode;
	}

	Node* remove_node(Node* target) {
		Node* next = target->next;
		delete target;
		--length;
		return next;
	}

	Node* node_pre(Node* node) {
		if (!node) return tail;
		if (node == head) return nullptr;
		Node* pre = head;
		while (pre && pre->next != node) { pre = pre->next; }
		return pre;
	}

	value_type remove_next(Node* node) throw(std::runtime_error) {
		if (!node || !node->next) throw std::runtime_error("No next node to remove");
		Node* target = node->next;
		value_type result = target->value;
		if (target == tail) tail = node;
		node->next = remove_node(target);
		return result;
	}
	
	Node* add_next(Node* node, const value_type& value) throw(std::runtime_error) {
		if (!node) throw std::runtime_error("Can't add node to null");
		Node* newNode = create_node(value, node->next);
		node->next = newNode;
		if (node == tail) tail = newNode;
		return newNode;
	}
	
	Node* node_at(size_type i) const throw(std::out_of_range) {
		if (i >= size()) throw std::out_of_range("Index out of range");
		Node* element = head;
		while (i != 0 && element != nullptr) {
			element = element->next;
			--i;
		}
		return element;
	}

#pragma endregion
};

template <typename T> class SLList<T>::Node {
public:
	Node(const value_type& value = {}, Node* next = {})
		: value{ value }, next{ next }{};
	value_type value{};
	Node* next{};
};

template <typename T> class SLList<T>::Iterator {
public:
	Iterator(Node* ptr = {}) : ptr{ ptr } {};

	Iterator& operator++() {
		ptr = ptr->next;
		return *this;
	}

	Iterator& go_next() throw (std::runtime_error) {
		if (ptr == nullptr) {throw std::runtime_error("Can't get the next of a null node");}
		return operator++();
	}

	value_type& operator*() { return ptr->value; }

	value_type& value() throw (std::runtime_error) {
		if (ptr == nullptr) { throw std::runtime_error("Can't get a value of a null node"); }
		return operator*();
	}

	Iterator& value(const value_type& value) throw (std::runtime_error) {
		if (ptr == nullptr) { throw std::runtime_error("Can't set a value for a null node"); }
		ptr->value = value;
		return *this;
	}

	Iterator next() throw (std::runtime_error) {
		if (ptr == nullptr) { throw std::runtime_error("Can't get the next of a null node"); }
		return Iterator(ptr->next);
	}

	Iterator& next(const Iterator& it) throw (std::runtime_error) {
		if (ptr == nullptr) { throw std::runtime_error("Can't get the next of a null node"); }
		ptr->next = it.ptr;
		return *this;
	}

	Iterator operator+(size_type index) {
		auto curr = ptr;
		while (index--) curr = curr->next;
		return Iterator(curr);
	}

	value_type& operator[](size_type index) {
		auto curr = ptr;
		while (index--) curr = curr->next;
		return curr->value;
	}

	bool operator==(const Iterator& it) const { return ptr == it.ptr; }
	bool operator==(Node* node) const { return ptr == node; }

	bool operator!=(const Iterator& it)  const{ return ptr != it.ptr; }
	bool operator!=(Node* node) const { return ptr != node; }

	explicit operator bool() const { return ptr != nullptr; }

private:
	Node* ptr{};
};
