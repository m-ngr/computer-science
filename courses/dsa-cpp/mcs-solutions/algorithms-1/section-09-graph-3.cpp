/**
* Mahmoud Elnagar
*/
#include <iostream>
#include <vector>
#include <queue>
#include <math.h>
#include <unordered_set>
#include <set>
#include <algorithm>

using namespace std;

using GRAPH = vector<vector<int>>;

#pragma region MCS: Problem Set 1 Solutions
//Problem #1: Print Path
vector<int> parentsBFS(GRAPH& graph){
  vector<int> parents(graph.size(), -1);
  queue<int> q;
  q.push(0);
  parents[0] = 0;

  while(!q.empty()){
    auto node = q.front(); q.pop();

    for(auto& ni: graph[node]){
      if (parents[ni] != -1) continue;
      parents[ni] = node;
      q.push(ni);
    }
  }

  return parents;
}

void print_parents(const vector<int>& parents, int node){
  if (parents[node] == -1) return;
  if (node != 0) print_parents(parents, parents[node]);
  cout<< node <<" ";
}

void problem1() {
	int cases;
	cin >> cases;

	while (cases--) {
		int nodes, edges;
		cin >> nodes >> edges;

		GRAPH graph(nodes);

		for (int e = 0; e < edges; ++e) {
			int from, to;
			cin >> from >> to;
      graph[from].push_back(to);
		}
		int start = 0;
		vector<int> parents = parentsBFS(graph);

		for (int i = 0; i < nodes; ++i) {
			if(start == i)
				continue;

			cout << "Path from " << start << " to " << i << ": ";
			if(parents[i] == -1)
				cout<<"Not exist";
			else
				print_parents(parents, i);
			cout << "\n";
		}

		cout << "\n";
	}
}

//Problem #2: LeetCode 261 - Graph Valid Tree
bool validTree(int n, const vector<vector<int>> &edges){
  if (edges.size() != (n - 1)) return false;
  vector<bool> visited(n, false);
  int countNodes = 0;

  for(auto& edge : edges){
    if (!visited[edge[0]]) ++countNodes;
    if (!visited[edge[1]]) ++countNodes;

    visited[edge[0]] = true;
    visited[edge[1]] = true;
  }

  return (countNodes == n);
}

//Problem #3: LeetCode 1730 - Shortest Path to Get Food
int getFood(vector<vector<char>> &matrix){
  int di[4] = {0,0,1,-1};
  int dj[4] = {1,-1,0,0};

  const int ROWS = matrix.size(), COLS = matrix[0].size();
  queue<pair<int, int>> q;
  // push (i,j) of (*) into q
  for(auto i = 0; i < ROWS; ++i){
    bool found = false;
    for(auto j = 0; j < COLS; ++j){
      if (matrix[i][j] != '*') continue;
      q.push({i, j});
      found = true;
      break;
    }
    if (found) break;
  }

  for(int len = 0, size = 1; !q.empty(); ++len, size = q.size()){
    while(size--){
      int i = q.front().first, j = q.front().second;
      q.pop();

      for(int d = 0; d < 4; ++d){
        int ni = i + di[d];
        int nj = j + dj[d];
        if (ni < 0 || ni >= ROWS || nj < 0 || nj >= COLS) continue;
        if (matrix[ni][nj] == '#') return len + 1;
        if (matrix[ni][nj] != 'O') continue;
        matrix[ni][nj] = '*';
        q.push({ni, nj});
      }
    }
  }

  return -1;
}

#pragma endregion

#pragma region MCS: Problem Set 2 Solutions
//Problem #1: LeetCode 1306 - Jump Game III
bool canReach(vector<int>& arr, int start) {
  if (arr[start] == 0) return true;
  const int SIZE = arr.size();
  queue<int> q;
  q.push(start);
  arr[start] *= -1;

  while(!q.empty()){
    int i = q.front(); q.pop();
    int ni = i + arr[i];

    if (ni >= 0 && ni < SIZE && arr[ni] >= 0){
      if (arr[ni] == 0) return true;
      arr[ni] *= -1;
      q.push(ni);
    }

    ni = i - arr[i];

    if (ni >= 0 && ni < SIZE && arr[ni] >= 0){
      if (arr[ni] == 0) return true;
      arr[ni] *= -1;
      q.push(ni);
    }
  }

  return false;
}

