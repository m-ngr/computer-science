/**
* Mahmoud Elnagar - 18/12/2022
*/
#include <iostream>
#include <cassert>
#include <vector>
#include <queue>
#include <algorithm>
using namespace std;

class AVLTree {
#pragma region Starter Code
private:
	struct BinaryNode {
		int data{ };
		int height{ };
		BinaryNode* left{ };
		BinaryNode* right{ };

		BinaryNode(int data) :
			data(data) {
		}

		int ch_height(BinaryNode* node) {	// child height
			if (!node)
				return -1;			// -1 for null
			return node->height;	// 0 for leaf
		}
		int update_height() {	// call in end of insert function
			return height = 1 + max(ch_height(left), ch_height(right));
		}
		int balance_factor() {
			return ch_height(left) - ch_height(right);
		}
	};

	BinaryNode* root{ };

	///////////////////////////
	bool search(int target, BinaryNode* node) {
		if (!node)
			return false;

		if (target == node->data)
			return true;

		if (target < node->data)
			return search(target, node->left);

		return search(target, node->right);
	}

	BinaryNode* right_rotation(BinaryNode* Q) {
		BinaryNode* P = Q->left;
		Q->left = P->right;
		P->right = Q;
		Q->update_height();
		P->update_height();
		return P;
	}

	BinaryNode* left_rotation(BinaryNode* P) {
		BinaryNode* Q = P->right;
		P->right = Q->left;
		Q->left = P;
		P->update_height();
		Q->update_height();
		return Q;
	}

	BinaryNode* balance(BinaryNode* node) {
		if (node->balance_factor() == 2) { 			// Left
			if (node->left->balance_factor() == -1)	// Left Right?
				node->left = left_rotation(node->left);	// To Left Left

			node = right_rotation(node);	// Balance Left Left
		} else if (node->balance_factor() == -2) {
			if (node->right->balance_factor() == 1)
				node->right = right_rotation(node->right);

			node = left_rotation(node);
		}
		return node;
	}

	BinaryNode* insert_node(int target, BinaryNode* node) {
		if (target < node->data) {
			if (!node->left)
				node->left = new BinaryNode(target);
			else
				// change left. update left as it might be balanced
				node->left = insert_node(target, node->left);
		} else if (target > node->data) {
			if (!node->right)
				node->right = new BinaryNode(target);
			else
				node->right = insert_node(target, node->right);
		}
		node->update_height();
		return balance(node);
	}

	BinaryNode* min_node(BinaryNode* cur) {
		while (cur && cur->left)
			cur = cur->left;
		return cur;
	}

	BinaryNode* delete_node(int target, BinaryNode* node) {
		if (!node)
			return nullptr;

		if (target < node->data)
			node->left = delete_node(target, node->left);
		else if (target > node->data)
			node->right = delete_node(target, node->right);
		else {
			BinaryNode* tmp = node;

			if (!node->left && !node->right)	// case 1: no child
				node = nullptr;
			else if (!node->right) 	// case 2: has left only
				node = node->left;		// connect with child
			else if (!node->left)	// case 2: has right only
				node = node->right;
			else {	// 2 children: Use successor
				BinaryNode* mn = min_node(node->right);
				node->data = mn->data;	// copy & go delete
				node->right = delete_node(node->data, node->right);
				tmp = nullptr;	// Don't delete me. Successor will be deleted
			}
			if (tmp)
				delete tmp;
		}
		if (node) {
			node->update_height();
			node = balance(node);
		}
		return node;
	}
public:
	void insert_value(int target) {
		if (!root)
			root = new BinaryNode(target);
		else
			root = insert_node(target, root);
	}

	void delete_value(int target) {
		if (root) {
			root = delete_node(target, root);
		}
	}
#pragma endregion

#pragma region MCS: Problem Set 1 Solutions
//Problem #1: Lower Bound
private:
	BinaryNode* lower_bound(int target, BinaryNode* node) {
		if (!node) return nullptr;

		if (target <= node->data) {
			auto left_result = lower_bound(target, node->left);
			return (left_result) ? left_result : node;
		} else {
			return lower_bound(target, node->right);
		}
	}
public:
	pair<bool, int> lower_bound(int target) {
		BinaryNode* node = lower_bound(target, root);
		if (!node) return { false, -1 };
		return { true, node->data };
	}
//Problem #2: Upper Bound
private:
	BinaryNode* upper_bound(int target, BinaryNode* node) {
		if (!node) return nullptr;

		if (target < node->data) {
			auto left_result = upper_bound(target, node->left);
			return (left_result) ? left_result : node;
		} else {
			return upper_bound(target, node->right);
		}
	}
public:
	pair<bool, int> upper_bound(int target) {
		BinaryNode* node = upper_bound(target, root);
		if (!node) return { false, -1 };
		return { true, node->data };
	}

//Problem #3: Count inversions
	int count_inversions(const vector<int>& v) {
		int sum = 0;
		for (auto& x : v) sum += insert_count(x);
		return sum;
	}

