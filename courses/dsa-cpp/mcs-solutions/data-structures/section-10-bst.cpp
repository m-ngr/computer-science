/**
* Mahmoud Elnagar - 03/12/2022
*/
#include<iostream>
#include<vector>
#include<queue>
using namespace std;

class BinarySearchTree {
private:
	int data{ };
	BinarySearchTree* left{ };
	BinarySearchTree* right{ };

public:

#pragma region Starter Code
	BinarySearchTree(int data, BinarySearchTree* left = nullptr, BinarySearchTree* right = nullptr) :
		data(data), left(left), right(right) {
	}

	void get_inorder(vector<int>& values) {
		if (left)
			left->get_inorder(values);
		values.push_back(data);
		if (right)
			right->get_inorder(values);
	}

	void level_order_traversal2() {
		queue<BinarySearchTree*> nodes_queue;
		nodes_queue.push(this);

		int level = 0;
		while (!nodes_queue.empty()) {
			int sz = nodes_queue.size();

			cout << "Level " << level << ": ";
			while (sz--) {
				BinarySearchTree* cur = nodes_queue.front();
				nodes_queue.pop();

				cout << cur->data << " ";
				if (cur->left)
					nodes_queue.push(cur->left);
				if (cur->right)
					nodes_queue.push(cur->right);
			}
			level++;
			cout << "\n";
		}
	}

	void insert(int target) {
		if (target < data) {
			if (!left) {
				left = new BinarySearchTree(target);
			} else
				left->insert(target);
		} else if (target > data) {
			if (!right)
				right = new BinarySearchTree(target);
			else
				right->insert(target);
		} // else: exists already
	}

	bool is_leaf() const {
		return !(left || right);
	}

	void print_preorder() {
		cout << data << " ";
		if (left) left->print_preorder();
		if (right) right->print_preorder();
	}
#pragma endregion

 #pragma region MCS: Problem Set 1 Solutions
	//Problem #1: Iterative Search
	bool search_iterative(int target) {
		BinarySearchTree* node = this;
		while (node) {
			if (node->data == target) return true;
			else if (node->data > target) node = node->left;
			else node = node->right;
		}
		return false;
	}

	//Problem #2: Is BST?
	bool is_bst() {
		vector<int> inorder;
		get_inorder(inorder);
		for (int i = 1; i < inorder.size(); ++i) {
			if (inorder[i - 1] >= inorder[i]) return false;
		}
		return true;
	}
	bool is_bst2(int min = INT_MIN, int max = INT_MAX) {
		if (min >= data || data >= max) return false;
		if (left && !left->is_bst2(min, data)) return false;
		if (right && !right->is_bst2(data, max)) return false;
		return true;
	}

	//Problem #3: Array to Balanced BST
	static BinarySearchTree* build_balanced_bst_tree(vector<int>& values, int start = 0, int end = -1) {
		if (end == -1) end = values.size() - 1;
		if (start > end) return nullptr;
		int mid = (start + end) / 2;
		BinarySearchTree* leftTree = build_balanced_bst_tree(values, start, mid - 1);
		BinarySearchTree* rightTree = build_balanced_bst_tree(values, mid + 1, end);
		return new BinarySearchTree(values[mid], leftTree, rightTree);
	}

	//Problem #4: Kth smallest number 
	int kth_smallest(int& k) {
		int result = -1234;
		if (left) result = left->kth_smallest(k);
		if (k == 0) return result;
		if (k == 1) return data;
		--k;
		if (right) result = right->kth_smallest(k);
		return result;
	}

	//Problem #5: Lowest Common Ancestor (LCA)
	int lca(int x, int y) {
		if (data > x && data > y) return left ? left->lca(x, y) : -1;
		if (data < x && data < y) return right ? right->lca(x, y) : -1;
		return data;
	}
#pragma endregion

#pragma region MCS: Problem Set 2 Solutions
	//Problem #1: Parent Link [Implemented]
	//Problem #2: Queries of ancestors
	deque<int> successors(deque<int>& q) {
		if (q.empty()) return {};
		deque<int> res;
		if (left && data > q.front()) res = left->successors(q);
		if (data > q.front()) {
			q.pop_front();
			res.push_back(data);
		}
		if (right && data < q.front()) {
			for (auto& item : right->successors(q)) {
				res.push_back(item);
			}
		}
		return res;
	}
	//Problem #3: Is degenerate tree
	bool is_degenerate(vector<int>& preorder) {
		if (preorder.size() < 3) return true;
		int mn = INT_MIN, mx = INT_MAX;
		for (int i = 1; i < preorder.size(); ++i) {
			int v = preorder[i - 1];
			int nv = preorder[i];
			if (v > mx || v < mn) return false;
			if (nv < v) mx = v; else mn = v;
		}
		return true;
	}
#pragma endregion

#pragma region MCS: Problem Set 3 Solutions
	// general constructor for all the problems
	enum class PROBLEM {
		PREORDER_SLOW = 1, //Problem #1
		PREORDER_FAST = 3, //Problem #3
		LEVEL_ORDER = 4,   //Problem #4
	};