bool canDFS(vector<int>& arr, vector<bool>& visited , int i){
  if (i < 0 || i > arr.size() || visited[i]) return false;
  if (arr[i] == 0) return true;
  visited[i] = true;
  return canDFS(arr, visited, i + arr[i]) || canDFS(arr, visited, i - arr[i]);
}

bool canReachDFS(vector<int>& arr, int start) {
  vector<bool> visited(arr.size(), false);
  return canDFS(arr, visited, start);
}

//Problem #2: LeetCode 2059 - Minimum Operations to Convert Number
bool processMinOps(queue<int>& q, vector<bool>& visited, int nx, int goal){
  if (nx == goal ) return true;
  if (0 <= nx && nx <= 1000 && !visited[nx]) {
    q.push(nx);
    visited[nx] = true;
  }
  return false;
}

int minimumOperations(vector<int>& nums, int start, int goal) {
  vector<bool> visited(1001, false);
  queue<int> q;
  q.push(start);
  visited[start] = true;

  for(int len = 0, size = 1; !q.empty(); ++len, size = q.size()){
    while(size--){
      int x = q.front(); q.pop();
      for(int i = 0; i < nums.size(); ++i){
        if (processMinOps(q, visited, x + nums[i], goal) ||
            processMinOps(q, visited, x - nums[i], goal) ||
            processMinOps(q, visited, x ^ nums[i], goal) ){
              return len + 1;
        }
      }
    }
  }

  return -1;
}

//Problem #3: LeetCode 752 - Open the Lock
// string-based solution
string moveWheel(string wheel, int slot, bool up){
  char digit = wheel[slot] + (up? 1 : -1);
  if (digit < '0') digit = '9';
  if (digit > '9') digit = '0';
  wheel[slot] = digit;
  return wheel;
}
int openLock(vector<string>& deadends, string target) {
  if (target == "0000") return 0;
  unordered_set<string> visited;
  for(auto& dstr: deadends) visited.insert(dstr);
  if (visited.count("0000")) return -1;

  queue<string> q;
  q.push("0000");
  visited.insert("0000");

  for(int move = 0, size = 1; !q.empty(); ++move, size = q.size()){
    while(size--){
      string key = q.front(); q.pop();
      // generate all 8 possible keys
      for(int s = 0; s < 4; ++s){
        string newKey1 = moveWheel(key, s, true);
        string newKey2 = moveWheel(key, s, false);

        if (newKey1 == target || newKey2 == target) return move + 1;

        if (!visited.count(newKey1)){
          q.push(newKey1);
          visited.insert(newKey1);
        }

        if (!visited.count(newKey2)){
          q.push(newKey2);
          visited.insert(newKey2);
        }
      }
    }
  }

  return -1;
}

// string-based solution (Modular version) 
pair<string, string> moveWheel(const string& wheel, int slot){
  char digit1 = wheel[slot] + 1;
  char digit2 = wheel[slot] - 1;
  if (digit1 > '9') digit1 = '0';
  if (digit2 < '0') digit2 = '9';
  auto keys = make_pair(wheel, wheel);
  keys.first[slot] = digit1;
  keys.second[slot] = digit2;
  return keys;
}
bool processKey(queue<string>& q, unordered_set<string>& visited, const string& key, const string& target){
  if (key == target) return true;
  if (!visited.count(key)){
    q.push(key);
    visited.insert(key);
  }
  return false;
}
int openLock_v2(vector<string>& deadends, string target) {
  if (target == "0000") return 0;
  unordered_set<string> visited;
  for(auto& dstr: deadends) visited.insert(dstr);
  if (visited.count("0000")) return -1;

  queue<string> q;
  q.push("0000");
  visited.insert("0000");
  
  for(int move = 0, size = 1; !q.empty(); ++move, size = q.size()){
    while(size--){
      string key = q.front(); q.pop();
      // generate all 8 possible keys
      for(int s = 0; s < 4; ++s){
        auto newKey = moveWheel(key, s);
        if (processKey(q, visited, newKey.first, target) ||
            processKey(q, visited, newKey.second, target)){
              return move + 1;
        }
      }
    }
  }

  return -1;
}