	int insert_count(int target) {
		int count = 0;
		if (!root) {
			root = new BinaryNode(target);
		} else {
			root = insert_node_count(target, root, count);
		}
		return count;
	}

	BinaryNode* insert_node_count(int target, BinaryNode* node, int& count) {
		if (target < node->data) {
			count += 1 + count_nodes(node->right);
			if (!node->left)
				node->left = new BinaryNode(target);
			else
				node->left = insert_node_count(target, node->left, count);
		} else if (target > node->data) {
			if (!node->right)
				node->right = new BinaryNode(target);
			else
				node->right = insert_node_count(target, node->right, count);
		}
		node->update_height();
		return balance(node);
	}

	int count_nodes(BinaryNode* node) {
		if (!node) return 0;
		return 1 + count_nodes(node->left) + count_nodes(node->right);
	}

//Problem #4: Priority Queue [Implemented]
//Problem #5: Min nodes from AVL height
	/*
	* N(h) = N(h-1) + N(h-2) + 1;	N(0) = 1 and N(1) = 2
	* N(h): the min number of nodes required to form an AVL tree with height of h
	*/
public:
	static int avl_nodes_rec(int height) {
		if (height == 0) return 1;
		if (height == 1) return 2;
		return avl_nodes_rec(height - 1) + avl_nodes_rec(height - 2) + 1;
	}
	static int avl_nodes_iter(int height) {
		if (height == 0) return 1;
		if (height == 1) return 2;
		int res = 2;
		int prev = 1;
		for (int i = 2; i <= height; ++i) {
			int temp = res;
			res = res + prev + 1;
			prev = temp;
		}
		return res;
	}
//Problem #6: AVL Dictionary [sourced]
//Problem #7: AVL using 1 class [sourced]
#pragma endregion
};

//Problem Set 1 >> Problem #4: Priority Queue
class PriorityQueue {
private:
	struct BinaryNode {
		int key{ };
		vector<int> values;

		int height{ };
		BinaryNode* left{ };
		BinaryNode* right{ };

		BinaryNode(int key, int value) :
			key(key) {
			values.push_back(value);
		}

		BinaryNode(int key, const vector<int>& values) :
			key(key), values{ values } {};

		int ch_height(BinaryNode* node) {	// child height
			if (!node)
				return -1;			// -1 for null
			return node->height;	// 0 for leaf
		}
		int update_height() {	// call in end of insert function
			return height = 1 + max(ch_height(left), ch_height(right));
		}
		int balance_factor() {
			return ch_height(left) - ch_height(right);
		}
	};

	BinaryNode* root{ };

	///////////////////////////
	bool search(int target, BinaryNode* node) {
		if (!node)
			return false;

		if (target == node->key)
			return true;

		if (target < node->key)
			return search(target, node->left);

		return search(target, node->right);
	}

	BinaryNode* right_rotation(BinaryNode* Q) {
		BinaryNode* P = Q->left;
		Q->left = P->right;
		P->right = Q;
		Q->update_height();
		P->update_height();
		return P;
	}

	BinaryNode* left_rotation(BinaryNode* P) {
		BinaryNode* Q = P->right;
		P->right = Q->left;
		Q->left = P;
		P->update_height();
		Q->update_height();
		return Q;
	}

	BinaryNode* balance(BinaryNode* node) {
		if (node->balance_factor() == 2) { 			// Left
			if (node->left->balance_factor() == -1)	// Left Right?
				node->left = left_rotation(node->left);	// To Left Left

			node = right_rotation(node);	// Balance Left Left
		} else if (node->balance_factor() == -2) {
			if (node->right->balance_factor() == 1)
				node->right = right_rotation(node->right);

			node = left_rotation(node);
		}
		return node;
	}

	BinaryNode* insert_node(int key, int value, BinaryNode* node) {
		if (key < node->key) {
			if (!node->left)
				node->left = new BinaryNode(key, value);
			else
				// change left. update left as it might be balanced
				node->left = insert_node(key, value, node->left);
		} else if (key > node->key) {
			if (!node->right)
				node->right = new BinaryNode(key, value);
			else
				node->right = insert_node(key, value, node->right);
		} else {
			node->values.push_back(value);
		}
		node->update_height();
		return balance(node);
	}

	BinaryNode* min_node(BinaryNode* cur) {
		while (cur && cur->left)
			cur = cur->left;
		return cur;
	}
	BinaryNode* max_node(BinaryNode* cur) {
		while (cur && cur->right)
			cur = cur->right;
		return cur;
	}
	
