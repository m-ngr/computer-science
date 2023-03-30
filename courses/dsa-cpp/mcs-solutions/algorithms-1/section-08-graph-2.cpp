/**
* Mahmoud Elnagar
*/
#include <iostream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <set>
#include <queue>
#include <algorithm>
using namespace std;

#pragma region MCS: Problem Set 1 Solutions
//Problem #1: LeetCode 582 - Kill Process
typedef unordered_map<int, vector<int>> ProcessGraph;

void killDFS(ProcessGraph& graph, int kill, vector<int>& killed){ //O(d) <= O(N)
  killed.push_back(kill);
  for(auto& pid: graph[kill]) killDFS(graph, pid, killed);
}

vector<int> killProcess(vector<int> &pid, vector<int> &ppid, int kill){ //O(N)
  ProcessGraph graph;
  vector<int> killed;
  for(int i = 0; i < ppid.size(); ++i){
    if (ppid[i]) graph[ppid[i]].push_back(pid[i]);
  }
  killDFS(graph, kill, killed);
  return killed;
}

//Problem #2: LeetCode 690 - Employee Importance
// Definition for Employee.
class Employee {
public:
  int id;
  int importance;
  vector<int> subordinates;
};

class Solution {
public:
  int getImportance(vector<Employee*> employees, int id) { // O(N)
    Graph graph;
    for(auto emp: employees) graph[emp->id] = emp;
    return sumDFS(graph, id);
  }
private:
  typedef unordered_map<int, Employee*> Graph;

  int sumDFS(Graph& graph, int id){ //O(d) <= O(N)
    int sum = graph[id]->importance;
    for(auto& eid: graph[id]->subordinates) {
      sum += sumDFS(graph, eid);
    }
    return sum;
  }
};

//Problem #3: LeetCode 323 - Number of Connected Components in an Undirected Graph
void visitDFS(vector<vector<int>>& graph, vector<bool>& visited, int i){
  visited[i] = true;
  for(auto& node: graph[i]){
    if (visited[node]) continue;
    visitDFS(graph, visited, node);
  }
}

int countComponents(int n, vector<vector<int>>& edges){
  vector<vector<int>> graph(n);
  vector<bool> visited(n, false);
  for(int i = 0; i < edges.size(); ++i){
    graph[edges[i][0]].push_back(edges[i][1]);
    graph[edges[i][1]].push_back(edges[i][0]);
  }

  int cc = 0;
  for(int i = 0; i < n; ++i){
    if (visited[i]) continue;
    visitDFS(graph, visited, i);
    ++cc;
  }
  return cc;
}

#pragma endregion

#pragma region MCS: Problem Set 2 Solutions
// Problem #1: LeetCode 1905 - Count Sub Islands
int di[4] = {0,0,1,-1};
int dj[4] = {1,-1,0,0};

bool subsetDFS(const vector<vector<int>>& grid1, vector<vector<int>>& grid2, int i, int j, int ROWS, int COLS){
  bool subset = (grid1[i][j] == grid2[i][j]);
  grid2[i][j] = 0;

  for(int d = 0; d < 4; ++d){
    int ni = i + di[d];
    int nj = j + dj[d];
    if (ni < 0 || ni >= ROWS || nj < 0 || nj >= COLS) continue;
    if(grid2[ni][nj] == 0) continue;
    subset &= subsetDFS(grid1, grid2, ni, nj, ROWS, COLS); 
  }

  return subset;
}

int countSubIslands(vector<vector<int>>& grid1, vector<vector<int>>& grid2) {
  int ROWS = grid2.size();
  int COLS = grid2[0].size();
  int count = 0;
  for(int i = 0; i < ROWS; ++i){
    for(int j = 0; j < COLS; ++j){
      if (grid2[i][j] == 0) continue;
      if (subsetDFS(grid1, grid2, i, j, ROWS, COLS)) ++count;
    }
  }
  return count;
}

//Problem #2: LeetCode 1034 - Coloring A Border
void colorDFS(vector<vector<int>>& grid, vector<vector<bool>>& visited, int i, int j, int ROWS, int COLS, int color){
  int myColor = grid[i][j];
  visited[i][j] = true;

  if (i == 0 || i == (ROWS - 1) || j == 0 || j == (COLS - 1)){
    grid[i][j] = color; // color me if at grid borders
  }

  for(int d = 0; d < 4; ++d){
    int ni = i + di[d];
    int nj = j + dj[d];
    
    if (ni < 0 || ni >= ROWS || nj < 0 || nj >= COLS || visited[ni][nj]) continue;

    if (grid[ni][nj] != myColor){
      grid[i][j] = color;   // color me if my adj-cell is different
      continue;
    }

    colorDFS(grid, visited, ni, nj, ROWS, COLS, color);
  }
}

