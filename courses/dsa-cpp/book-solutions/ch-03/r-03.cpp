/**
* Mahmoud Elnagar - 10/08/2022
*/
#include<iostream>
using namespace std;

template<unsigned N, typename T>
void transpose(T m[N][N]) {
	for (auto i = 0; i < N; ++i) {
		for (auto j = i+1; j < N; ++j) {
			swap(m[i][j],m[j][i]);
		}
	}
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
	transpose<N>(m);
	cout << "Transposed Matrix" << endl;
	print_matrix<N>(m);
	cout << "==============================================\n";
}

int main() {
	int m1[1][1] = { {0} };
	char m2[2][2] = { {'a','b'},{'c','d'} };
	float m3[3][3] = { {1,2,3},{4,5,6},{7,8,9} };
	
	test<1>(m1);
	test<2>(m2);
	test<3>(m3);
}
