#include<initializer_list>
#include<stdexcept>

template <typename T> class SLList {
	class Node; // private: Implementation Details
public:
	class Iterator;

#pragma region The Class Interface
	SLList() {}
	SLList(const std::initializer_list<T>& init) {
		for (const T& item : init) {
			push_back(item);
		}
	}
	~SLList() { while (!empty()) pop_front(); }
#pragma endregion

#pragma region The Standard Interface
	void push_back(const T& item) {
		Node* newNode = new Node{ item };

		if (empty()) {
			head = newNode;
		}
		else {
			tail->next = newNode;
		}

		tail = newNode;
		++length;
	}
	
	void push_front(const T& item) {
		Node* newNode = new Node{ item, head };
		if (empty()) tail = newNode;
		head = newNode;
		++length;
	}
	
	T pop_back() throw (std::exception) {
		if (empty()) { throw std::exception("Invalid Operation"); }
		Node* target = tail;
		T result = target->value;

		if (tail == head) {
			tail = nullptr;
			head = nullptr;
		}
		else {
			Node* curr = head;
			while (curr != target) {
				tail = curr;
				curr = curr->next;
			}
			tail->next = nullptr;
		}

		delete target;
		--length;
		return result;
	}
	
	T pop_front() throw (std::exception) {
		if (empty()) { throw std::exception("Invalid Operation"); }
		Node* target = head;
		T result = target->value;

		if (tail == head) {
			tail = nullptr;
			head = nullptr;
		}
		else {
			head = head->next;
		}

		delete target;
		--length;
		return result;
	}

	const T& front() const {
		if (empty()) throw std::exception("Invalid Operation");
		return head->value;
	}

	const T& back() const {
		if (empty()) throw std::exception("Invalid Operation");
		return tail->value;
	}

	void reverse() {
		Node* prev = nullptr, * curr = head;
		while (curr) {
			Node* next = curr->next;
			curr->next = prev;
			prev = curr;
			curr = next;
		}
		// swap head and tail pointers
		curr = tail;
		tail = head;
		head = curr;
	}

	bool empty() const {
		return (head == nullptr || tail == nullptr);
	}

	Iterator begin() const { return Iterator(head); }

	Iterator end() const { return Iterator(nullptr); }

#pragma endregion

#pragma region The Index Interface
	void set(size_t i, const T& value) {
		node_at(i)->value = value;
	}
	
	void insert(size_t i, const T& item) {
		if (i == 0) return push_front(item);
		if (i == length) return push_back(item);

		Node* prevNode = node_at(i - 1);
		Node* newNode = new Node{ item , prevNode->next };
		prevNode->next = newNode;
		++length;
	}

	T remove(size_t i) {
		if (i == 0) return pop_front();
		if (i == length) return pop_back();

		Node* prevNode = node_at(i - 1);
		Node* target = prevNode->next;
		T result = target->value;

		prevNode->next = target->next;
		delete target;
		--length;
		return result;
	}

	size_t size() const { return length; }

	const T& at(size_t i) const { return node_at(i)->value; }
#pragma endregion

#pragma region MCS: Problem Set 1 Solutions
	//Problem #1: Destructor [Implemented at ~SLList()]
	//Problem #2: Insert front [Implemented at push_front()]
	//Problem #3: Delete front [Implemented at pop_front()]
	//Problem #4: Get nth from back
	const T& at_back(size_t i) const {
		return node_at(size() - i)->value;
	}
	//Problem #5: Is Same
	bool is_same(const SLList<T>& sll) {
		Iterator i, j;
		for (i = begin(), j = sll.begin(); i != end() && j != sll.end(); ++i, ++j) {
			if (*i != *j) return false;
		}
		return (i == end() && j == sll.end());

	}
#pragma endregion

#pragma region MCS: Problem Set 2 Solutions
	//Problem #1: Delete with key
	void remove_value(const T& value) {
		Node* prev = nullptr, * curr = head;
		while (curr) {
			if (value == curr->value) {
				if (!prev) {
					pop_front();
					return;
				}
				prev->next = curr->next;
				delete curr;
				--length;
				return;
			}
			prev = curr;
			curr = curr->next;
		}
	}
	//Problem #2: Swap each pair values
	void swap_pairs() {
		for (Node* curr = head; curr && curr->next; curr = curr->next->next) {
			T temp = curr->value;
			curr->value = curr->next->value;
			curr->next->value = temp;
		}
	}
	//Problem #3: Reverse list nodes [Implemented at reverse()]
	//Problem #4: Delete even positions
	void delete_evens() {
		for (Node* curr = head; curr && curr->next; curr = curr->next) {
			Node* target = curr->next;
			curr->next = target->next;
			delete target;
			--length;
		}
	}
	//Problem #5: Insert to be sorted
	void insert_sorted(const T& value) {
		if (size() == 0 || value <= head->value) return push_front(value);
		if (value >= tail->value) return push_back(value);

		Node* prev = head, * curr = head->next;

		while (curr->value < value) {
			prev = curr;
			curr = curr->next;
		}

		Node* newNode = new Node{ value, curr };
		prev->next = newNode;
		++length;
	}
#pragma endregion

#pragma region MCS: Problem Set 3 Solutions
	//Problem #1: Swap head and tail
	void swap_head_tail() {
		if (head == tail) return;
		Node* pre_tail = head;
		while (pre_tail->next != tail) { pre_tail = pre_tail->next; }
		pre_tail->next = head;
		tail->next = head->next;
		head->next = nullptr;
		std::swap(head, tail);
	}
	//Problem #2: Left Rotate
	void left_rotate1(unsigned k) {
		//push-pop implementation
		for (int i = 0; i < (k % size()); ++i) {
			push_back(pop_front());
		}
	}
	void left_rotate2(unsigned k) {
		//re-linking implementation
		k = k % size();
		if (size() < 2 || k == 0) return;
		tail->next = head;
		tail = node_at(k - 1);
		head = tail->next;
		tail->next = nullptr;
	}
	//Problem #3: Remove duplicates
	void remove_duplicates() {
		Node* curr = head;
		while (curr->next) {
			Node* target = curr->next;
			bool is_dup = false;
			for (Node* it = head; it != target; it = it->next) {
				if (it->value == target->value) {
					is_dup = true;
					break;
				}
			}

			if (is_dup) {
				remove_next(curr);
			}
			else {
				curr = curr->next;
			}
		}
	}
	//Problem #4: Remove last occurance 
	void remove_last_value(const T& value) {
		Node* prev = nullptr, * curr = head, * last = tail;
		while (curr) {
			if (value == curr->value) last = prev;
			prev = curr;
			curr = curr->next;
		}
		if (last == nullptr) pop_front();
		if (last != tail) remove_next(last);
	}
	//Problem #5: Move to back!
	void move_back(const T& value) {
		Node* prev = nullptr, * curr = head, * stop = tail;

		while (curr && curr != stop) {
			if (curr->value == value) {
				if (prev) {
					push_back(remove_next(prev));
					curr = prev->next;
				}
				else {
					push_back(pop_front());
					curr = head;
				}
			}
			else {
				prev = curr;
				curr = curr->next;
			}

		}
	}
	//Problem #6: Recursive max
	T max(Node* start = nullptr) {
		if (!start) start = head;
		if (start->next == nullptr) { return start->value; };
		return std::max(start->value, max(start->next));
	}
#pragma endregion

#pragma region MCS: Problem Set 4 Solutions
	//Problem #1: Arrange odd & even nodes
	void odds_first() {
		if (size() < 3) return;
		Node* curr = head, * stop = nullptr;
		while (curr != stop && curr->next && curr->next != stop) {
			push_back(remove_next(curr));
			if (!stop) stop = tail;
			curr = curr->next;
		}
	}
	//Problem #2: Insert alternating
	void insert_alternate(const SLList& sll) {
		if (sll.empty()) return;
		if (empty()) return copy_back(sll);
		Node* curr = head;
		for (const T& value : sll) {
			if (curr) {
				curr = add_next(curr, value);
				curr = curr->next;
			}
			else {
				push_back(value);
			}
		}
	}
	void copy_back(const SLList& sll) {
		for (const T& item : sll) {
			push_back(item);
		}
	}
	//Problem #3: Adding 2 HUGE numbers
	void add_num(const SLList& sll) {
		int carry = 0;
		Node* curr = head;
		for (auto n : sll) {
			if (curr) {
				int sum = curr->value + n + carry;
				curr->value = sum % 10;
				carry = sum / 10;
				curr = curr->next;
			}
			else {
				int sum = n + carry;
				push_back(sum % 10);
				carry = sum / 10;
			}
		}
		if (carry > 0) push_back(carry);

	}
	//Problem #4: Remove all repeated
	void remove_repeated() {
		Node* prev = nullptr, * curr = head;
		bool is_last_deleted = false;
		T last_deleted = {};
		while (curr) {
			auto next = curr->next;
			if ((next && curr->value == next->value) || (is_last_deleted && curr->value == last_deleted)) {
				is_last_deleted = true;
				last_deleted = curr->value;

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
				is_last_deleted = false;
				prev = curr;
				curr = curr->next;
			}
		}
	}
	//Problem #5: Reverse Chains
	void reverse_chain(int k) {
		if (k > size() || k < 2) return;
		Node* prev = nullptr, * curr = head;
		while (curr) {
			for (int i = 1; i < k; i++) {
				if (!curr->next) break;
				if (prev) {
					add_next(prev, remove_next(curr));
				}
				else {
					push_front(remove_next(curr));
				}
			}
			prev = curr;
			curr = curr->next;
		}
	}
#pragma endregion

private:
	Node* head{};
	Node* tail{};
	size_t length{ 0 };
#pragma region Helpers
	Node* node_at(size_t i) const {
		if (i < 0 || i >= length) throw std::exception("out of range");
		Node* element = head;
		while (i != 0 && element != nullptr) {
			element = element->next;
			--i;
		}
		return element;
	}
	
	Node* node_pre(Node* node) {
		if (!node) return tail;
		if (node == head) return nullptr;
		Node* pre = head;
		while (pre && pre->next != node) { pre = pre->next; }
		return pre;
	}
	
	T remove_next(Node* node) {
		if (!node || node == tail) throw std::exception("INVALID OPERATION");
		Node* target = node->next;
		if (target == tail) return pop_back();
		node->next = target->next;
		T result = target->value;
		delete target;
		--length;
		return result;
	}
	
	Node* add_next(Node* node, const T& value) {
		if (!node) throw std::exception("Invalid Operation");
		if (node == tail) {
			push_back(value);
			return tail;
		}

		Node* newNode = new Node{ value, node->next };
		node->next = newNode;
		++length;
		return newNode;
	}
#pragma endregion
};

template <typename T> class SLList<T>::Node {
public:
	Node(const T& value = {}, Node* next = {})
		: value{ value }, next{ next }{};
private:
	T value{};
	Node* next{};
	friend class SLList<T>;
};

template <typename T> class SLList<T>::Iterator {
public:
	Iterator(Node* node) : node{ node } {};
	T& operator*() { return node->value; }
	Iterator& operator++() {
		if (node == nullptr) throw std::runtime_error("ACCESSING NULL NODE");
		node = node->next;
		return *this;
	}

	bool operator==(const Iterator& it) { return node == it.node; }
	bool operator!=(const Iterator& it) { return node != it.node; }
private:
	Node* node{};
	friend class SLList<T>;
};
