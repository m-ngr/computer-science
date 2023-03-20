/**
* Mahmoud Elnagar
*/
#include <iostream>
using namespace std;

#pragma region MCS: Problem Set 1 Solutions
//Homework 1: Length of 3n+1
int length_3n_plus_1(int n) {
	if (n == 1) return 1;
	if (n % 2 == 0) return 1 + length_3n_plus_1(n / 2);
	return 1 + length_3n_plus_1(3 * n + 1);
}

//Homework 2: Power function
int my_pow(int value, int p = 2){
  if (p == 0) return 1;
  return value * my_pow(value, p - 1);
}

//Homework 3: Array maximum
int arr_max(int arr[], int len){
  if (len == 0) throw exception();
  if (len == 1) return arr[0];
  return max(arr[len - 1], arr_max(arr, len-1));
}

//Homework 4: Array sum
int sum(int arr[], int len){
  if (len <= 0) return 0;
  return arr[len - 1] + sum(arr, len - 1);
}

//Homework 5: Array average
double average(int arr[], int len){
  if (len == 0) return 0;
  return (average(arr, len-1) * (len-1) + arr[len-1]) / len;
}

//Homework 6: Array Increment
void array_increment(int arr[], int len){
  if (len == 0) return;
  arr[len-1] += len - 1;
  return array_increment(arr, len - 1);
}

//Homework 7: Array Accumulation
void accumulate_arr(int arr[], int len){
  if (len == 1) return;
  accumulate_arr(arr, len-1);
  arr[len - 1] += arr[len - 2];
}

//Homework 8: Left-Max
void left_max(int arr[], int len){
  if (len == 1) return;
  left_max(arr, len-1);
  arr[len-1] = max(arr[len-1], arr[len-2]);
}

#pragma endregion

#pragma region MCS: Problem Set 2 Solutions
//Homework 9: Right-Max
void right_max(int arr[], int len){
  if(len == 1) return;
  arr[len-2] = max(arr[len-1], arr[len-2]);
  right_max(arr, len-1);
}

//Homework 10: Suffix Sum
int suffix_sum(int arr[], int len, int n){
  if (len == 0 || n == 0) return 0;
  return arr[len-1] + suffix_sum(arr, len-1, n-1);
}

//Homework 11: Prefix Sum
int prefix_sum(int arr[], int n){
  if (n == 0) return 0;
  return *arr + prefix_sum(arr+1, n-1);
}

//Homework 12: Is Palindrome
bool is_palindrome(int arr[], int len){
  if (len <= 1) return true;
  if(*arr != arr[len-1]) return false;
  return is_palindrome(arr + 1, len - 2);
}

//Homework 13: Is prefix
bool is_prefix(string str, string prefix, int i = 0){
  if (i >= prefix.size()) return true;
  if (str[i] != prefix[i]) return false;
  return is_prefix(str, prefix, i + 1);
}

/* Homework 14: Trace:
  1- print number digit by digit in a reverse order
  2- print the given number digit by digit
*/

//Homework 15: Count primes
bool is_prime(int n, int d = 2){
  if (d == n) return true;
  if (n % d == 0) return false;
  return is_prime(n, d+1);
}

int count_primes(int start, int end){
  if (start > end) return 0;
  return is_prime(start) + count_primes(start + 1, end);
}

//Homework 16: Grid Sum
int di[] = { 1, 0, 1 };
int dj[] = { 0, 1, 1 };

int path_sum(int grid[100][100], int ROWS, int COLS, int row = 0, int col = 0) {
	int sum = grid[row][col];

	if (row == ROWS - 1 && col == COLS - 1)
		return sum;

	int max_idx = -1, max_value = 0;

	for (int d = 0; d < 3; ++d) {
		int new_row = row + di[d];
		int new_col = col + dj[d];

		if (new_row >= ROWS || new_col >= COLS)
			continue;

		if (max_value < grid[new_row][new_col])
			max_value = grid[new_row][new_col], max_idx = d;
	}

	int new_row = row + di[max_idx];
	int new_col = col + dj[max_idx];

	return sum + path_sum(grid, ROWS, COLS, new_row, new_col);
}

//Homework 17: Fibonacci
int fibonacci(int n){
  if (n <= 1) return 1;
  return fibonacci(n-1) + fibonacci(n-2);
}

#pragma endregion
