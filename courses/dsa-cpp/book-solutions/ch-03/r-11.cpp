/**
* Mahmoud Elnagar - 11/08/2022
*/
#include<iostream>
using namespace std;

template<typename T>
T max(T* begin, T* end) {
	if (begin == end) throw std::out_of_range("No max in an empty array");
	T* next = begin; next++;
	T value1 = *begin;
	if (next == end) return value1;
	T value2 = max(next, end);
	return (value1 > value2) ? value1 : value2;
}

template<typename T>
void print_array(T* arr, unsigned n) {
	for (auto i = 0; i < n; ++i) {
		cout << arr[i] << " ";
	}
	cout << endl;
}

template<typename T>
void test(T* arr, unsigned n) {
	cout << "Array: ";
	print_array(arr, n);
	cout << "Max = " << max(arr, arr + n)<<endl;
	cout << "==============================================\n";
}

int main() {
	int a1[1] = { 1 };
	int a2[2] = { 1,3 };
	int a3[15] = { 1,33,5,4,6,9,8,7,5,52,12,5,65,22,25 };
	int a4[15] = { 100,33,5,4,6,9,8,7,5,52,12,5,65,22,25 };
	int a5[15] = { 10,33,5,4,6,9,8,7,5,52,12,5,65,22,250 };

	test(a1, 1);
	test(a2, 2);
	test(a3, 15);
	test(a4, 15);
	test(a5, 15);
}
