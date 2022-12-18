#include<vector>
/* AVL-based Dict */
template<typename K, typename V> class Dict {
public:
#pragma region Type Declarations
	class Iterator;
	class Entry;
	class Range;
	using key_type = K;
	using value_type = V;
	using size_type = unsigned;
#pragma endregion

#pragma region The Class Interface

	Dict() {};

	~Dict() { remove_tree(); }

	Dict(const std::initializer_list<Entry>& items) {
		insert(items);
	}

	Dict(const Dict& tree) { copy_tree(tree); }

	Dict(Dict&& tree) { move_tree(tree); }

	Dict& operator=(const Dict& tree) {
		remove_tree();
		copy_tree(tree);
		return *this;
	}

	Dict& operator=(Dict&& tree) {
		remove_tree();
		move_tree(tree);
		return *this;
	}

#pragma endregion

#pragma region The Standard Interface

	value_type& at(const key_type& key) throw (std::exception) {
		auto item = find(key);
		if (item == end()) throw std::exception("no such item");
		return item->value();
	}

	value_type& operator[](const key_type& key) {
		auto item = find(key);
		if (item == end()) item = insert(key, {});
		return item->value();
	}

	void insert(const std::initializer_list<Entry>& items) {
		for (auto& item : items) insert(item);
	}

	Iterator insert(const key_type& key, const value_type& value) {
		return insert({ key, value });
	};

	Iterator insert(const Entry& item) {
		if (!root()) {
			set_root(new BinaryNode(item));
			++_size;
			return Iterator(root());
		}

		return Iterator(inserter(item, root()));
	}

	Iterator find(const key_type& key) {
		BinaryNode* node = root();
		while (node) {
			if (node->entry.key() == key) {
				Iterator p = Iterator(node);
				while (p->key() == key) {
					if (p == begin()) return p;
					--p;
				}
				++p;
				return p;
			}
			node = (key < node->entry.key()) ? node->left : node->right;
		}
		return end();
	}

	Range find_all(const key_type& key) {
		Iterator first = find(key);
		if (first == end()) return Range(end(), end());
		Iterator last = first;
		while (last != end() && last->key() == key) ++last;
		return Range(first, last);
	}

	size_type count(const key_type& key) {
		size_type c = 0;
		for (auto& e : find_all(key)) ++c;
		return c;
	}

	bool has(const key_type& key) {
		if (find(key) == end()) return false;
		return true;
	}

	void erase(const key_type& key) throw (std::exception) {
		erase(find(key));
	}

	void erase(Iterator pointer) throw (std::exception) {
		if (pointer == end()) throw std::exception("no such item");
		eraser(pointer.node);
	}

	void erase(const Range& range) {
		auto it = range.begin();
		while (it != range.end()) erase(it++);
	};

	void erase_all(const key_type& key) { erase(find_all(key)); }

	void clear() { remove_tree(); }

	Iterator begin() {
		return Iterator(min_node(&super_root));
	}

	Iterator end() { return Iterator(&super_root); }

	bool empty() const { return size() == 0; }

	size_type size() const { return _size; }

	Iterator min() {
		if (!root()) return end();
		return Iterator(min_node(root()));
	}

	Iterator max() {
		if (!root()) return end();
		return Iterator(max_node(root()));
	}

	Iterator successor(Iterator pointer) throw (std::exception) {
		if (pointer == end()) throw std::exception("no such item");
		return Iterator(successor(pointer.node));
	}

	Iterator successor(const key_type& key) throw (std::exception) {
		return successor(find(key));
	}

	Iterator predecessor(Iterator pointer) throw (std::exception) {
		if (pointer == begin()) throw std::exception("no such item");
		return Iterator(predecessor(pointer.node));
	}

	Iterator predecessor(const key_type& key) throw (std::exception) {
		Iterator p = find(key);
		if (p == end()) throw std::exception("no such item");
		return predecessor(p);
	}

	Iterator lower_bound(const key_type& key) {
		BinaryNode* node = lower_bound(key, root());
		return (node ? Iterator(node) : end());
	}

	Iterator upper_bound(const key_type& key) {
		BinaryNode* node = upper_bound(key, root());
		return (node ? Iterator(node) : end());
	}

	template <typename C = std::vector<key_type>> C keys() const {
		C container;
		inorder_key(root(), container);
		return container;
	}

	template <typename C = std::vector<value_type>> C values() const {
		C container;
		inorder_value(root(), container);
		return container;
	}

	template <typename C = std::vector<Entry>> C entries() const {
		C container;
		inorder_entry(root(), container);
		return container;
	}

#pragma endregion

#pragma region The Functional Interface

	template<typename F>
	Dict filter(F& predicate) {
		Dict result;
		for (auto& entry : *this) {
			if (predicate(entry)) result.insert(entry);
		}
		return result;
	}

	template <typename K2, typename V2>
	Dict<K2, V2> map(typename Dict<K2, V2>::Entry(*transform) (const Entry&)) {
		Dict<K2, V2> result;
		for (auto& entry : *this) {
			result.insert(transform(entry));
		}
		return result;
	}

	template <typename R>
	R reduce(const R& init, R(*accumulate) (const R&, const Entry&)) {
		R result = init;
		for (auto& entry : *this) {
			result = accumulate(result, entry);
		}
		return result;
	}

	template <typename R>
	R reduce_values(const R& init, R(*accumulate) (const R&, const value_type&)) {
		R result = init;
		for (auto& entry : *this) {
			result = accumulate(result, entry.value());
		}
		return result;
	}

	template <typename F>
	bool exists(F& predicate) {
		for (auto& entry : *this) {
			if (predicate(entry)) return true;
		}
		return false;
	}

	template <typename F>
	Iterator match(F& predicate) {
		for (auto it = begin(); it != end(); ++it) {
			if (predicate(*it)) return it;
		}
		return end();
	}

	template<typename F>
	Dict& filter_self(F& predicate) {

		for (auto it = begin(); it != end();) {
			auto next = it; ++next;
			if (!predicate(*it)) erase(it);
			it = next;
		}

		return *this;
	}

	Dict& remove_all(const value_type& value) {
		auto pred = [=](auto e) {return e.value() != value; };
		return filter_self(pred);
	}

#pragma endregion

private:
	class BinaryNode;
	BinaryNode super_root{ {} };
	size_type _size{ 0 };

#pragma region Helper Functions

	BinaryNode* root() const { return super_root.left; }

	void set_root(BinaryNode* node) {
		super_root.set_left(node);
	};

	static BinaryNode* min_node(BinaryNode* node) {
		while (node->left) node = node->left;
		return node;
	}

	static BinaryNode* max_node(BinaryNode* node) {
		while (node->right) node = node->right;
		return node;
	}

	static BinaryNode* successor(BinaryNode* node) {
		if (node->right) return min_node(node->right);
		while (node->parent->right == node) node = node->parent;
		return node->parent;
	}

	static BinaryNode* predecessor(BinaryNode* node) {
		if (node->left) return max_node(node->left);
		while (node->parent->left == node) node = node->parent;
		return node->parent;
	}

	BinaryNode* inserter(const Entry& item, BinaryNode* node) {
		while (node) {
			if (item.key() < node->entry.key()) {
				if (!node->left) {
					node->left = new BinaryNode(item, node);
					++_size;
					check_balance(node);
					return node->left;
				}
				node = node->left;
			} else {
				if (!node->right) {
					node->right = new BinaryNode(item, node);
					++_size;
					check_balance(node);
					return node->right;
				}
				node = node->right;
			}
		}
		return node;
	}

	void eraser(BinaryNode* node) {
		BinaryNode* parent = node->parent;

		if (node->is_leaf()) {
			if (parent->left == node) {
				parent->set_left(nullptr);
			} else {
				parent->set_right(nullptr);
			}
			delete node;
		} else if (!node->right) {
			if (parent->left == node) {
				parent->set_left(node->left);
			} else {
				parent->set_right(node->left);
			}
			delete node;
		} else if (!node->left) {
			if (parent->left == node) {
				parent->set_left(node->right);
			} else {
				parent->set_right(node->right);
			}
			delete node;
		} else {
			BinaryNode* next = min_node(node->right);
			node->entry = next->entry;
			return eraser(next);
		}
		--_size;
		check_balance(parent);
	}

	void copy_tree(const Dict& tree) {
		set_root(copy_subtree(tree.root()));
		_size = tree._size;
	}

	BinaryNode* copy_subtree(BinaryNode* tree) {
		if (!tree) return nullptr;
		BinaryNode* node = new BinaryNode(tree->entry);
		node->set_left(copy_subtree(tree->left));
		node->set_right(copy_subtree(tree->right));
		return node;
	}

	void move_tree(Dict& tree) {
		set_root(tree.root());
		_size = tree._size;

		tree.set_root(nullptr);
		tree._size = 0;
	}

	void remove_tree() {
		remove_subtree(root());
		set_root(nullptr);
		_size = 0;
	}

	void remove_subtree(BinaryNode* tree) {
		if (!tree) return;
		remove_subtree(tree->left);
		remove_subtree(tree->right);
		delete tree;
	}

	template<typename C> void inorder_key(BinaryNode* node, C& container) const {
		if (!node) return;
		inorder_key(node->left, container);
		container.push_back(node->entry.key());
		inorder_key(node->right, container);
	}

	template<typename C> void inorder_value(BinaryNode* node, C& container) const {
		if (!node) return;
		inorder_value(node->left, container);
		container.push_back(node->entry.value());
		inorder_value(node->right, container);
	}

	template<typename C> void inorder_entry(BinaryNode* node, C& container) const {
		if (!node) return;
		inorder_entry(node->left, container);
		container.push_back(node->entry);
		inorder_entry(node->right, container);
	}

	void right_rotation(BinaryNode* tree) {
		BinaryNode* parent = tree->parent;
		bool is_left = (parent->left == tree);

		BinaryNode* l_tree = tree->left;
		BinaryNode* lr_tree = l_tree->right;

		tree->set_left(lr_tree);
		l_tree->set_right(tree);

		if (is_left) {
			parent->set_left(l_tree);
		} else {
			parent->set_right(l_tree);
		}
	}

	void left_rotation(BinaryNode* tree) {
		BinaryNode* parent = tree->parent;
		bool is_left = (parent->left == tree);

		BinaryNode* r_tree = tree->right;
		BinaryNode* rl_tree = r_tree->left;

		tree->set_right(rl_tree);
		r_tree->set_left(tree);

		if (is_left) {
			parent->set_left(r_tree);
		} else {
			parent->set_right(r_tree);
		}
	}

	void rebalance(BinaryNode* tree) {
		if (tree->balance_factor() <= -2) {
			if (tree->right->balance_factor() > 0) {
				right_rotation(tree->right);
			}
			left_rotation(tree);
		} else if (tree->balance_factor() >= 2) {
			if (tree->left->balance_factor() < 0) {
				left_rotation(tree->left);
			}
			right_rotation(tree);
		}
	}

	void check_balance(BinaryNode* node) {
		while (!node->is_root()) {
			node->update_height();
			rebalance(node);
			node = node->parent;
		}
	}

	BinaryNode* lower_bound(const key_type& key, BinaryNode* tree) {
		if (!tree) return nullptr;

		if (key <= tree->entry.key()) {
			auto left_result = lower_bound(key, tree->left);
			return (left_result) ? left_result : tree;
		} else {
			return lower_bound(key, tree->right);
		}
	}

	BinaryNode* upper_bound(const key_type& key, BinaryNode* tree) {
		if (!tree) return nullptr;

		if (key < tree->entry.key()) {
			auto left_result = upper_bound(key, tree->left);
			return (left_result) ? left_result : tree;
		} else {
			return upper_bound(key, tree->right);
		}
	}

#pragma endregion

};

