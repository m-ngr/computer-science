/**
* Mahmoud Elnagar - 06/01/2023
*/
#include <iostream>
#include <vector>
#include <algorithm>
#include<map>
using namespace std;

#pragma region MCS: Problem Set 1 Solutions
//Problem #1: Insertion Sort v2
void insertion_sort(vector<int>& v) {
	for (int i = 1; i < v.size(); ++i) {
		for (int j = i; (j > 0) && (v[j - 1] < v[j]); --j) {
			swap(v[j - 1], v[j]);
		}
	}
}
//Problem #2: Count Sort for Negative Values
//Problem #3: Count Sort for a range
void count_sort(vector<int>& v) {
	int min_val = v[0], max_val = v[0];
	for (int i = 1; i < v.size(); ++i) {
		if (min_val > v[i]) min_val = v[i];
		if (max_val < v[i]) max_val = v[i];
	}
	int len = max_val - min_val + 1;
	vector<int> freq(len);

	for (int& n : v) ++freq[n - min_val];

	int k = 0;
	for (int i = 0; i < freq.size(); ++i) {
		for (int c = 0; c < freq[i]; ++c) {
			v[k++] = i + min_val;
		}
	}
}
//Problem #4: Count Sort for strings v1
void count_sort(vector<string>& v) {
	vector<vector<string>> cstrs(26);
	
	for (auto& str : v) {
		cstrs[str[0] - 'a'].push_back(str);
	}

	int i = 0;
	for (auto& strs : cstrs) {
		for (auto& str : strs) {
			v[i++] = str;
		}
	}
}
//Problem #5: Count Sort for strings v2
void count_sort2(vector<string>& v) {
	vector<vector<string>> cstrs(26 * 26);

	for (auto& str : v) {
		int idx = (str[0] - 'a') * 26 + (str[1] - 'a');
		cstrs[idx].push_back(str);
	}

	int i = 0;
	for (auto& strs : cstrs) {
		for (auto& str : strs) {
			v[i++] = str;
		}
	}
}
//Problem #6: Count Sort Version 2
vector<int> countSort(const vector<int>& array) {
	/*
	* Definitions:
	*	- N = array.size();
	*	- M = mxVal;
	* Time Complexity: O(N+M)
	* Space Complexity: O(N+M) -> output:O(N) and count:O(M)
	* Properties:
	*	- Stable = true
	*	- Adaptive = false
	*	- online = false
	*	- in-place = false
	* Notes:
	*	- This version contains more memory referencing operations, which is slow.
	*	- This version has bad locality (not cache friendly), which is even more slow.
	*/

	// Find the largest element of the array
	int size = array.size();
	int mxVal = array[0];
	for (int i = 1; i < size; ++i) //O(N)
		if (array[i] > mxVal)
			mxVal = array[i];

	// Compute Frequency
	vector<int> count(mxVal + 1); // O(M)
	for (int i = 0; i < size; ++i) // O(N)
		count[array[i]] += 1;

	// Accumulate the counting
	for (int i = 1; i <= mxVal; ++i) // O(M)
		count[i] += count[i - 1];

	// Find the index and put the number
	vector<int> output(size); // O(N)
	for (int i = size - 1; i >= 0; --i) { // O(N)
		output[count[array[i]] - 1] = array[i]; // bad locality!
		count[array[i]] -= 1; // bad locality!
	}

	return output;
}
#pragma endregion

#pragma region MCS: Problem Set 2 Solutions
//Problem #1: LeetCode 1200 - Minimum Absolute Difference
vector<vector<int>> minimumAbsDifference(vector<int>& arr) {
	sort(arr.begin(), arr.end());

	int min_diff = arr[1] - arr[0];
	for (int i = 2; i < arr.size(); ++i) {
		int diff = arr[i] - arr[i - 1];
		if (min_diff > diff) min_diff = diff;
	}

	vector<vector<int>> pairs;
	for (int i = 1; i < arr.size(); ++i) {
		int a = arr[i - 1];
		int b = arr[i];
		if ((b - a) == min_diff) pairs.push_back({ a,b });
	}

	return pairs;
}
vector<vector<int>> minimumAbsDifference_opt(vector<int>& arr) {
	sort(arr.begin(), arr.end());

	int min_diff = arr[1] - arr[0];
	int pair_num = 1;
	for (int i = 2; i < arr.size(); ++i) {
		int diff = arr[i] - arr[i - 1];
		if (min_diff > diff) {
			min_diff = diff;
			pair_num = 1;
		} else if (min_diff == diff) {
			++pair_num;
		}
	}

	vector<vector<int>> pairs;
	pairs.reserve(pair_num);
	for (int i = 1; i < arr.size(); ++i) {
		int a = arr[i - 1];
		int b = arr[i];
		if ((b - a) == min_diff) pairs.push_back({a,b});
	}

	return pairs;
}
//Problem #2: LeetCode 976 - Largest Perimeter Triangle
int largestPerimeter(vector<int>& nums) {
	sort(nums.begin(), nums.end());

	for (int i = nums.size() - 1; i > 1; --i) {
		int a = nums[i];
		int b = nums[i - 1];
		int c = nums[i - 2];
		int diff = a - b;
		if (c > diff) return a + b + c;
	}

	return 0;
}
//Problem #3: LeetCode 561 - Array Partition
int arrayPairSum(vector<int>& nums) {
	sort(nums.begin(), nums.end());
	int sum = 0;
	for (int i = 0; i < nums.size(); i += 2) {
		sum += nums[i];
	}
	return sum;
}
//Problem #4: LeetCode 280 - Wiggle Sort
void wiggleSort(vector<int>& nums) { //O(NlogN)
	sort(nums.begin(), nums.end());
	for (int i = 2; i < nums.size(); i += 2) {
		swap(nums[i], nums[i - 1]);
	}
}
void wiggleSort2(vector<int>& nums) {//O(N)
	bool less = true;
	for (int i = 1; i < nums.size(); ++i) {
		int a = nums[i - 1];
		int b = nums[i];
		if (a > b) swap(a, b);
		// a <= b
		nums[i - 1] = less ? a :b;
		nums[i] = less ? b : a;

		less = !less;
	}
}
//Problem #5: LeetCode 1921 - Eliminate Maximum Number of Monsters
int eliminateMaximum(vector<int>& dist, vector<int>& speed) {
	int size = dist.size();
	vector<int> time(size);

	for (int i = 0; i < size; ++i) {
		int d = dist[i];
		int s = speed[i];
		int t = d / s;
		if ((t * s) < d) ++t;
		time[i] = t;
	}
	sort(time.begin(), time.end());
	for (int i = 1; i < size; ++i) {
		if (i >= time[i]) return i;
	}
	return size;
}
//Problem #6: LeetCode 1005 - Maximize Sum Of Array After K Negations
int largestSumAfterKNegations(vector<int>& nums, int k) {
	sort(nums.begin(), nums.end());
	int mn = INT_MAX, sum = 0;

	for (int i = 0; i < nums.size(); i++) {
		int v = nums[i];
		if (k && v < 0) {
			k -= 1, v *= -1;
		}
		sum += v;
		mn = min(mn, v);
	}

	if (k % 2) sum -= 2 * mn;
	return sum;
}
#pragma endregion

