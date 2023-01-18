/**
* Mahmoud Elnagar - 18/01/2023
*/
#include <iostream>
#include <vector>
#include <algorithm>
#include<map>
#include<cmath>
#include<iomanip>
using namespace std;

#pragma region MCS: Problem Set 1 Solutions
//Problem #1: LeetCode 34. Find First and Last Position of Element in Sorted Array
vector<int> searchRange_v1(vector<int>& nums, int target) {
	if (nums.empty()) return { -1,-1 };
	int start = 0, end = (int)nums.size() - 1;
	int s = -1, e = -1;

	while (start <= end) {
		int mid = start + (end - start) / 2;

		if (nums[mid] == target) {
			s = e = mid;
			break;
		} else if (nums[mid] < target) {
			start = mid + 1;
		} else {
			end = mid - 1;
		}
	}

	while ((s > 0) && (nums[s - 1] == target)) --s;
	while ((e < nums.size() - 1) && (nums[e + 1] == target)) ++e;

	return { s,e };
}

int first_occ(vector<int>& nums, int target, int start, int end) {
	int i = -1;

	while (start <= end) {
		int mid = start + (end - start) / 2;

		if (nums[mid] == target) {
			i = mid;
			end = mid - 1;
		} else if (nums[mid] < target) {
			start = mid + 1;
		} else {
			end = mid - 1;
		}
	}

	return i;
}

int last_occ(vector<int>& nums, int target, int start, int end) {
	int i = -1;

	while (start <= end) {
		int mid = start + (end - start) / 2;

		if (nums[mid] == target) {
			i = mid;
			start = mid + 1;
		} else if (nums[mid] < target) {
			start = mid + 1;
		} else {
			end = mid - 1;
		}
	}

	return i;
}

vector<int> searchRange_v2(vector<int>& nums, int target) {
	if (nums.empty()) return { -1,-1 };
	int end = (int)nums.size() - 1;
	int s = first_occ(nums, target, 0, end);
	if (s == -1) return { -1, -1 };
	int e = last_occ(nums, target, s, end);
	return { s,e };
}

vector<int> searchRange_v3(vector<int>& nums, int target) {
	auto res = equal_range(nums.begin(), nums.end(), target);
	if (res.first == nums.end()) return { -1, -1 };
	if (*res.first != target) return { -1, -1 };
	int s = res.first - nums.begin();
	int e = (res.second - nums.begin()) - 1;
	return { s,e };
}

//Problem #2: LeetCode 436 - Find Right Interval
vector<int> findRightInterval_v1(vector<vector<int>>& intervals) {
	for (int i = 0; i < intervals.size(); ++i) {
		intervals[i].push_back(i);
	}
	sort(intervals.begin(), intervals.end());
	vector<int> rights(intervals.size());
	vector<int> comp(1);

	for (auto it = intervals.begin(); it != intervals.end(); ++it) {
		int s = it->at(0);
		int e = it->at(1);
		int i = it->at(2);
		comp[0] = e;
		auto res = lower_bound(it, intervals.end(), comp);
		
		if (res == intervals.end()) {
			rights[i] = -1;
		} else {
			rights[i] = res->at(2);
		}

	}

	return rights;
}

vector<int> findRightInterval_v2(vector<vector<int>>& intervals) {
	map<int, int> ivals;
	for (int i = 0; i < intervals.size(); ++i) {
		ivals[intervals[i][0]] = i;
	}

	vector<int> result(intervals.size());

	for (int i = 0; i < intervals.size(); ++i) {
		auto it = ivals.lower_bound(intervals[i][1]);
		result[i] = (it == ivals.end()) ? -1 : it->second;
	}

	return result;
}

//Problem #3: LeetCode 611 - Valid Triangle Number
int triangleNumber_v1(vector<int>& nums) {
	sort(nums.begin(), nums.end());
	int count = 0;
	int size = nums.size();
	for (int a = 0; a < size - 2; ++a) {
		if (nums[a] == 0) continue;
		for (int b = a + 1; b < size - 1; ++b) {
			if (nums[b] == 0) continue;
			auto it = nums.begin() + b + 1;
			int c = lower_bound(it, nums.end(), nums[b] + nums[a]) - it;
			if (c > 0) count += c;
		}
	}
	return count;
}

