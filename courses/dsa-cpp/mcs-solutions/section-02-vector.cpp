/**
* Mahmoud Elnagar - 20/09/2022
*/
#include<iostream>
#include<vector>
using namespace std;

#pragma region MCS: Problem Set 1 Solutions
// Problem #1: Right rotation
template<typename T> void right_rotate1(vector<T>& v) { //O(n)
	T value = v.back(); //O(1)
	v.pop_back(); //O(1)
	v.insert(v.begin(), value); //O(n): shifting
}

template<typename T> void right_rotate2(vector<T>& v) { //O(n)
	T value = v.back();
	for (int i = v.size() - 2; i >= 0; --i) {
		v[i+1] = v[i];
	}
	v[0] = value;
}
// Problem #2: Left rotation
template<typename T> void left_rotate1(vector<T>& v) { //O(n)
	T value = v.front(); //O(1)
	v.erase(v.begin()); //O(n): shifting
	v.push_back(value); //O(1)
}

template<typename T> void left_rotate2(vector<T>& v) { //O(n)
	T value = v.front();
	for (int i = 1; i < v.size(); ++i) {
		v[i - 1] = v[i];
	}
	v[v.size()-1] = value;
}
// Problem #3: Right rotation with steps
template<typename T> void right_rotate1(vector<T>& v, unsigned times) { //O(n)
	int k = times % v.size();
	vector<T> t(k);
	for (int i = 0; i < k; ++i) {
		t[i] = v.back();
		v.pop_back();
	}
	v.insert(v.begin(), t.rbegin(), t.rend()); //O(n): shifting
}

template<typename T> void right_rotate2(vector<T>& v, unsigned times) { //O(n)
	int k = times % v.size(); // k = [0 : n-1] 
	if (k == 0) return;
	// copy last k items: O(k)
	vector<T> t(k);
	for (int i = 0; i < k; ++i) {
		t[i] = v[v.size() - k + i];
	}
	// shift n-k items: O(n-k)
	for (int i = v.size() - 1 - k; i >= 0; --i) {
		v[i + k] = v[i];
	}
	// copy k items to front: O(k)
	for (int i = 0; i < k; ++i) {
		v[i] = t[i];
	}
}
// Problem #4: Deleting a position
template<typename T> T pop(vector<T>& v, unsigned idx) {
	if (idx < 0 || idx >= v.size()) throw exception("NO");
	T value = v[idx];
	for (auto i = idx + 1; i < v.size(); ++i) {
		v[i - 1] = v[i];
	}
	v.pop_back(); // --size;
	return value;
}
// Problem #5: Improved search
template<typename T> unsigned find_transposition(vector<T>& v, const T& value) {

	for (int i = 0; i < v.size(); ++i) {
		if (v[i] == value) {
			if (i != 0) { swap(v[i - 1], v[i]); --i; }
			return i;
		}
	}

	return -1; //NOT_FOUND
}
#pragma endregion