	BinarySearchTree(deque<int>& order, PROBLEM problem) {
		if (problem == PROBLEM::PREORDER_SLOW) {
			preorder_constructor_v1(order);
		}

		if (problem == PROBLEM::PREORDER_FAST) {
			preorder_constructor_v2(order);
		}

		if (problem == PROBLEM::LEVEL_ORDER) {
			level_order_constructor(order);
		}
	}

	//Problem #1: BST from preorder v1
	void preorder_constructor_v1(deque<int>& preorder) {
		data = preorder.front();
		preorder.pop_front();
		for (auto& item : preorder) insert(item);
	}

	//Problem #2: BST from preorder v2
	//	[No optimization for my method for v1.]

	//Problem #3: BST from preorder v3
	void preorder_constructor_v2(deque<int>& preorder) {
		auto tree = createBST(preorder);
		if (tree) {
			data = tree->data;
			left = tree->left;
			right = tree->right;
		}

	}
	BinarySearchTree* createBST(deque<int>& preorder, int mn = INT_MIN, int mx = INT_MAX) {
		if (preorder.empty()) return nullptr;
		int value = preorder.front();
		BinarySearchTree* tree = nullptr;
		if (mn < value && value < mx) {
			tree = new BinarySearchTree(value);
			preorder.pop_front();
			tree->left = createBST(preorder, mn, value);
			tree->right = createBST(preorder, value, mx);
		}
		return tree;
	}

	//Problem #4: BST from level-order traversal
	struct RangedNode {
		BinarySearchTree* node{};
		int min{ INT_MIN };
		int max{ INT_MAX };
		RangedNode(BinarySearchTree* node, int min = INT_MIN, int max = INT_MAX)
			:node{ node }, min{ min }, max{ max }{};

		bool in_range(int value) const {
			return (min < value&& value < max);
		}

		bool left_range(int value) const {
			return (min < value&& value < node->data);
		}

		bool right_range(int value) const {
			return (node->data < value&& value < max);
		}
	};
	void level_order_constructor(deque<int>& level_order) {
		if (level_order.empty()) return;
		data = level_order.front();
		level_order.pop_front();

		queue<RangedNode> q;
		q.push({ this });

		while (!level_order.empty()) {
			auto entry = q.front(); q.pop();
			int value = level_order.front();

			if (entry.left_range(value)) {
				entry.node->left = new BinarySearchTree(value);
				q.push({ entry.node->left, entry.min, entry.node->data });
				level_order.pop_front();
				value = level_order.front(); // get value for a possible right child
			}

			if (entry.right_range(value)) {
				entry.node->right = new BinarySearchTree(value);
				q.push({ entry.node->right, entry.node->data, entry.max });
				level_order.pop_front();
			}
		}
	}

#pragma endregion

#pragma region MCS: Problem Set 4 Solutions
//Problem #1: Node Deletion using predecessor
	BinarySearchTree* max_node(BinarySearchTree* node) {
		auto curr = node;
		while (curr->right) curr = curr->right;
		return curr;
	}
	BinarySearchTree* delete_node_p(int target, BinarySearchTree* node) {
		if (!node) return nullptr;

		if (target < node->data) {
			node->left = delete_node_p(target, node->left);
		} else if (target > node->data) {
			node->right = delete_node_p(target, node->right);
		} else {
			if (node->is_leaf()) {
				delete node;
				return nullptr;
			} else if (!node->right) {
				auto temp = node->left;
				delete node;
				return temp;
			} else if (!node->left) {
				auto temp = node->right;
				delete node;
				return temp;
			} else {
				auto pred = max_node(node->left);
				node->data = pred->data;
				node->left = delete_node_p(node->data, node->left);
			}
		}
		return node;
	}
//Problem #2: Node Deletion without recursion
	void delete_node_p_it(int target, BinarySearchTree* node) {
		BinarySearchTree** node_ptr = nullptr;
		while (node) {
			if (target < node->data) {
				node_ptr = &(node->left);
				node = node->left;
			} else if (target > node->data) {
				node_ptr = &(node->right);
				node = node->right;
			} else {
				if (node->is_leaf()) {
					delete node;
					if (node_ptr) *node_ptr = nullptr;
					return;
				} else if (!node->right) {
					auto temp = node->left;
					delete node;
					if (node_ptr) *node_ptr = temp;
					return;
				} else if (!node->left) {
					auto temp = node->right;
					delete node;
					if (node_ptr) *node_ptr = temp;
					return;
				} else {
					auto pred = max_node(node->left);
					node->data = pred->data;
					delete_node_p_it(node->data, node->left);
				}
			}
		}
	}
//Problem #3: Rewriting Binary Search Tree [Implemented]
#pragma endregion

};