// int-based solution (much faster)
int moveWheel(int wheel, int slot, bool up){
  signed char key [4];
  for(int i = 0; i < 4; ++i){
    key[3 - i] = wheel % 10;
    wheel /= 10; 
  }

  key[slot] = (key[slot] + (up? 1 : -1) + 10) % 10;

  wheel = 0;
  for(int i = 0; i < 4; ++i){
    wheel *= 10;
    wheel += key[i];
  }

  return wheel;
}
int openLock_v3(vector<string>& deadends, string target) {
  if (target == "0000") return 0;
  vector<bool> visited(10000, false);
  for(auto& dstr: deadends) visited[stoi(dstr)] = true;
  if (visited[0]) return -1;
  int tar = stoi(target);
  queue<int> q;
  q.push(0);
  visited[0] = true;

  for(int move = 0, size = 1; !q.empty(); ++move, size = q.size()){
    while(size--){
      int key = q.front(); q.pop();
      // generate all 8 possible keys
      for(int s = 0; s < 4; ++s){
        int newKey1 = moveWheel(key, s, true);
        int newKey2 = moveWheel(key, s, false);

        if (newKey1 == tar || newKey2 == tar) return move + 1;

        if (!visited[newKey1]){
          q.push(newKey1);
          visited[newKey1] = true;
        }

        if (!visited[newKey2]){
          q.push(newKey2);
          visited[newKey2] = true;
        }
      }
    }
  }

  return -1;
}

#pragma endregion

#pragma region MCS: Problem Set 3 Solutions
//Problem #1: LeetCode 286 - Walls and Gates
void wallsAndGates(vector<vector<int>>& rooms){
  const int ROWS = rooms.size(), COLS = rooms[0].size();
  const int INF = INT32_MAX;

  int di[4] = {0,0,1,-1};
  int dj[4] = {1,-1,0,0};

  queue<pair<int, int>> q;
  // push all gates to q;
  for(int i = 0; i < ROWS; ++i){
    for(int j = 0; j < COLS; ++j){
      if (rooms[i][j] == 0) q.push({i, j});
    }
  }

  for(int move = 0, size = q.size(); !q.empty(); ++move, size = q.size()){
    while(size--){
      int i = q.front().first, j = q.front().second;
      q.pop();

      for(int d = 0; d < 4; ++d){
        int ni = i + di[d];
        int nj = j + dj[d];
        if(ni < 0 || ni >= ROWS || nj < 0 || nj >= COLS) continue;
        if (rooms[ni][nj] == INF){
          rooms[ni][nj] = move + 1;
          q.push({ni, nj});
        }
      }
    }
  }
}

//Problem #2: LeetCode 417 - Pacific Atlantic Water Flow
vector<vector<int>> pacificAtlantic(vector<vector<int>>& heights) {
  int di[4] = {1,-1,0,0};
  int dj[4] = {0,0,1,-1};
  const int ROWS = heights.size(), COLS = heights[0].size();

  vector<vector<int>> result;
  vector<vector<bool>> isPac(ROWS, vector<bool>(COLS, false));
  vector<vector<bool>> isATL(ROWS, vector<bool>(COLS, false));
  vector<vector<bool>> added(ROWS, vector<bool>(COLS, false));

  queue<pair<int, int>> q;

  for(int i = 0; i < ROWS; ++i){
    for(int j = 0; j < COLS; ++j){
      if (i == 0 || j == 0 || i == (ROWS -1) || j == (COLS-1)){
        q.push({i, j});
      }
      if (i == 0 || j == 0) isPac[i][j] = true;
      if (i == (ROWS-1) || j == (COLS-1)) isATL[i][j] = true;
    }
  }

  while(!q.empty()){
    int i = q.front().first, j = q.front().second;
    q.pop();
    if (added[i][j]) continue;
    if (isPac[i][j] && isATL[i][j]) {
      result.push_back({i, j});
      added[i][j] = true;
    }

    for(int d = 0; d < 4; ++d){
      int ni = i + di[d];
      int nj = j + dj[d];
      bool changed = false;
      if (ni < 0 || ni >= ROWS || nj < 0 || nj >= COLS) continue;

      if (heights[ni][nj] >= heights[i][j]){
        if (isPac[i][j] && !isPac[ni][nj]){
          isPac[ni][nj] = changed = true;
        }

        if (isATL[i][j] && !isATL[ni][nj]){
          isATL[ni][nj] = changed = true;
        }

        if (changed) q.push({ni, nj});
      }
    }
  }

  return result;
}