#pragma region MCS: Problem Set 3 Solutions
//Problem #1: LeetCode 581 - Shortest Unsorted Continuous Subarray
int findUnsortedSubarray(vector<int>& nums) {
	/*
	* time complexity : O(NlogN)
	* space complexity: O(N)
	*/
	vector<int> v(nums);
	sort(v.begin(), v.end());
	int s = -1, e = -1;
	for (int i = 0; i < v.size(); ++i) {
		if (v[i] != nums[i]) {
			if (s == -1) { s = i; e = i + 1; }
			e = i;
		}
	}
	if (s == -1) return 0;
	return e - s + 1;
}
int findUnsortedSubarray_opt(vector<int>& nums) {
	/*
	* time complexity : O(N)
	* space complexity: O(1)
	*/
	
	int size = nums.size();

	int end = -1, maxSoFar = nums[0];
	for (int i = 1; i < size; ++i) {
		int v = nums[i];
		if (v < maxSoFar) end = i; else maxSoFar = v;
	}

	int start = -1, minSoFar = nums.back();
	for (int i = size - 2; i >= 0; --i) {
		int v = nums[i];
		if (v > minSoFar) start = i; else minSoFar = v;
	}

	if (start == -1) return 0;
	return end - start + 1;
}
//Problem #2: LeetCode 826 - Most Profit Assigning Work
struct Job {
	int difficulty;
	int profit;
	Job(int d, int p) : difficulty{ d }, profit{ p }{};

	bool operator<(const Job& j) const {
		return difficulty < j.difficulty;
	}
};
int maxProfitAssignment(vector<int>& difficulty, vector<int>& profit, vector<int>& worker) {
	/*
	* N = jobs.size()
	* M = worker.size()
	* space-complexity: O(N)
	* time-complexity: O(NlogN + MlogM)
	*/
	vector<Job> jobs;
	jobs.reserve(profit.size());
	for (int i = 0; i < profit.size(); ++i) {
		jobs.push_back({ difficulty[i], profit[i] });
	}

	sort(jobs.begin(), jobs.end()); // O(NlogN)
	sort(worker.begin(), worker.end()); // O(MlogM)

	int k = 0;
	int msf = 0;
	int sum = 0;

	for (int i = 0; i < worker.size(); ++i) { // O(M+N)
		while (k < jobs.size() && worker[i] >= jobs[k].difficulty) {
			msf = max(msf, jobs[k].profit);
			++k;
		}
		sum += msf;
	}

	return sum;
}
//Problem #3: LeetCode 1887 - Reduction Operations to Make the Array Elements Equal
int reductionOperations(vector<int>& nums) {
	/*
	* time-complexity: O(NlogN)
	* space-complexity: O(1)
	*/
	sort(nums.begin(), nums.end());
	int ops = 0;
	int osf = 0;
	int msf = nums[0];
	
	for (int i = 1; i < nums.size(); ++i) {
		if (nums[i] > msf) {
			msf = nums[i];
			++osf;
		}
		ops += osf;
	}

	return ops;
}
int reductionOperations(vector<int>& nums) {
	/*
	* time-complexity: O(NlogN)
	* space-complexity: O(N)
	*/
	map<int, int> freq;

	for (int i = 0; i < nums.size(); ++i) {
		++freq[nums[i]];
	}

	int ops = 0;
	int i = 0;
	for (auto& e : freq) {
		ops += e.second * i;
		++i;
	}

	return ops;
}
#pragma endregion