	BinaryNode* delete_node(int target, BinaryNode* node) {
		if (!node)
			return nullptr;

		if (target < node->key)
			node->left = delete_node(target, node->left);
		else if (target > node->key)
			node->right = delete_node(target, node->right);
		else {
			BinaryNode* tmp = node;

			if (!node->left && !node->right)	// case 1: no child
				node = nullptr;
			else if (!node->right) 	// case 2: has left only
				node = node->left;		// connect with child
			else if (!node->left)	// case 2: has right only
				node = node->right;
			else {	// 2 children: Use successor
				BinaryNode* mn = min_node(node->right);
				node->key = mn->key;	// copy & go delete
				node->values.swap(mn->values);	// copy & go delete
				node->right = delete_node(node->key, node->right);
				tmp = nullptr;	// Don't delete me. Successor will be deleted
			}
			if (tmp)
				delete tmp;
		}
		if (node) {
			node->update_height();
			node = balance(node);
		}
		return node;
	}
public:

	void enqueue(int task_id, int task_priority) {
		if (!root) root = new BinaryNode(task_priority, task_id);
		root = insert_node(task_priority, task_id, root);
	}

	int dequeue() {
		BinaryNode* max = max_node(root);
		int value = max->values.back();
		max->values.pop_back();
		if (max->values.empty()) {
			root = delete_node(max->key, root);
		}
		return value;
	}

	bool isempty() { return root == nullptr; }
};

//Problem Set 1 >> Problem #6: AVL Dictionary
class Dictionary {
private:
	struct BinaryNode {
		string data{ };
		bool is_full_word{};
		int height{ };
		BinaryNode* left{ };
		BinaryNode* right{ };

		BinaryNode(string data, bool is_full_word) :
			data(data), is_full_word(is_full_word) {
		}

		int ch_height(BinaryNode* node) {	// child height
			if (!node)
				return -1;			// -1 for null
			return node->height;	// 0 for leaf
		}
		int update_height() {	// call in end of insert function
			return height = 1 + max(ch_height(left), ch_height(right));
		}
		int balance_factor() {
			return ch_height(left) - ch_height(right);
		}
	};

	BinaryNode* root{ };

	///////////////////////////
	BinaryNode* right_rotation(BinaryNode* Q) {
		BinaryNode* P = Q->left;
		Q->left = P->right;
		P->right = Q;
		Q->update_height();
		P->update_height();
		return P;
	}

	BinaryNode* left_rotation(BinaryNode* P) {
		BinaryNode* Q = P->right;
		P->right = Q->left;
		Q->left = P;
		P->update_height();
		Q->update_height();
		return Q;
	}

	BinaryNode* balance(BinaryNode* node) {
		if (node->balance_factor() == 2) { 			// Left
			if (node->left->balance_factor() == -1)	// Left Right?
				node->left = left_rotation(node->left);	// To Left Left

			node = right_rotation(node);	// Balance Left Left
		} else if (node->balance_factor() == -2) {
			if (node->right->balance_factor() == 1)
				node->right = right_rotation(node->right);

			node = left_rotation(node);
		}
		return node;
	}

	// -1 (not exist), 0 exist as prefix, 1 exist as full word
	int search(string target, BinaryNode* node) {
		if (!node)
			return -1;

		if (target == node->data)
			return node->is_full_word;

		if (target < node->data)
			return search(target, node->left);

		return search(target, node->right);
	}

	BinaryNode* insert_node(string target, bool is_full_word, BinaryNode* node) {
		if (target < node->data) {
			if (!node->left)
				node->left = new BinaryNode(target, is_full_word);
			else	// change left. update left as it might be balanced
				node->left = insert_node(target, is_full_word, node->left);
		} else if (target > node->data) {
			if (!node->right)
				node->right = new BinaryNode(target, is_full_word);
			else
				node->right = insert_node(target, is_full_word, node->right);
		} else if (is_full_word)	// if existed before but this is full_word, mark it
			node->is_full_word = 1;
		node->update_height();
		return balance(node);
	}

	void insert_value(string target, int is_full_word) {
		if (!root)
			root = new BinaryNode(target, is_full_word);
		else
			root = insert_node(target, is_full_word, root);
	}

public:
	void insert_string(string target) {	// O(nlogn)
		// Idea is to insert all the possible prefixes and add to AVL
		// Mark which is prefix and which is full word
		if (target == "")
			return;

		string cur;
		for (int i = 0; i < (int)target.size(); ++i) {
			cur += target[i];
			// If it is last index, then this is the original full word
			insert_value(cur, i == (int)target.size() - 1);
		}
	}

	bool word_exist(string target) {
		return search(target, root) == 1;
	}

	bool prefix_exist(string target) {
		// if exists as full or prefix
		return search(target, root) != -1;
	}
};