template<typename K, typename V> class Dict<K, V>::BinaryNode {
public:
	Entry entry{};
	int height{ 0 };
	BinaryNode* parent{};
	BinaryNode* left{};
	BinaryNode* right{};

	BinaryNode(const Entry& entry, BinaryNode* parent = nullptr,
		BinaryNode* left = nullptr, BinaryNode* right = nullptr)
		: entry{ entry }, parent{ parent }, left{ left }, right{ right }{};

	bool is_leaf() const { return (!left) && (!right); }

	bool is_root() const { return !parent; }

	void set_left(BinaryNode* node) {
		left = node;
		if (node) node->parent = this;
		update_height();
	}

	void set_right(BinaryNode* node) {
		right = node;
		if (node) node->parent = this;
		update_height();
	}

	int balance_factor() const {
		int left_height = left ? left->height : -1;
		int right_height = right ? right->height : -1;
		return (left_height - right_height);
	}

	int update_height() {
		int left_height = left ? left->height : -1;
		int right_height = right ? right->height : -1;
		height = 1 + std::max(left_height, right_height);
		return height;
	}
};

template<typename K, typename V> class Dict<K, V>::Iterator {
public:
	Entry& operator*() const throw (std::exception) {
		if (node->is_root()) throw std::exception("can't dereference end node");
		return node->entry;
	};

	Entry* operator->() const throw (std::exception) {
		if (node->is_root()) throw std::exception("can't dereference end node");
		return &(node->entry);
	};

	bool operator==(const Iterator& pointer) const {
		return (pointer.node == node);
	};

	bool operator!=(const Iterator& pointer) const {
		return !(*this == pointer);
	};

	Iterator& operator++() {
		node = successor(node);
		return *this;
	};

	Iterator operator++(int) {
		Iterator curr = *this;
		++(*this);
		return curr;
	};

	Iterator& operator--() {
		node = predecessor(node);
		return *this;
	};

	Iterator operator--(int) {
		Iterator curr = *this;
		--(*this);
		return curr;
	};

private:
	BinaryNode* node{};
	friend class Dict;
	Iterator(BinaryNode* node) :node{ node } {};
};

template<typename K, typename V> class Dict<K, V>::Entry {
public:
	Entry(const key_type& key = {}, const value_type& value = {})
		:_key{ key }, _value{ value } {};

	const key_type& key() const { return _key; }
	const value_type& value() const { return _value; }
	value_type& value() { return _value; }
	void set_value(const value_type& value) { _value = value; }
private:
	key_type _key{};
	value_type _value{};
};

template<typename K, typename V> class Dict<K, V>::Range {
public:
	Range(const Iterator& begin, const Iterator& end) :_begin{ begin }, _end{ end } {};
	Iterator begin() const { return _begin; }
	Iterator end() const { return _end; }
private:
	Iterator _begin;
	Iterator _end;
};