int triangleNumber_v2(vector<int>& nums) {
	sort(nums.begin(), nums.end());
	int count = 0;
	int size = nums.size();
	for (int a = 0; a < size - 2; ++a) {
		if (nums[a] == 0) continue;
		auto itref = nums.begin() + a + 2;
		for (int b = a + 1; b < size - 1; ++b) {
			if (nums[b] == 0) continue;
			auto it = nums.begin() + b + 1;
			itref = lower_bound(itref, nums.end(), nums[b] + nums[a]);
			int c = itref - it;
			if (c > 0) count += c;
		}
	}
	return count;
}

int triangleNumber_v3(vector<int>& nums) {
	sort(nums.begin(), nums.end());
	int count = 0;
	int size = nums.size();
	for (int a = 0; a < size - 2; ++a) {
		if (nums[a] == 0) continue;
		int c = a + 2;
		for (int b = a + 1; b < size - 1; ++b) {
			if (nums[b] == 0) continue;
			while (c < size && nums[c] < nums[a] + nums[b]) ++c;
			count += c - b - 1;
		}
	}
	return count;
}

//Problem #4: LeetCode 441 - Arranging Coins
int arrangeCoins_v1(int n) {
	/*
	* N(r) = (r * (r + 1))/2 ; no.of coins required to fill r rows
	* let n = (r * (r + 1))/2 ; solve for (r), find floor(r);
	*	r^2 + r - 2n = 0
	* Solving the quadratic equation with:
	*	a = 1 , b = 1 , c = -2n
	* We get: r = floor((sqrt(1 + 8.0 * n) - 1) / 2);
	*/
	return floor((sqrt(1 + 8.0 * n) - 1) / 2);
}

int arrangeCoins_v2(int n) {
	/*
	* Given n coins, where n > 0
	* N(r) = (r * (r + 1))/2 ; no.of coins required to fill r rows
	* Search for (r) such that:
	*	- N(r) <= n
	*	- N(r) is maximized
	* The Search Space is r = [1, n]
	* Use Binary Search Technique
	*/
	long start = 1, end = n;
	long ans = 0;

	while (start <= end) {
		long r = start + (end - start) / 2;
		long nr = r * (r + 1) / 2;
		if (nr == n) {
			return r;
		} else if (nr < n) {
			start = r + 1;
			ans = r;
		} else {
			end = r - 1;
		}
	}

	return ans;
}
#pragma endregion

#pragma region MCS: Problem Set 2 Solutions
//Problem #1: LeetCode 1283 - Find the Smallest Divisor Given a Threshold
int sumNumsDiv(vector<int>& nums, double d) {
	int sum = 0;
	for (auto& n : nums) {
		sum += ceil(n / d);
	}
	return sum;
}

int smallestDivisor_v1(vector<int>& nums, int threshold) {
	/*
	* time complexity = O(Nlog(INT_MAX)) => O(32*N) => O(N)
	*/
	int start = 1, end = INT_MAX;
	int ans = 1;
	while (start <= end) {
		int d = start + (end - start) / 2;
		int sum = sumNumsDiv(nums, d);
		if (sum > threshold) {
			start = d + 1;
		} else {
			end = d - 1;
			ans = d;
		}
	}
	return ans;
}

int sumNumsDiv(vector<int>& nums, int d) {
	int sum = 0;
	for (auto& n : nums) {
		sum += (n + d - 1) / d; //ceil equation using ints
	}
	return sum;
}