//Problem Set 2 >> Problem #1: Parent Link
class BinarySearchTree2 {
private:
	int data{ };
	BinarySearchTree2* left{ };
	BinarySearchTree2* right{ };
	BinarySearchTree2* parent{ };

	BinarySearchTree2* find(int target) {
		if (target == data)
			return this;

		if (target < data) {
			if (!left)
				return nullptr;
			return left->find(target);
		}

		if (!right)
			return nullptr;
		return right->find(target);
	}

public:

	BinarySearchTree2(int data, BinarySearchTree2* parent = nullptr) :
		data(data), parent(parent) {
	}

	void print_inorder() {
		if (left)
			left->print_inorder();
		cout << data << " ";
		if (right)
			right->print_inorder();
	}
	////////////////////////////////////////////

	void insert(int target) {
		if (target > data) {
			if (right) return right->insert(target);
			right = new BinarySearchTree2(target, this);
		} else {
			if (left) return left->insert(target);
			left = new BinarySearchTree2(target, this);
		}
	}

	int min_value() {
		BinarySearchTree2* node = this;
		while (node->left) node = node->left;
		return node->data;
	}

	pair<bool, int> successor(int target) {
		BinarySearchTree2* node = find(target);
		if (!node) return { false, target };
		if (node->right) return { true, node->right->min_value() };
		BinarySearchTree2* parent = node->parent;
		while (parent) {
			if (parent->left == node) return { true, parent->data };
			node = parent;
			parent = node->parent;
		}
		return { false, target };
	}
};
//Problem Set 4 >> Problem #3: Rewriting Binary Search Tree
class BinarySearchTree3 {
private:
	struct BinaryNode {
		int data{ };
		BinaryNode* left{ };
		BinaryNode* right{ };

		BinaryNode(int data) :
			data(data) {
		}
	};

	BinaryNode* root{ };

	///////////////////////////

	void print_inorder(BinaryNode* node) {
		if (!node) return;
		print_inorder(node->left);
		cout << node->data << " ";
		print_inorder(node->right);
	}

	BinaryNode* min_node(BinaryNode* tree) {
		BinaryNode* node = tree;
		while (node->left) node = node->left;
		return node;
	}

	void delete_value(int target, BinaryNode** tree_link) {
		if (!tree_link || !(*tree_link)) return;
		BinaryNode** link = tree_link;
		BinaryNode* node = *tree_link;

		while (node && node->data != target) {
			if (target < node->data) {
				link = &(node->left);
				node = node->left;
			} else if (target > node->data) {
				link = &(node->right);
				node = node->right;
			}
		}

		if (!node) return;

		if (!node->left && !node->right) {
			*link = nullptr;
			delete node;
		} else if (!node->right) {
			*link = node->left;
			delete node;
		} else if (!node->left) {
			*link = node->right;
			delete node;
		} else {
			BinaryNode* next = min_node(node->right);
			node->data = next->data;
			delete_value(next->data, &(node->right));
		}
	}

public:
	void insert_value(int target) {
		if (!root) {
			root = new BinaryNode(target);
			return;
		}

		BinaryNode* curr = root;

		while(curr){
			if (curr->data == target) return;
			if (target < curr->data) {
				if (curr->left) { curr = curr->left; continue; }
				curr->left = new BinaryNode(target);
			} else {
				if (curr->right) { curr = curr->right; continue; }
				curr->right = new BinaryNode(target);
			}
		}

	}

	void delete_value(int target) {
		delete_value(target, &root);
	}

	bool search(int target) {
		BinaryNode* node = root;
		while (node) {
			if (node->data == target) return true;
			if (target < node->data) node = node->left;
			if (target > node->data) node = node->right;
		}
		return false;
	}

	void print_inorder() {
		print_inorder(root);
	}

	void level_order_traversal() {
		queue<BinaryNode*> q;
		if(root) q.push(root);
		cout << "**********************\n";
		while (!q.empty()) {
			int size = q.size();
			while (size--) {
				BinaryNode* node = q.front(); q.pop();
				cout << node->data << " ";
				if (node->left) q.push(node->left);
				if (node->right) q.push(node->right);
			}
			cout << "\n";
		}
	}
};