//Problem Set 1 >> Problem #7: AVL using 1 class
class AVLTree1 {
private:
	int data{ };
	int height{ };
	AVLTree1* left{ };
	AVLTree1* right{ };

	int ch_height(AVLTree1* node) {	// child height
		if (!node)
			return -1;			// -1 for null
		return node->height;	// 0 for leaf
	}
	int update_height() {	// call in end of insert function
		return height = 1 + max(ch_height(left), ch_height(right));
	}
	int balance_factor() {
		return ch_height(left) - ch_height(right);
	}

	AVLTree1* right_rotation(AVLTree1* Q) {
		cout << "right_rotation " << Q->data << "\n";
		AVLTree1* P = Q->left;
		Q->left = P->right;
		P->right = Q;
		Q->update_height();
		P->update_height();
		return P;
	}

	AVLTree1* left_rotation(AVLTree1* P) {
		cout << "left_rotation " << P->data << "\n";
		AVLTree1* Q = P->right;
		P->right = Q->left;
		Q->left = P;
		P->update_height();
		Q->update_height();
		return Q;
	}

	AVLTree1* min_node() {
		AVLTree1* cur = this;
		while (cur && cur->left)
			cur = cur->left;
		return cur;
	}

	void special_delete(AVLTree1* child) {
		data = child->data;
		left = child->left;
		right = child->right;
		delete child;
	}

	AVLTree1* delete_node(int target, AVLTree1* node) {
		if (!node)
			return nullptr;

		if (target < node->data)
			node->left = delete_node(target, node->left);
		else if (target > node->data)
			node->right = delete_node(target, node->right);
		else {
			if (!node->left && !node->right) {
				delete node;
				node = nullptr;
			} else if (!node->right) 	// case 2: has left only
				node->special_delete(node->left);		// connect with child
			else if (!node->left)	// case 2: has right only
				node->special_delete(node->right);
			else {
				AVLTree1* mn = node->right->min_node();
				node->data = mn->data;
				node->right = delete_node(node->data, node->right);
			}
		}
		if (node) {
			node->update_height();
			node = balance(node);
		}
		return node;
	}

	AVLTree1* balance(AVLTree1* node) {
		if (node->balance_factor() == 2) { 			// Left
			if (node->left->balance_factor() == -1)	// Left Right?
				node->left = left_rotation(node->left);	// To Left Left

			node = right_rotation(node);	// Balance Left Left
		} else if (node->balance_factor() == -2) {
			if (node->right->balance_factor() == 1)
				node->right = right_rotation(node->right);

			node = left_rotation(node);
		}
		return node;
	}

	/*
	 * Very tricky: this root itself got changed to argument node structure
	 * "This" is now either left or right of this node!
	 * We need carefully swap "This" and "node" data
	 * to maintain same new structure.
	 * If not careful = this will point to itself (recursion)
	 *
	 * Tip: this limitation is due to our structure
	 * Recall we avoided having a separate node structure
	 * This is not AVL issue but appears with AVL due to changing myself
	 */
	void reroot(AVLTree1* node) {
		if (node == this)
			return;

		swap(this->data, node->data);
		swap(this->height, node->height);

		if (this == node->left) {
			// Give node my 2 children
			// I take his right child and
			// point to him as my left
			swap(node->right, this->right);
			node->left = this->left;
			this->left = node;
		} else if (this == node->right) {
			swap(node->left, this->left);
			node->right = this->right;
			this->right = node;
		}
	}

public:
	AVLTree1(int data) :
		data(data) {
	}

	void level_order_traversal() {
		queue<AVLTree1*> nodes_queue;
		nodes_queue.push(this);

		int level = 0;
		while (!nodes_queue.empty()) {
			int sz = nodes_queue.size();

			cout << "Level " << level << ": ";
			while (sz--) {
				AVLTree1* cur = nodes_queue.front();
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
		cout << "******************\n";
	}

	void insert(int target) {
		if (target < data) {
			if (!left)
				left = new AVLTree1(target);
			else
				left->insert(target);
		} else if (target > data) {
			if (!right)
				right = new AVLTree1(target);
			else
				right->insert(target);
		}
		update_height();
		// Can't. Do manual copy!
		//this = balance(this);
		reroot(balance(this));	// copy
	}

	void delete_value(int target) {
		if (target == data && !left && !right)
			return;	// can't remove root in this structure
		reroot(delete_node(target, this));
	}

	bool is_bst() {
		bool left_bst = !left || data > left->data && left->is_bst();

		if (!left_bst)
			return false;

		bool right_bst = !right || data < right->data&& right->is_bst();
		return right_bst;
	}

	void verify() {
		assert(abs(balance_factor()) <= 1);
		assert(is_bst());
	}
};
