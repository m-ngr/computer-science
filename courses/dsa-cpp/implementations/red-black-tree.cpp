#include<vector>

template<typename T, typename C = std::vector<T>> class RedBlackTree {
public:
#pragma region Type Declarations
	using value_type = T;
	using container_type = C;
	using size_type = unsigned;
	class Iterator; //in-order iterator
#pragma endregion

#pragma region The Class Interface

	RedBlackTree() {};

	~RedBlackTree() { remove_tree(); }

	RedBlackTree(const std::initializer_list<value_type>& items) {
		insert(items);
	}

	RedBlackTree(const RedBlackTree& tree) {
		copy_tree(tree);
	}

	RedBlackTree(RedBlackTree&& tree) {
		move_tree(tree);
	}

	RedBlackTree& operator=(const RedBlackTree& tree) {
		remove_tree();
		copy_tree(tree);
		return *this;
	}
	
	RedBlackTree& operator=(RedBlackTree&& tree) {
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
		BinaryNode* new_node = inserter(value, root());
		resolve_double_red(new_node);
		return Iterator(new_node);
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
	static const bool RED = true;
	static const bool BLACK = false;
	class BinaryNode;
	BinaryNode super_root{ {} };
	size_type _size{ 0 };

#pragma region Helper Functions

	BinaryNode* root() const { return super_root.left; }

	void set_root(BinaryNode* node) {
		super_root.set_left(node);
		if (node) node->color = BLACK;
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
					return node->left;
				}
				node = node->left;
			} else {
				if (!node->right) {
					node->right = new BinaryNode(value, node);
					++_size;
					return node->right;
				}
				node = node->right;
			}
		}
		return node;
	}

	void eraser(BinaryNode* node) {
		if (node->left && node->right) {
			BinaryNode* next = min_node(node->right);
			node->value = next->value;
			return eraser(next);
		}

		bool red_node = is_red(node);
		BinaryNode* parent = node->parent;
		BinaryNode* child = (node->left) ? node->left : node->right;

		if (parent->left == node) {
			parent->set_left(child);
		} else {
			parent->set_right(child);
		}
		delete node;
		--_size;

		if (red_node || is_red(child) || parent->is_root()) {
			if (child) child->color = BLACK;
		} else {
			resolve_double_black(parent, child);
		}
	}

	void copy_tree(const RedBlackTree& tree) {
		set_root(copy_subtree(tree.root()));
		_size = tree._size;
	}

	BinaryNode* copy_subtree(BinaryNode* tree) {
		if (!tree) return nullptr;
		BinaryNode* node = new BinaryNode(tree->value);
		node->color = tree->color;
		node->set_left(copy_subtree(tree->left));
		node->set_right(copy_subtree(tree->right));
		return node;
	}

	void move_tree(RedBlackTree& tree) {
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

	BinaryNode* right_rotation(BinaryNode* tree) {
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

		return l_tree;
	}

	BinaryNode* left_rotation(BinaryNode* tree) {
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

		return r_tree;
	}

	BinaryNode* restructure(BinaryNode* node) {
		BinaryNode* p = node->parent;
		BinaryNode* g = p->parent;

		if (g->right == p) {
			if (p->left == node) right_rotation(p);
			return left_rotation(g);
		} else {
			if (p->right == node) left_rotation(p);
			return right_rotation(g);
		}
	}

	void resolve_double_red(BinaryNode* node) {
		BinaryNode* parent = node->parent;
		if (is_black(node) || is_black(parent)) return;
		BinaryNode* grand_parent = parent->parent;
		BinaryNode* uncle = sibling_node(parent); // nullable

		if (is_black(uncle)) {
			BinaryNode* subroot = restructure(node);
			subroot->color = BLACK;
			subroot->left->color = RED;
			subroot->right->color = RED;
		} else {
			parent->color = BLACK;
			uncle->color = BLACK;
			if (grand_parent == root()) return;
			grand_parent->color = RED;
			resolve_double_red(grand_parent);
		}
	}

	void resolve_double_black(BinaryNode* x, BinaryNode* r) {
		// r is nullable
		BinaryNode* y = (x->left == r) ? x->right : x->left; //nullable

		if (is_red(y)) {
			if (x->left == y) {
				right_rotation(x);
			} else {
				left_rotation(x);
			}
			y->color = BLACK;
			x->color = RED;
			return resolve_double_black(x, r);
		}

		bool has_red = y && (is_red(y->left) || is_red(y->right));

		if (has_red) {
			BinaryNode* z = is_red(y->left) ? y->left : y->right;
			bool xcolor = x->color;
			z = restructure(z);
			z->color = xcolor;
			z->left->color = BLACK;
			z->right->color = BLACK;
			if (r) r->color = BLACK;
		} else {
			if (r) r->color = BLACK;
			if (y) y->color = RED;
			if (is_black(x) && (x != root())) {
				resolve_double_black(x->parent, x);
			}
			x->color = BLACK;
		}
	}

	bool is_black(BinaryNode* node) {
		if (!node) return true;
		return (node->color == BLACK);
	}

	bool is_red(BinaryNode* node) { return !is_black(node); }

	BinaryNode* sibling_node(BinaryNode* node) {
		BinaryNode* parent = node->parent;
		return (parent->left == node) ? parent->right : parent->left;
	}

#pragma endregion

};

template<typename T, typename C> class RedBlackTree<T,C>::BinaryNode {
public:
	value_type value;
	bool color{ RED };
	BinaryNode* parent{};
	BinaryNode* left{};
	BinaryNode* right{};
	
	BinaryNode(const value_type& value, BinaryNode* parent = nullptr,
		BinaryNode* left = nullptr, BinaryNode* right = nullptr)
		: value{ value }, parent{ parent }, left{ left }, right{ right }{};

	bool is_leaf() const { return (!left) && (!right); }

	bool is_root() const { return !parent; }

	void set_left(BinaryNode* node) {
		left = node;
		if (node) node->parent = this;
	}

	void set_right(BinaryNode* node) {
		right = node;
		if (node) node->parent = this;
	}
};

template<typename T, typename C> class RedBlackTree<T,C>::Iterator {
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
	friend class RedBlackTree;
	Iterator(BinaryNode* node) :node{ node } {};
};