//Problem #3: LeetCode 1215 - Stepping Numbers

// brute-force approach
bool is_stepping(int num){
  if (num < 9) return true;
  int prev_dig = num % 10;
  num = num / 10;

  while(num){
    int cur_dig = num % 10;
    if (abs(cur_dig - prev_dig) != 1) return false;
    prev_dig = cur_dig;
    num = num / 10;
  }

  return true;
}
vector<int> countSteppingNumbers_bruteForce(int low, int high){
  vector<int> nums;
  for(int i = low; i <= high; ++i){
    if (is_stepping(i)) nums.push_back(i);
  }
  return nums;
}

// BFS approach
void processStep(queue<int>& q, vector<int>& res, int low, int high, int num){
  if (num < high) q.push(num);
  if (low <= num && num <= high) res.push_back(num);
}
vector<int> countSteppingNumbers(int low, int high){
  vector<int> res;
  for(int i = low; i <= min(9, high); ++i) res.push_back(i);

  queue<int> q;
  for(int d = 1; d <= 9; ++d) q.push(d);

  for(int size = q.size(); !q.empty(); size = q.size()){
    while(size--){
      int num = q.front(); q.pop();
      int last_digit = num % 10;
      num = num * 10 + last_digit;
      if (last_digit != 0) processStep(q, res, low, high, num - 1);
      if (last_digit != 9) processStep(q, res, low, high, num + 1);
    }
  }

  return res;
}

void testStepping(int low, int high){
  auto bf_ans = countSteppingNumbers_bruteForce(low, high);
  auto ans = countSteppingNumbers(low, high);

  if (bf_ans.size() != ans.size()) {
    cout<<"FAILED SIZE\n";
    return;
  }

  for(int i = 0; i < bf_ans.size(); ++i){
    if (ans[i] != bf_ans[i]){
      cout<<"FAILED VALUE\n";
      break;
    }
  }

  cout<<"SUCCESS\n";
}

#pragma endregion

#pragma region MCS: Problem Set 4 Solutions
//Problem #1: LeetCode 1129 - Shortest Path with Alternating Colors
struct NODE{
  int num;
  int color;

  NODE(int n, int color): num{n}, color{color}{};
};
vector<int> shortestAlternatingPaths(int n, vector<vector<int>>& redEdges, vector<vector<int>>& blueEdges) {
  const int RED = 1, BLUE = 0, NOCOLOR = -1;
  vector<vector<NODE>> graph(n);
  vector<bool> redVisited(n, false), blueVisited(n, false);
  vector<int> shortPath(n, -1);
  queue<NODE> q;
  // Build the graph
  for(auto& e: redEdges) graph[e[0]].push_back({e[1], RED});
  for(auto& e: blueEdges) graph[e[0]].push_back({e[1], BLUE});
  // push Node 0 to start BFS from
  q.push({0, NOCOLOR});
  shortPath[0] = 0;
  redVisited[0] = blueVisited[0] = true;
  // do BFS level by level
  for(int len = 1, size = q.size(); !q.empty(); ++len, size = q.size()){
    while(size--){
      auto node = q.front(); q.pop();

      for(auto& next: graph[node.num]){
        if (node.color == next.color) continue;
        if (next.color == RED && redVisited[next.num]) continue;
        if (next.color == BLUE && blueVisited[next.num]) continue;

        if (shortPath[next.num] == -1) shortPath[next.num] = len;
        q.push(next);

        if (next.color == RED){
          redVisited[next.num] = true;
        }else{
          blueVisited[next.num] = true;
        }
      }
    }
  }

  return shortPath;
}

//Problem #2: LeetCode 365 - Water and Jug Problem

//Define hash function for pair<int, int>
namespace std {
  template <> struct hash<std::pair<int, int>> {
    inline size_t operator()(const std::pair<int, int> &v) const {
      std::hash<int> int_hasher;
      return int_hasher(v.first) ^ int_hasher(v.second);
    }
  };
}

