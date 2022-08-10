/**
* Mahmoud Elnagar - 10/08/2022
*/
#include<iostream>
using namespace std;

template<typename T>
T sum_array(T m[], unsigned i) {
	if (i == 0) return m[0];
	return m[i] + sum_array(m, i - 1);
}

template<unsigned N, typename T>
T sum_matrix(T m[N][N], unsigned r = N - 1) {
	if (r == 0) return sum_array(m[0], N - 1);
	return sum_array(m[r], N - 1) + sum_matrix<N>(m, r - 1);
}

template<unsigned N, typename T>
void print_matrix(T m[N][N]) {
	for (auto i = 0; i < N; ++i) {
		for (auto j = 0; j < N; ++j) {
			cout << m[i][j] << " ";
		}
		cout << endl;
	}
}

template<unsigned N, typename T>
void test(T m[N][N]) {
	cout << "Matrix" << endl;
	print_matrix<N>(m);
	cout << "Sum = " << sum_matrix<N>(m) << endl;
	cout << "==============================================\n";
}

int main() {
	int m1[1][1] = { {2} };
	int m2[2][2] = { {1,2},{3,4} };
	float m3[3][3] = { {1,2,3},{4,5,6},{7,8,9} };
	
	test<1>(m1);
	test<2>(m2);
	test<3>(m3);
}
