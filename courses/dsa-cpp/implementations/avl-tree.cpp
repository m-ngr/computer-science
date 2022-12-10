#include<vector>

template<typename T, typename C = std::vector<T>> class AVLTree {
public:
#pragma region Type Declarations
	using value_type = T;
	using container_type = C;
	using size_type = unsigned;
	class Iterator; //in-order iterator
#pragma endregion

#pragma region The Class Interface

	AVLTree() {};

	~AVLTree() { remove_tree(); }

	AVLTree(const std::initializer_list<value_type>& items) {
		insert(items);
	}

	AVLTree(const AVLTree& tree) {
		copy_tree(tree);
	}

	AVLTree(AVLTree&& tree) {
		move_tree(tree);
	}

	AVLTree& operator=(const AVLTree& tree) {
		remove_tree();
		copy_tree(tree);
		return *this;
	}
	
	AVLTree& operator=(AVLTree&& tree) {
		remove_tree();
		move_tree(tree);
		return *this;
	}

#pragma endregion

#pragma region The Standard Interface

	void insert(const std::initializer_list<value_type>& items) {
		for (auto& item : items) insert(item);
	}

	Iterator insert(const value_type& value) {
		if (!root()) {
			set_root(new BinaryNode(value));
			++_size;
			return Iterator(root());
		}

		return Iterator(inserter(value, root()));
	}

	Iterator find(const value_type& value) {
		BinaryNode* node = root();

		while (node) {
			if (node->value == value) return Iterator(node);

			node = (value < node->value) ? node->left : node->right;
		}

		return end();
	}

	bool has(const value_type& value) {
		if (find(value) == end()) return false;
		return true;
	}

	void erase(const value_type& value) throw (std::exception) {
		erase(find(value));
	}

	void erase(Iterator pointer) throw (std::exception) {
		if (pointer == end()) throw std::exception("no such item");
		eraser(pointer.node);
	}

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

	Iterator successor(Iterator pointer) {
		if (pointer == end()) throw std::exception("NOP");
		return Iterator(successor(pointer.node));
	}

	Iterator successor(const value_type& value) {
		return successor(find(value));
	}

	Iterator predecessor(Iterator pointer) {
		if (pointer == begin()) throw std::exception("NOP");
		return Iterator(predecessor(pointer.node));
	}

	Iterator predecessor(const value_type& value) {
		Iterator p = find(value);
		if (p == end()) throw std::exception("NOP");
		return predecessor(p);
	}

	container_type inorder() const {
		container_type container;
		inorder(root(), container);
		return container;
	}

	container_type postorder() const {
		container_type container;
		postorder(root(), container);
		return container;
	}

	container_type preorder() const {
		container_type container;
		preorder(root(), container);
		return container;
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

	BinaryNode* inserter(const value_type& value, BinaryNode* node) {
		while (node->value != value) {
			if (value < node->value) {
				if (!node->left) {
					node->left = new BinaryNode(value, node);
					++_size;
					check_balance(node);
					return node->left;
				}
				node = node->left;
			} else {
				if (!node->right) {
					node->right = new BinaryNode(value, node);
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
			node->value = next->value;
			return eraser(next);
		}
		--_size;
		check_balance(parent);
	}

	void copy_tree(const AVLTree& tree) {
		set_root(copy_subtree(tree.root()));
		_size = tree._size;
	}

	BinaryNode* copy_subtree(BinaryNode* tree) {
		if (!tree) return nullptr;
		BinaryNode* node = new BinaryNode(tree->value);
		node->set_left(copy_subtree(tree->left));
		node->set_right(copy_subtree(tree->right));
		return node;
	}

	void move_tree(AVLTree& tree) {
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

	void inorder(BinaryNode* node, container_type& container) const {
		if (!node) return;
		inorder(node->left, container);
		container.push_back(node->value);
		inorder(node->right, container);
	}

	void postorder(BinaryNode* node, container_type& container) const {
		if (!node) return;
		postorder(node->left, container);
		postorder(node->right, container);
		container.push_back(node->value);
	}

	void preorder(BinaryNode* node, container_type& container) const {
		if (!node) return;
		container.push_back(node->value);
		preorder(node->left, container);
		preorder(node->right, container);
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

#pragma endregion

};

template<typename T, typename C> class AVLTree<T,C>::BinaryNode {
public:
	value_type value;
	int height{ 0 };
	BinaryNode* parent{};
	BinaryNode* left{};
	BinaryNode* right{};
	
	BinaryNode(const value_type& value, BinaryNode* parent = nullptr,
		BinaryNode* left = nullptr, BinaryNode* right = nullptr)
		: value{ value }, parent{ parent }, left{ left }, right{ right }{};

	bool is_leaf() const { return (!left) && (!right); }

	bool is_root() const { return !parent; }

	void set_left(BinaryNode* node){
		left = node;
		if(node) node->parent = this;
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

template<typename T, typename C> class AVLTree<T,C>::Iterator {
public:
	const value_type& operator*() const { return node->value; }
	const value_type* operator->() const { return &(node->value); };

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

	Iterator& operator--(){
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
	friend class AVLTree;
	Iterator(BinaryNode* node) :node{ node } {};
};