void addJugs(queue<pair<int, int>>& q, unordered_set<pair<int, int>>& visited, int j1, int j2){
  if (j1 > j2) swap(j1, j2);
  pair<int, int> jugs {j1, j2};
  if (visited.count(jugs)) return;
  q.push(jugs);
  visited.insert(jugs);
}

bool canMeasureWater(int jug1Capacity, int jug2Capacity, int targetCapacity) {
  if (jug1Capacity + jug2Capacity < targetCapacity) return false;
  if (jug1Capacity + jug2Capacity == targetCapacity) return true;
  queue<pair<int, int>> q;
  unordered_set<pair<int, int>> visited;
  q.push({0,0});
  visited.insert({0,0});

  while(!q.empty()){
    int j1 = q.front().first, j2 = q.front().second;
    q.pop();
    if ((j1 + j2) == targetCapacity) return true;
    // generate possible cases
    int m1 = min(j1, jug2Capacity - j2), m2 = min(j2, jug1Capacity - j1);
    
    addJugs(q, visited, jug1Capacity, j2); // fill j1
    addJugs(q, visited, j1, jug2Capacity); // fill j2
    addJugs(q, visited, 0, j2); // empty j1
    addJugs(q, visited, j1, 0); // empty j2
    addJugs(q, visited, j1 - m1, j2 + m1); // j1 to j2
    addJugs(q, visited, j1 + m2, j2 - m2); // j2 to j1
  }

  return false;
}

//Problem #3: LeetCode 773 - Sliding Puzzle
// board-based solution
int hashBoard(const vector<vector<int>>& board){
  const int ROWS = board.size(), COLS = board[0].size();
  int hash = 0;
  for(int i = 0; i < ROWS; ++i){
    for(int j = 0; j < COLS; ++j){
      hash = hash * 10 + board[i][j];
    }
  }
  return hash;
}

pair<int, int> setBoard(vector<vector<int>>& board, int hash){
  const int ROWS = board.size(), COLS = board[0].size();
  pair<int, int> zeroPos {0,0};

  for(int i = ROWS-1; i >= 0; --i){
    for(int j = COLS-1; j >= 0; --j){
      int digit = hash % 10;
      hash /=10;
      board[i][j] = digit;
      if (digit == 0) zeroPos = {i, j};
    }
  }

  return zeroPos;
}

int slidingPuzzle(vector<vector<int>>& board){
  const int target = 123450;
  int code = hashBoard(board);
  if (code == target) return 0;

  const int ROWS = board.size(), COLS = board[0].size();
  int di[4] = {0,0,1,-1};
  int dj[4] = {1,-1,0,0};

  unordered_set<int> visited;
  queue<int>q;
  q.push(code);
  visited.insert(code);

  for(int moves = 1, size = q.size(); !q.empty(); ++moves, size = q.size()){
    while(size--){
      code = q.front(); q.pop();
      auto pt = setBoard(board, code);

      for(int d = 0; d < 4; ++d){
        int ni = pt.first + di[d];
        int nj = pt.second + dj[d];
        if (ni < 0 || ni >= ROWS || nj < 0 || nj >= COLS) continue;

        swap(board[ni][nj], board[pt.first][pt.second]);
        int newCode = hashBoard(board);
        if (newCode == target) return moves;
        swap(board[ni][nj], board[pt.first][pt.second]);

        if (visited.count(newCode)) continue;
        q.push(newCode);
        visited.insert(newCode);
      }
    }
  }

  return -1;
}

// string-based solution
string boardString(const vector<vector<int>>& board){
  const int ROWS = board.size(), COLS = board[0].size();
  string code = "";
  for(int i = 0; i < ROWS; ++i){
    for(int j = 0; j < COLS; ++j){
      code += board[i][j] + '0';
    }
  }
  return code;
}

