/**
* Mahmoud Elnagar
*/
#include <iostream>
#include <map>
using namespace std;

#pragma region MCS: Problem Set 1 Solutions
// Problem #1: Is increasing array?
bool is_increasing(){
  int n; cin>>n;
  int msf = INT32_MIN;
  int number;

  for (int i = 0; i < n; ++i){
    cin >> number;
    if (number < msf) return false;
    msf = number;
  }

  return true;
}

// Problem #2: Replace MinMax
void replace_minmax(){
  int n; cin>>n;
  int arr[n];
  int max_val = INT32_MIN;
  int min_val = INT32_MAX;
  int number;

  for (int i = 0; i < n; ++i){
    cin >> number;
    max_val = max(max_val, number);
    min_val = min(min_val, number);
    arr[i] = number;
  }

  for(int i = 0; i < n; ++i){
    number = arr[i];
    if (number == max_val) cout<<min_val<<" ";
    else if (number == min_val) cout<<max_val<<" ";
    else cout<<number<<" ";
  }

  cout<<endl;
}

// Problem #3: Unique Numbers of ordered list
void print_unique(){
  int n; cin>>n;
  int prev = INT32_MIN;
  int number;

  for (int i = 0; i < n; ++i){
    cin >> number;
    if (number != prev) cout << number << " ";
    prev = number;
  }

  cout<<endl;
}

//Problem #4: Is Palindrome?
bool is_palindrome(){
  int n; cin>>n;
  int arr[n];

  for (int i = 0; i < n; ++i) cin >> arr[i];

  for (int i = 0; i < n/2; ++i){
    if (arr[i] != arr[n-1-i]) return false;
  }

  return true;
}

//Problem #5: Smallest pair
void smallest_pair(){
  int n; cin>>n;
  int arr[n];

  for(int i = 0; i < n; ++i) cin >> arr[i];

  int ssf = INT32_MAX;
  for(int i = 1; i < n; ++i){
    for(int j = i + 1; j < n; ++j){
      int eq = arr[i] + arr[j] + j - i;
      ssf = min(ssf, eq);
    }
  }

  cout<<ssf<<endl;
}

#pragma endregion

#pragma region MCS: Problem Set 2 Solutions
// Problem #1: Find the 3 minimum values
void min3vals(){
  int n; cin>>n;
  int minval[3] = {INT32_MAX, INT32_MAX, INT32_MAX};
  int number;

  for(int i = 0; i < n; ++i){
    cin>>number;
    if (number < minval[0]) swap(minval[0], number);
    if (number < minval[1]) swap(minval[1], number);
    if (number < minval[2]) swap(minval[2], number);
  }

  for(int i = 0; i < 3; ++i) cout<<minval[i]<<" ";
}

// Problem #2: Search for a number
void search_number(){
  int n; cin>>n;
  int arr[501];
  for(int i = 0; i < 501; ++i) arr[i] = -1;

  int number;
  for(int i = 0; i < n; ++i){
    cin>>number;
    arr[number] = i;
  }

  int q; cin>>q;
  for(int i = 0; i < q; ++i){
    cin>>number;
    cout<< arr[number]<< " ";
  }
}

// Problem #3: Find most frequent number
void frequent(){
  int n; cin>>n;
  int freq[771] = {};
  int number;

  for(int i = 0; i < n; ++i){
    cin>>number;
    ++freq[number+500];
  }

  int fi = 0;

  for(int i = 0; i < 771; ++i){
    if (freq[i] > fi) fi = i;
  }

  cout<< (fi - 500) <<" repeated "<< freq[fi] <<" times"<<endl;
}

// Problem #4: Digits frequency
void digit_freq(){
  int n; cin>>n;
  int freq[10] = {};
  int number;

  for(int i = 0; i < n; ++i){
    cin>>number;
    while (number > 9){
      ++freq[number%10];
      number = number/10;
    }
    ++freq[number];
  }

  for(int i = 0; i < 10; ++i) cout<<i<<" "<<freq[i]<<endl;
}

// Problem #5: Unique Numbers of unordered list
void unique_unordered(){
  int n; cin>>n;
  int arr[501] = {};
  int number;

  for(int i = 0; i < n; ++i){
    cin>>number;
    if (arr[number] == 0){
      arr[number] = 1;
      cout<<number<<" ";
    }
  }

  cout<<endl;
}

// Problem #6: Sorting numbers
void sort_short(){
  int n; cin>>n;
  int freq[501] = {};
  int number;

  for(int i = 0; i < n; ++i){
    cin>>number;
    ++freq[number];
  }

  for(int i = 0; i < 501; ++i){
    int f = freq[i];
    for(int p = 0; p < f; ++p) cout<< i << " ";
  }

  cout<<endl;
}

#pragma endregion

#pragma region MCS: Problem Set 3 Solutions
// Problem #1: Recamán's sequence
void recaman(){
  int n; cin>>n;
  int last_value = 0;
  map<int, bool> seq;
  seq[0] = true;

  for(int i = 1; i <= n; ++i){
    int val = last_value - (i - 1) - 1;
    if ((val < 0) || seq[val]){
      val = last_value + (i - 1) + 1;
    }
    seq[val] = true;
    last_value = val;
  }

  cout<<last_value<<endl;
}

// Problem #2: Fixed sliding window
void fixed_window(){
  int k; cin>>k;
  int n; cin>>n;
  int arr[n];

  for(int i = 0; i < n; ++i) cin>>arr[i];

  int sum = 0;
  int wi = 0;
  for(int i = 0; i < k; ++i) sum += arr[i];

  int msf = sum;
  int mwi = 0;

  for(int i = k; i < n; ++i){
    sum -= arr[wi++];
    sum += arr[i];
    if (sum > msf){
      msf = sum;
      mwi = wi;
    }
  }

  cout<<"["<<mwi<<","<<(mwi + k - 1)<<"] = "<< msf <<endl;
}

// Problem #3: Count increasing subarrays
void count_subarrays(){
  int n; cin>>n;
  int count = 0, length = 1;
  int number, prev;
  cin>>prev;
  for(int i = 1; i < n; ++i){
    cin>>number;
    if (number < prev){
      count += (length * (length + 1) / 2);
      length = 0;
    }
    ++length;
    prev = number;
  }
  count += (length * (length + 1) / 2);
  cout<<count<<endl;
}

// Problem #4: Josephus problem
void josephus(){
  int n; cin>>n;
  int k; cin>>k;
  bool killed[n] = {false};
  int idx = 0;

  for(int t = 0; t < n; ++t){
    int alive = n - t;
    int r = (k % alive)? (k % alive) : alive;
    
    int step = 0;
		int last_person = -1;

		while (step < r) {
			if (!killed[idx]) last_person = idx, step++;
			idx = (idx + 1) % n;
		}

		killed[last_person] = true;
		cout << last_person + 1 << " ";
  }

}

// Problem #5: longest subarray
void longest_subarray(){
  int n; cin>>n;
  int bin[n];
  for(int i = 0; i < n; ++i) cin>>bin[i];

  int balance, len, max_len = 0;

  for(int i = 0; i < n; ++i){
    len = 0;
    balance = 0;
    for(int j = i; j < n; ++j){
      balance += (bin[j]? 1 : -1);
      ++len;
      if (balance == 0) max_len = max(max_len, len);
    }
  }

  cout<<max_len<<endl;
}

#pragma endregion