vector<vector<int>> colorBorder(vector<vector<int>>& grid, int row, int col, int color) {
  int ROWS = grid.size(), COLS = grid[0].size();
  vector<vector<bool>> visited(ROWS, vector<bool>(COLS, false));
  colorDFS(grid, visited, row, col, ROWS, COLS, color);
  return grid;
}

// Problem #3: LeetCode 1254 - Number of Closed Islands
bool closedDFS(vector<vector<int>>& grid, int i, int j, const int ROWS, const int COLS){
  bool is_closed = true;
  grid[i][j] = 1;

  for(int d = 0; d < 4; ++d){
    int ni = i + di[d];
    int nj = j + dj[d];    
    if (ni < 0 || ni >= ROWS || nj < 0 || nj >= COLS){
      is_closed = false;
      continue;
    }
    if (grid[ni][nj]) continue;
    is_closed &= closedDFS(grid, ni, nj, ROWS, COLS);
  }

  return is_closed;
}

int closedIsland(vector<vector<int>>& grid) {
  int ROWS = grid.size(), COLS = grid[0].size();
  int count = 0;
  for(int i = 1; i < (ROWS - 1); ++i){
    for(int j = 1; j < (COLS - 1); ++j){
      if (grid[i][j]) continue;
      count += closedDFS(grid, i, j, ROWS, COLS);
    }
  }
  return count;
}

//Problem #4: LeetCode 1559 - Detect Cycles in 2D Grid
bool isCyclicDFS(vector<vector<char>>& grid, int i, int j, int pi, int pj){
  static const int ROWS = grid.size(), COLS = grid[0].size();
  char org_value = grid[i][j]; 
  grid[i][j] -= 'a';  // mark as visited <unique number per path>

  for(int d = 0; d < 4; ++d){
    int ni = i + di[d];
    int nj = j + dj[d];
    if (ni < 0 || ni >= ROWS || nj < 0 || nj >= COLS) continue;
    if (pi == ni && pj == nj) continue; // skip previous cell
    if (grid[ni][nj] == grid[i][j]) return true; // if next-cell is visited -> we have cycle
    if (grid[ni][nj] != org_value) continue; // skip if next-cell isn't part of the current path
    if (isCyclicDFS(grid, ni, nj, i, j)) return true;
  }

  return false;
}

bool containsCycle(vector<vector<char>>& grid) {
  for(int i = 0; i < grid.size(); ++i){
    for(int j = 0; j < grid[0].size(); ++j){
      if (grid[i][j] < 'a') continue;
      if (isCyclicDFS(grid, i, j, -1, -1)) return true;
    }
  }
  return false;
}

#pragma endregion

#pragma region MCS: Problem Set 3 Solutions
// Problem #1: LeetCode 1743 - Restore the Array From Adjacent Pairs
using GRAPH = unordered_map<int, vector<int>>;

void restoreDFS(GRAPH& graph, vector<int>& nums, int i, int pi){
  nums.push_back(i);
  for(auto& ni: graph[i]){
    if (ni == pi) continue;
    restoreDFS(graph, nums, ni, i);
  }
}

vector<int> restoreArray(vector<vector<int>>& adjacentPairs) {
  GRAPH graph;
  for(int i = 0; i < adjacentPairs.size(); ++i){
    graph[adjacentPairs[i][0]].push_back(adjacentPairs[i][1]);
    graph[adjacentPairs[i][1]].push_back(adjacentPairs[i][0]);
  }

  int entry_point;
  for(auto& entry: graph){
    if (entry.second.size() != 1) continue;
    entry_point = entry.first;
    break;
  }

  vector<int> nums;
  nums.reserve(adjacentPairs.size() + 1);
  restoreDFS(graph, nums, entry_point, -1);
  return nums;
}

//Problem #2: LeetCode 1202 - Smallest String With Swaps
void getCC(vector<vector<int>>& graph, vector<bool>& visited, set<int>& cc, int i){
  cc.insert(i);
  visited[i] = true;

  for(auto& ni: graph[i]){
    if (visited[ni]) continue;
    getCC(graph, visited, cc, ni);
  }
}

vector<set<int>> getCCList(vector<vector<int>>& graph){
  vector<bool> visited(graph.size(), false);
  vector<set<int>> ccs;
  for(int i = 0; i < graph.size(); ++i){
    if (visited[i]) continue;
    set<int> newCC;
    getCC(graph, visited, newCC, i);
    ccs.push_back(newCC);
  }
  return ccs;
}

