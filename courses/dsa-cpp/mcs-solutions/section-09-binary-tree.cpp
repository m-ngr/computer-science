/**
* Mahmoud Elnagar - 16/10/2022
*/
#include<iostream>
#include <cassert>
#include<vector>
#include<string>
#include<stack>
#include<list>
#include<queue>
#include<deque>
using namespace std;

template<typename T> class BinaryTree {
private:
	T data{ };
	BinaryTree* left{ };
	BinaryTree* right{ };

public:
	BinaryTree(const T& data) : data(data) {}

	void print_preorder() {
		cout << data << " ";
		if (left) left->print_preorder();
		if (right) right->print_preorder();
	}

	void print_inorder() {
		if (left) left->print_inorder();
		cout << data << " ";
		if (right) right->print_inorder();
	}

	void print_postorder() {
		if (left) left->print_postorder();
		if (right) right->print_postorder();
		cout << data << " ";
	}

	void print_breadth() {
		queue<BinaryTree*> q;
		q.push(this);
		while (!q.empty()) {
			BinaryTree* node = q.front(); q.pop();
			cout << node->data << " ";
			if (node->left) q.push(node->left);
			if (node->right) q.push(node->right);
		}
	}

	void print_levels() {
		queue<BinaryTree*> q;
		q.push(this);
		while (!q.empty()) {
			int lvlSize = q.size();
			while (lvlSize > 0) {
				BinaryTree* node = q.front(); q.pop();
				cout << node->data << " ";
				if (node->left) q.push(node->left);
				if (node->right) q.push(node->right);
				--lvlSize;
			}
			cout << "\n";
		}
	}

	void add(vector<T> values, vector<char> direction) {
		assert(values.size() == direction.size());
		BinaryTree* current = this;
		// iterate on the path, create all necessary nodes
		for (int i = 0; i < (int)values.size(); ++i) {
			if (direction[i] == 'L') {
				if (!current->left)
					current->left = new BinaryTree(values[i]);
				else
					assert(current->left->data == values[i]);
				current = current->left;
			}
			else {
				if (!current->right)
					current->right = new BinaryTree(values[i]);
				else
					assert(current->right->data == values[i]);
				current = current->right;
			}
		}
	}

#pragma region MCS: Problem Set 1 Solutions
	//Problem #1: Tree Max
	T tree_max() {
		T res = data;
		if (left) res = max(res, left->tree_max());
		if (right) res = max(res, right->tree_max());
		return res;
	}
	//Problem #2: Tree Height
	int tree_height() {
		if (!left && !right) return 0;
		int h = 0;
		if (left) h = left->tree_height();
		if (right) h = max(h, right->tree_height());
		return h + 1;
	}
	//Problem #3: Count all nodes
	int total_nodes() {
		int num = 1;
		if (left) num += left->total_nodes();
		if (right) num += right->total_nodes();
		return num;
	}
	//Problem #4: Count leaf nodes
	int leaf_nodes() {
		if (!left && !right) return 1;
		int num = 0;
		if (left) num += left->leaf_nodes();
		if (right) num += right->leaf_nodes();
		return num;
	}
	//Problem #5: Search the tree
	bool is_exist(const T& value) {
		if (data == value) return true;
		if (left && left->is_exist(value)) return true;
		if (right && right->is_exist(value)) return true;
		return false;
	}
	//Problem #6: Is Perfect Tree
	bool is_perfect() {
		if (!left && !right) return true;
		if (left && right && left->tree_height() == right->tree_height()) {
			return left->is_perfect() && right->is_perfect();
		}
		return false;
	}
	bool is_perfect_formula() {
		int h = tree_height();
		int l = leaf_nodes();
		return (l == pow(2, h));
	}
#pragma endregion

#pragma region MCS: Problem Set 2 Solutions
	//Problem #1: Binary Tree Destructor
	~BinaryTree() { clear(); }

	void clear() {
		if (left) delete left;
		if (right) delete right;
		left = right = nullptr;
	}
	
	//Problem #2: Inorder iterative
	void print_inorder_iterative() {
		stack<pair<BinaryTree*, bool>> s;
		s.push(make_pair(this, false));

		while (!s.empty()) {
			auto node = s.top().first;
			auto is_done = s.top().second;
			s.pop();

			if (is_done) {
				cout << node->data << " ";
			}else {
				if (node->right) s.push(make_pair(node->right, false));
				s.push(make_pair(node, true));
				if (node->left) s.push(make_pair(node->left, false));
			}
		}
	}
	//Problem #3: Tree boundary
	void traverse_left_boundry() {
		cout << data << " ";
		if (left) left->traverse_left_boundry();
		else if (right) right->traverse_left_boundry();
	}
	//Problem #4: Binary Tree Diameter
	int tree_diameter() {
		int diameter = 0;
		if (left) diameter += left->tree_height() + 1;
		if (right) diameter += right->tree_height() + 1;

		if (left) diameter = max(diameter, left->tree_diameter());
		if (right) diameter = max(diameter, right->tree_diameter());

		return diameter;
	}

	int tree_diameter_optimized() { return tree_DH().first; }

	pair<int, int> tree_DH() {
		if (!left && !right) return make_pair(0, 0);

		int d = 0;
		int h = 0;
		pair<int, int> leftDH = make_pair(0, 0);
		pair<int, int> rightDH = make_pair(0, 0);

		if (left) {
			leftDH = left->tree_DH();
			d += leftDH.second + 1;
		}

		if (right) {
			rightDH = right->tree_DH();
			d += leftDH.second + 1;
		}

		d = max(d, max(leftDH.first, rightDH.first));
		h = 1 + max(leftDH.second, rightDH.second);

		return make_pair(d, h);
	}
	//Problem #5: Expression Tree
	BinaryTree(string postfix) {
		stack<BinaryTree*> s;
		for (auto c : postfix) {
			auto node = new BinaryTree(c);
			if (isdigit(c) == false) {
				node->right = s.top(); s.pop();
				node->left = s.top(); s.pop();
			}
			s.push(node);
		}
		data = s.top()->data;
		left = s.top()->left;
		right = s.top()->right;
	}
	//Problem #6: Expression Tree Inorder
	void print_inorder_expression() {
		if (!left && !right) { cout << data; return; }

		if (!isdigit(left->data)) cout << "(";
		left->print_inorder_expression();
		if (!isdigit(left->data)) cout << ")";
		
		cout << data;

		if (!isdigit(right->data)) cout << "(";
		right->print_inorder_expression();
		if (!isdigit(right->data)) cout << ")";
	}
#pragma endregion

#pragma region MCS: Problem Set 3 Solutions
	//Problem #1: Recursive Level Order Traversal
	void level_order_traversal_recursive() {
		for (auto i = 0; i <= tree_height(); ++i) {
			print_level(i);
		}
	}

	void print_level(int n) {
		if (n == 0) { cout << data << " "; return; }
		if (left) left->print_level(n - 1);
		if (right) right->print_level(n - 1);
	}
	//Problem #2: Level Order Spiral
	void level_order_traversal_spiral() {
		deque<BinaryTree*> q;
		q.push_back(this);

		int level = 0;
		bool forward = true;

		while (!q.empty()) {
			cout << "Level " << level++ << ": ";
			int size = q.size();

			while (size--) {
				BinaryTree* node;

				if (forward) {
					node = q.front(); q.pop_front();
					if (node->left) q.push_back(node->left);
					if (node->right) q.push_back(node->right);
				} else {
					node = q.back(); q.pop_back();
					if (node->right) q.push_front(node->right);
					if (node->left) q.push_front(node->left);					
				}

				cout << node->data << " ";
			}

			cout << endl;
			forward = !forward;
		}

	}
	//Problem #3: Is complete Binary Tree
	bool is_complete() {
		queue<BinaryTree*> q;
		q.push(this);
		bool gap = false;

		while (!q.empty()) {
			int size = q.size();

			while (size--) {
				BinaryTree* node = q.front(); q.pop();
				if (gap && node->left) return false;
				if (node->left) q.push(node->left); else gap = true;
				if (gap && node->right) return false;
				if (node->right) q.push(node->right); else gap = true;
			}
		}

		return true;
	}
#pragma endregion

#pragma region MCS: Problem Set 4 Solutions
	//Problem #1: Tree from preorder & inorder
	BinaryTree(deque<T>& preorder, deque<T>& inorder) {
		data = preorder.front(); preorder.pop_front();
		if (preorder.size() == 0) return;

		deque<T> preorder_left, inorder_left;

		//split inorder (LvR) to L and R subtrees
		while (inorder.front() != data) {
			inorder_left.push_back(inorder.front());
			inorder.pop_front();
		}
		inorder.pop_front(); //remove v

		//split preorder (LR) to L and R subtrees
		for (int i = 0; i < inorder_left.size(); ++i) {
			preorder_left.push_back(preorder.front());
			preorder.pop_front();
		}

		// generate L and R subtrees
		if (inorder_left.size() > 0) {
			left = new BinaryTree(preorder_left, inorder_left);
		}
		if (inorder.size() > 0) {
			right = new BinaryTree(preorder, inorder);
		}
	}

	BinaryTree* generate_tree(deque<T>& preorder, deque<T>& inorder) {
		T value = preorder.front(); preorder.pop_front();
		BinaryTree* node = new BinaryTree(value);
		if (preorder.size() == 0) return node;

		deque<T> preorder_left, preorder_right, inorder_left, inorder_right;

		//split inorder (LvR) to L and R subtrees
		while (inorder.front() != value) {
			inorder_left.push_back(inorder.front());
			inorder.pop_front();
		}
		inorder.pop_front(); //remove v
		while (!inorder.empty()) {
			inorder_right.push_back(inorder.front());
			inorder.pop_front();
		}
		//split preorder (LR) to L and R subtrees
		for (int i = 0; i < inorder_right.size(); ++i) {
			preorder_right.push_front(preorder.back());
			preorder.pop_back();
		}
		while (!preorder.empty()) {
			preorder_left.push_back(preorder.front());
			preorder.pop_front();
		}
		// generate L and R subtrees
		if (inorder_left.size() > 0) {
			node->left = generate_tree(preorder_left, inorder_left);
		}
		if (inorder_right.size() > 0) {
			node->right = generate_tree(preorder_right, inorder_right);
		}

		return node;
	}

	//Problem #2: Generate a full binary tree
	BinaryTree(queue<pair<T, int>>& preorder) {
		data = preorder.front().first;
		bool is_leaf = preorder.front().second;
		preorder.pop();

		if (!is_leaf && preorder.size() > 0) {
			left = new BinaryTree(preorder);
			right = new BinaryTree(preorder);
		}
	}

#pragma endregion
	
#pragma region MCS: Problem Set 5 Solutions
	//Problem #1: Symmetric Tree
	bool is_symmetric2(){ return is_mirror(left, right); }
	bool is_symmetric() { 
		if (!left && !right) return true;
		if (left && right) return left->serialize("vlr") == right->serialize("vrl");
		return false;
	}

	bool is_mirror(BinaryTree* left_tree, BinaryTree* right_tree) {
		if (!left_tree && !right_tree) return true;
		if (!left_tree && right_tree) return false;
		if (left_tree && !right_tree) return false;

		if (left_tree->data != right_tree->data) return false;
		return is_mirror(left_tree->left, right_tree->right) && 
			is_mirror(left_tree->right, right_tree->left);
	}
	string serialize(const string& pattern = "vlr") {
		string output = "(", r_str = "()", l_str = "()";

		if (right) { r_str = right->serialize(pattern); }
		if (left) { l_str= left->serialize(pattern); }

		for (auto& p : pattern) {
			if (p == 'v' || p == 'V') output += to_string(data);
			else if (p == 'l' || p == 'L') output += l_str;
			else if (p == 'r' || p == 'R') output += r_str;
			else throw exception("unknown pattern");
		}

		output += ")";
		return output;
	}

	//Problem #2: Flip Equivalent Binary Trees
	bool is_flip_equiv(BinaryTree* other) {
		if (data != other->data) return false;

		if (left->is_flip_equiv(other->left) ||
			left->is_flip_equiv(other->right) == false) return false;

		if (right->is_flip_equiv(other->left) ||
			right->is_flip_equiv(other->right) == false) return false;

		return true;
	}
	//Problem #3: Print all duplicate subtrees
	void print_duplicate_subtrees() {
		vector<string> v;
		print_dups(v);
	}

	void print_dups(vector<string>& trees) {
		check_node(trees, left);
		check_node(trees, right);
		if (left) left->print_dups(trees);
		if (right) right->print_dups(trees);
	}

	void check_node(vector<string>& trees, BinaryTree* node) {
		if (node && node->tree_height() > 0) {
			string str = node->serialize();
			if (includes(trees, str)) {
				cout << str << endl;
			}
			else {
				trees.push_back(str);
			}
		}
	}

	bool includes(const vector<string>& v, string value) {
		for (auto& x : v) {
			if (x == value) return true;
		}
		return false;
	}
#pragma endregion

};