int smallestDivisor_v2(vector<int>& nums, int threshold) {
	/*
	* time complexity = O(NlogM)
	* M : max number in nums
	*/
	int start = 1, end = *max_element(nums.begin(), nums.end());
	int ans = end;
	while (start <= end) {
		int d = start + (end - start) / 2;
		int sum = sumNumsDiv(nums, d);
		if (sum > threshold) {
			start = d + 1;
		} else {
			end = d - 1;
			ans = d;
		}
	}
	return ans;
}
//Problem #2: LeetCode 1482 - Minimum Number of Days to Make m Bouquets
int bouquets(vector<int>& bloomDay, int k, int d) {
	int m = 0;
	int n = bloomDay.size();
	int left = k;
	for (int i = 0; i < n; ++i) {
		if (bloomDay[i] > d) {
			left = k;
		} else {
			--left;
		}
		if (left == 0) {
			++m;
			left = k;
		}
	}
	return m;
}

int minDays_v1(vector<int>& bloomDay, int m, int k) {
	if ((bloomDay.size() / k) < m) return -1;
	int start = 1, end = *max_element(bloomDay.begin(), bloomDay.end());
	int ans = -1;

	while (start <= end) {
		int d = start + (end - start) / 2;
		if (bouquets(bloomDay, k, d) < m) {
			start = d + 1;
		} else {
			end = d - 1;
			ans = d;
		}
	}

	return ans;
}

bool isPossible(vector<int>& bloomDay,int m, int k, int d) {
	int left = k;

	for (int i = 0; i < bloomDay.size(); ++i) {
		if (bloomDay[i] > d) {
			left = k;
			continue;
		}

		--left;

		if (left == 0) {
			--m;
			if (m == 0) return true;
			left = k;
		}
	}

	return false;
}

int minDays_v2(vector<int>& bloomDay, int m, int k) {
	if (bloomDay.size() < (unsigned long) m * k) return -1;
	auto limits = minmax_element(bloomDay.begin(), bloomDay.end());
	int start = *limits.first, end = *limits.second;
	int ans = -1;

	while (start <= end) {
		int d = start + (end - start) / 2;
		if (isPossible(bloomDay, m, k, d)) {
			end = d - 1;
			ans = d;
		} else {
			start = d + 1;
		}
	}

	return ans;
}
//Problem #3: LeetCode 475 - Heaters
bool coverAll(vector<int>& houses, vector<int>& heaters, int r) { //O(NlogM)
	auto it = heaters.begin();
	for (auto& h : houses) {
		long maxDist = h + r;
		long minDist = h - r;
		if (minDist < 0) minDist = 0;
		it = lower_bound(it, heaters.end(), minDist); //O(logM)
		if (it == heaters.end()) return false;
		if (*it > maxDist) return false;
	}
	return true;
}

int findRadius_v1(vector<int>& houses, vector<int>& heaters) {
	sort(houses.begin(), houses.end()); // O(NlogN)
	sort(heaters.begin(), heaters.end()); //O(MlogM)
	int start = 0, end = max(heaters.back(), houses.back());
	int ans = end;
	//O(NlogMlogK)
	while (start <= end) {
		int r = start + (end - start) / 2;
		if (coverAll(houses, heaters, r)) {
			end = r - 1;
			ans = r;
		} else {
			start = r + 1;
		}
	}
	return ans;
}

int findRadius_v2(vector<int>& houses, vector<int>& heaters) {
	sort(heaters.begin(), heaters.end());
	int ans = -1;
	for (auto h : houses) {
		auto ith = lower_bound(heaters.begin(), heaters.end(), h);
		auto itl = ith;
		if (itl != heaters.begin()) --itl;
		if (ith == heaters.end()) --ith;
		int dh = abs(*ith - h);
		int dl = abs(h - *itl);
		ans = max(ans, min(dl, dh));
	}
	return ans;
}
#pragma endregion

#pragma region MCS: Problem Set 3 Solutions
//Problem #1: LeetCode 1060 - Missing Element in Sorted Array
int missingElement_v1(const vector<int>& nums, int k) {
	int i = 1;
	int number = nums[0];
	while (k && (i < nums.size())) {
		++number;
		if (number == nums[i]) {
			++i;
		} else {
			--k;
		}
	}
	return number + k;
}