int slidingPuzzle_v2(vector<vector<int>>& board){
  const string target = "123450";
  string code = boardString(board);
  if (code == target) return 0;

  unordered_set<string> visited;
  queue<string>q;
  q.push(code);
  visited.insert(code);
  // 1-D based indexes
  vector<vector<int>> dz {{1,3}, {0,2,4}, {1,5}, {0,4}, {3,5,1}, {4,2}};

  for(int moves = 1, size = q.size(); !q.empty(); ++moves, size = q.size()){
    while(size--){
      code = q.front(); q.pop();
      int i = code.find('0');

      for(auto& ni: dz[i]){
        swap(code[i], code[ni]);
        if (code == target) return moves;
        if (visited.insert(code).second) q.push(code);
        swap(code[i], code[ni]);
      }
    }
  }

  return -1;
}

//Problem #4: LeetCode 1298 - Maximum Candies You Can Get from Boxes
int maxCandies(vector<int>& status, vector<int>& candies, vector<vector<int>>& keys, 
vector<vector<int>>& containedBoxes, vector<int>& initialBoxes) {
  int candy = 0;
  queue<int> openBoxes;
  vector<bool> ownedBoxes(status.size(), false);

  for(auto& box: initialBoxes){
    ownedBoxes[box] = true;
    if (status[box]) openBoxes.push(box);
  }

  while(!openBoxes.empty()){
    int i = openBoxes.front(); openBoxes.pop();
    candy += candies[i];
    candies[i] = 0;

    for(auto& key: keys[i]){
      if(status[key]) continue;
      status[key] = 1;
      if (ownedBoxes[key]) openBoxes.push(key);
    }

    for(auto& box: containedBoxes[i]){
      ownedBoxes[box] = true;
      if (status[box]) openBoxes.push(box);
    }
  }

  return candy;
}

#pragma endregion

#pragma region MCS: Problem Set 5 Solutions
//Problem #1: LeetCode 934 - Shortest Bridge
int di [4] = {0,0,1,-1};
int dj [4] = {1,-1,0,0};

void markDFS(vector<vector<int>>& grid, queue<pair<int, int>>& water , const int N, int i, int j){
  grid[i][j] = 2;

  for(int d = 0; d < 4; ++d){
    int ni = i + di[d];
    int nj = j + dj[d];
    if (ni < 0 || ni >= N || nj < 0 || nj >= N) continue;
    if (grid[ni][nj] == 1) markDFS(grid, water, N, ni, nj);
    if (grid[ni][nj] == 0) {
      water.push({ni, nj});
      grid[ni][nj] = 2;
    }
  }
}

queue<pair<int, int>> markIsland(vector<vector<int>>& grid){
  const int N = grid.size();
  queue<pair<int, int>> water;

  for(int i = 0; i < N; ++i){
    for(int j = 0; j < N; ++j){
      if (grid[i][j] == 0) continue;
      markDFS(grid, water, N, i, j);
      return water;
    }
  }

  return water;
}

int shortestBridge(vector<vector<int>>& grid) {
  const int N = grid.size();
  queue<pair<int, int>> q = markIsland(grid);

  for(int moves = 1, size = q.size(); !q.empty(); ++moves, size = q.size()){
    while(size--){
      int i = q.front().first, j = q.front().second;
      q.pop();

      for(int d = 0; d < 4; ++d){
        int ni = i + di[d];
        int nj = j + dj[d];
        if (ni < 0 || ni >= N || nj < 0 || nj >= N || grid[ni][nj] == 2) continue;
        if (grid[ni][nj] == 1) return moves;
        q.push({ni, nj});
        grid[ni][nj] = 2;
      }
    }
  }

  return N*N;
}

//Problem #2: SPOJ PT07Z - Longest path in a tree
pair<int, int> farBFS(GRAPH& graph, int start){
  vector<bool> visited(graph.size(), false);
  queue<int>q;
  q.push(start);
  visited[start] = true;

  int length = -1, node = start;
  for(int size = q.size(); !q.empty(); ++length, size = q.size()){
    while(size--){
      node = q.front(); q.pop();

      for(auto& nextNode: graph[node]){
        if (visited[nextNode]) continue;
        q.push(nextNode);
        visited[nextNode] = true;
      }
    }
  }

  return {node, length};
}

int treeDiam(GRAPH &graph) {
  auto farNode = farBFS(graph, 0); // Get farthest node from 0
  farNode = farBFS(graph, farNode.first); // Get farthest node from this farthest node
  return farNode.second;
}

#pragma endregion