void processCC(const set<int>& cc, string& str){
  string ccStr = "";
  for(auto& i: cc) ccStr += str[i];
  sort(ccStr.begin(), ccStr.end());
  int cci = 0;
  for(auto& i: cc) str[i] = ccStr[cci++];
}

string smallestStringWithSwaps(string s, vector<vector<int>>& pairs) {
  vector<vector<int>> graph(s.size());
  for(auto& pair: pairs){
    graph[pair[0]].push_back(pair[1]);
    graph[pair[1]].push_back(pair[0]);
  }
  for(auto& cc: getCCList(graph)) processCC(cc, s);
  return s;
}

//Problem #3: Leetcode 128 - Longest Consecutive Sequence
int longestConsecutive(vector<int>& nums) {
  unordered_map<int, int> length;
  for(auto& n : nums){
    if (!length.count(n)) length[n] = 1;
    if (length.count(n+1)) length[n] = length[n+1] + 1;
    for(int i = n; length.count(i-1); --i) length[i-1] = length[i] + 1;
  }

  int longest = 0;
  for(auto& entry : length){
    longest = max(longest, entry.second);
  }
  return longest;
}

#pragma endregion

#pragma region MCS: Problem Set 4 Solutions
//Problem #1: LeetCode 785 - Is Graph Bipartite?
bool markDFS(vector<vector<int>>& graph, vector<int>& marks, int i, int m){
  marks[i] = m;

  for(auto& ni: graph[i]){
    if (marks[ni] == m) return false;
    if (marks[ni] != -1) continue;
    if (!markDFS(graph, marks, ni, !m)) return false;
  }

  return true;
}

bool isBipartite(vector<vector<int>>& graph) {
  vector<int> marks(graph.size(), -1);

  for(int i = 0; i < graph.size(); ++i){
    if (marks[i] != -1) continue;
    if (!markDFS(graph, marks, i, 0)) return false;
  }

  return true;
}

//Problem #2: LeetCode 1466 - Reorder Routes to Make All Paths Lead to the City Zero

int visitCityDFS(vector<vector<pair<int, bool>>>& graph, int i, int pi){
  int count = 0;

  for(auto& ni: graph[i]){
    if (ni.first == pi) continue;
    if (ni.second) ++count;
    count += visitCityDFS(graph, ni.first, i);
  }

  return count;
}

int minReorder(int n, vector<vector<int>>& connections) {
  vector<vector<pair<int, bool>>> graph(n);
  for(auto& edge: connections) {
    graph[edge[0]].push_back({edge[1], true});
    graph[edge[1]].push_back({edge[0], false});
  }

  return visitCityDFS(graph, 0, -1);
}

//Problem #3: LeetCode 1631. Path With Minimum Effort

bool limitedReachDFS(vector<vector<int>>& graph, vector<vector<bool>>& visited, int i, int j, int limit){
  const int ROWS = graph.size(), COLS = graph[0].size();
  if (i == (ROWS - 1) && j == (COLS - 1)) return true;

  visited[i][j] = true;

  for(int d = 0; d < 4; ++d){
    int ni = i + di[d];
    int nj = j + dj[d];
    if (ni < 0 || nj < 0 || ni >= ROWS || nj >= COLS || visited[ni][nj]) continue;

    int diff = abs(graph[i][j] - graph[ni][nj]);
    if (diff > limit) continue;
    if (limitedReachDFS(graph, visited, ni, nj, limit)) return true;
  }

  return false;
}

int minimumEffortPath(vector<vector<int>>& heights) {

  int start = 0, end = 1000000, result = INT32_MAX;

  while(start <= end){
    int mid = start + (end - start) / 2;
    vector<vector<bool>> visited (heights.size(), vector<bool>(heights[0].size(), false));
    if (limitedReachDFS(heights, visited, 0,0, mid)){
      end = mid - 1;
      result = mid;
    }else{
      start = mid + 1;
    }
  }

  return result;
}

//Problem #4: O(V) Cycle Detection in undirected

bool cycleDFS(GRAPH& graph, vector<bool>& visited, int i, int pi){
  if (visited[i]) return true;
  visited[i] = true;
  for(auto& ni: graph[i]){
    if (ni == pi) continue;
    if (cycleDFS(graph, visited, ni, i)) return true;
  }

  return false;
}

bool has_cycle_undirected(GRAPH& graph){
  vector<bool> visited(graph.size());

  for(int i = 0; i < graph.size(); ++i){
    if (visited[i]) continue;
    if (cycleDFS(graph, visited, i, -1)) return true;
  }

  return false;
}

#pragma endregion