int missing(const vector<int>& nums, int i) {
	return nums[i] - nums[0] - i;
}

int missingElement_v2(const vector<int>& nums, int k) {
	int start = 0, end = (int)nums.size() - 1;
	int idx = nums.size();
	while (start <= end) {
		int mid = start + (end - start) / 2;
		if (missing(nums, mid) < k) {
			start = mid + 1;
		} else {
			end = mid - 1;
			idx = mid;
		}
	}
	--idx;
	return nums[idx] + (k - missing(nums, idx));
}
//Problem #2: LeetCode 668. Kth Smallest Number in Multiplication Table
int numbersPre(int rows, int cols, int value) {
	int count = 0;
	for (int r = 1; r <= rows; ++r) {
		count += min(value / r, cols);
	}
	return count;
}

int findKthNumber(int m, int n, int k) {
	int start = 1, end = m * n;
	int ans = -1;
	while (start <= end) {
		int mid = start + (end - start) / 2;
		if (numbersPre(m, n, mid) < k) {
			start = mid + 1;
		} else {
			end = mid - 1;
			ans = mid;
		}
	}
	return ans;
}
#pragma endregion

#pragma region MCS: Problem Set 4 Solutions
//Problem #1: LeetCode 69 - Sqrt(x)
int mySqrt(int x) {
	double EPS = 1e-10;
	double start = 0.0, end = x;
	while (fabs(end - start) > EPS) {
		double m = start + (end - start) / 2.0;
		if ((m*m) < x) {
			start = m;
		} else {
			end = m;
		}
	}
	return end;
}
//Problem #2: Spoj PIE
bool allEat(vector<double>& pie_volume, int n, double v) {
	for (auto pie : pie_volume) {
		int pieces = pie / v;
		n -= pieces;
		if (n <= 0) return true;
	}
	return false;
}

double largest_piece(vector<double>& pie_volume, int n, double end) {
	double start = 0.0;

	for (int i = 0; i < 100; i++) {
		double v = start + (end - start) / 2.0;
		if (allEat(pie_volume, n, v)) {
			start = v;
		} else {
			end = v;
		}
	}
	return start;
}

void pie_driver() {
	int tests; cin >> tests;
	vector<double> v;
	for (int t = 0; t < tests; ++t) {
		int n, f;
		cin >> n >> f;
		v.clear();
		v.reserve(n);
		double maxPie = -1;
		int r;
		for (int i = 0; i < n; ++i) {
			cin >> r;
			double volume = 3.14159265358979323846 * r * r;
			maxPie = max(maxPie, volume);
			v.push_back(volume);
		}
		cout << fixed;
		cout << setprecision(4) << largest_piece(v, f + 1, maxPie) << endl;
	}
}
//Problem #3: Spoj GLASNICI [sourced]
bool possible(vector<double>& positions, double msg_dist, double can_move_dist) {
	double last_position = positions[0] + can_move_dist;
	for (int i = 1; i < (int)positions.size(); i++) {
		double last_possible_msg_position = last_position + msg_dist;

		if (positions[i] - can_move_dist > last_possible_msg_position)
			return false;
		last_position = min(last_possible_msg_position, positions[i] + can_move_dist);
	}
	return true;
}

double min_time(vector<double>& positions, double msg_dist) {
	double start = 0.0, end = 1e9;

	while (end - start > 1e-9) {
		double mid = start + (end - start) / 2.0;
		if (possible(positions, msg_dist, mid))
			end = mid;
		else
			start = mid;
	}
	return start;
}

int glasnici_driver() {
	int cases, n;
	double msg_dist;
	cin >> cases;

	while (cases--) {
		cin >> msg_dist >> n;
		vector<double> positions(n);

		for (int i = 0; i < n; i++)
			cin >> positions[i];

		double time = min_time(positions, msg_dist);

		cout << fixed;
		cout << setprecision(2) << time << "\n";
	}
	return 0;
}
#pragma endregion
