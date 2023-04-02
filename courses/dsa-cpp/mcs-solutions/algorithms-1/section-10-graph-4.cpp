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
//Problem #1: The lexicographical order
vector<int> topSortMin(const vector<vector<int>>& graph){
  vector<int> indegree(graph.size(), 0);
  for(auto& from: graph){
    for(auto& to: from){
      ++indegree[to];
    }
  }

  priority_queue<int,vector<int>, greater<int>> q;
  for(int i = 0; i < indegree.size(); ++i){
    if (indegree[i] == 0) q.push(i);
  }

  vector<int> result;
  while(!q.empty()){
    int node = q.top(); q.pop();
    result.push_back(node);

    for(auto& ni: graph[node]){
      --indegree[ni];
      if (indegree[ni] == 0) q.push(ni);
    }
  }

  if(result.size() != graph.size()) return {};
  return result;
}

//Problem #2: LeetCode 1136 - Parallel Courses
int parallelCourses(int n, vector<vector<int>> &prerequisites){
  vector<int> indegree(n, 0);
  vector<vector<int>> graph(n);

  for(auto& edge: prerequisites){
    graph[edge[0]-1].push_back(edge[1]-1);
    ++indegree[edge[1]-1];
  }

  queue<int> q;
  for(int i = 0; i < n; ++i){
    if (indegree[i] == 0) q.push(i);
  }

  int semesters = 0, count = 0;

  for(int size = q.size(); !q.empty(); ++semesters, size = q.size()){
    while(size--){
      int node = q.front(); q.pop();
      ++count;
      for(auto& ni: graph[node]){
        --indegree[ni];
        if (indegree[ni] == 0) q.push(ni);
      }      
    }
  }

  if (count != n) return -1;

  return semesters;
}

#pragma endregion

#pragma region MCS: Problem Set 2 Solutions
//Problem 1#: LeetCode 444 - Sequence Reconstruction
bool shortestSuperSeq(vector<int> &org, vector<vector<int>> &seq) {
  const int N = org.size();
  vector<vector<int>> graph(N);
  vector<int> indegree(N, 0);

  for(auto& s: seq){
    int from = s[0] - 1;
    for(int i = 1; i < s.size(); ++i){
      int to = s[i] - 1;
      graph[from].push_back(to);
      ++indegree[to];
      from = to;
    }
  }

  queue<int> q;

  for(int i = 0; i < N; ++i){
    if (indegree[i] == 0) q.push(i);
  }

  int counter = 0;

  while(!q.empty()){
    if (q.size() > 1) return false; // Not Unique
    int node = q.front(); q.pop();
    if (org[counter] != node + 1) return false; // org not superseq
    ++counter;

    for(auto& ni : graph[node]){
      --indegree[ni];
      if (indegree[ni] == 0) q.push(ni);
    }
  }

  return (counter == N); 
}

//Problem #2: LeetCode 310 - Minimum Height Trees
vector<int> findMinHeightTrees(int n, vector<vector<int>>& edges) {
  if (n == 1) return {0};
  vector<vector<int>> graph(n);
  vector<int> conn(n, 0);

  for(auto& edge : edges){
    graph[edge[0]].push_back(edge[1]);
    graph[edge[1]].push_back(edge[0]);
    ++conn[edge[0]];
    ++conn[edge[1]];
  }

  queue<int> q;
  for(int i = 0; i < n; ++i){
    if (conn[i] == 1) q.push(i);
  }

  int count = n;

  for(int size = q.size(); !q.empty(); size = q.size()){
    if (count < 3) break;
    count -= size;
    while(size--){
      int node = q.front(); q.pop();
      for(auto& ni: graph[node]){
        --conn[ni];
        if (conn[ni] == 1) q.push(ni);
      }
    }
  }

  vector<int> res;
  while(!q.empty()) res.push_back(q.front()), q.pop();
  return res;
}

//Problem #3: LeetCode 329 - Longest Increasing Path in a Matrix
int di[4] = {0,0,1,-1};
int dj[4] = {1,-1,0,0};

// DFS-based Solution
int getPathLength(vector<vector<int>>& matrix, vector<vector<int>>& length, int i, int j){
  const int ROWS = matrix.size(), COLS = matrix[0].size();
  int len = 1;

  for(int d = 0; d < 4; ++d){
    int ni = i + di[d];
    int nj = j + dj[d];
    if (ni < 0 || ni >= ROWS || nj < 0 || nj >= COLS) continue;
    if (matrix[i][j] < matrix[ni][nj]){
      int newLen = 1;
      if (length[ni][nj] == -1){
        newLen += getPathLength(matrix, length, ni, nj);
      }else{
        newLen += length[ni][nj];
      }
      
      len = max(len, newLen);
    }
  }

  length[i][j] = len;
  return len;
}

int longestIncreasingPath(vector<vector<int>>& matrix) {
  const int ROWS = matrix.size(), COLS = matrix[0].size();
  vector<vector<int>> length(ROWS, vector<int>(COLS, -1));

  int longest = 0;

  for(int i = 0; i < ROWS; ++i){
    for(int j = 0; j < COLS; ++j){
      if (length[i][j] != -1) continue;
      longest = max(longest, getPathLength(matrix, length, i, j));
    }
  }

  return longest;
}

// Topological-based Solution
int getDegree(vector<vector<int>>& matrix, int i, int j){
  const int ROWS = matrix.size(), COLS = matrix[0].size();
  int ndegree = 0;

  for(int d = 0; d < 4; ++d){
    int ni = i + di[d];
    int nj = j + dj[d];
    if (ni < 0 || ni >= ROWS || nj < 0 || nj >= COLS) continue;
    if(matrix[i][j] > matrix[ni][nj]) ++ndegree;
  }

  return ndegree;
}

int longestIncreasingPath(vector<vector<int>>& matrix) {
  const int ROWS = matrix.size(), COLS = matrix[0].size();
  vector<vector<int>> indegree(ROWS, vector<int>(COLS));
  queue<pair<int, int>> q;

  for(int i = 0; i < ROWS; ++i){
    for(int j = 0; j < COLS; ++j){
      indegree[i][j] = getDegree(matrix, i, j);
      if (indegree[i][j] == 0) q.push({i, j});
    }
  }

  int len = 0;
  for(int size = q.size(); !q.empty(); ++len, size = q.size()){
    while(size--){
      int i = q.front().first, j = q.front().second;
      q.pop();

      for(int d = 0; d < 4; ++d){
        int ni = i + di[d];
        int nj = j + dj[d];
        if (ni < 0 || ni >= ROWS || nj < 0 || nj >= COLS) continue;
        if(matrix[i][j] >= matrix[ni][nj]) continue;

        --indegree[ni][nj];
        if (indegree[ni][nj] == 0) q.push({ni, nj});
      }
    }
  }

  return len;
}

#pragma endregion

#pragma region MCS: Problem Set 3 Solutions
//Problem #: LeetCode 2050 - Parallel Courses III
// DFS-based Solution
int timeDFS(vector<vector<int>>& graph, vector<int>& length, vector<int>& time, int i){
  int t = 0;

  for(auto& ni: graph[i]){
    int newTime;
    if (length[ni] == 0){
      newTime = timeDFS(graph, length, time, ni);
    }else{
      newTime = length[ni];
    }
    t = max(t, newTime);
  }

  t += time[i];
  length[i] = t;
  return t;
}

int minimumTime(int n, vector<vector<int>>& relations, vector<int>& time) {
  vector<vector<int>> graph(n);
  vector<int> length(n, 0);
  for(auto&edge: relations){
    graph[edge[0]-1].push_back(edge[1]-1);
  }

  int longest = 0;

  for(int i = 0; i < n; ++i){
    if (length[i] != 0) continue;
    longest = max(longest, timeDFS(graph, length, time, i));
  }

  return longest;
}

// vector-based Slow Solution
int removeSmallest(vector<int>& nodes, vector<int>& time){
  int si, node;
  int smallest = INT32_MAX;
  
  for(int i = 0; i < nodes.size(); ++i){
    if (time[nodes[i]] < smallest){
      smallest = time[nodes[i]];
      si = i;
    }
  }

  node = nodes[si];
  swap(nodes[si], nodes.back()); nodes.pop_back();

  for(auto& ni: nodes){
    time[ni] -= smallest;
  }

  return node;
}

int minimumTime(int n, vector<vector<int>>& relations, vector<int>& time) {
  vector<vector<int>> graph(n);
  vector<int> indegree(n, 0);
  for(auto&edge: relations){
    graph[edge[0]-1].push_back(edge[1]-1);
    ++indegree[edge[1]-1];
  }

  vector<int> v;
  for(int i = 0; i < n; ++i){
    if (indegree[i] == 0) v.push_back(i);
  }

  int t = 0;

  while(!v.empty()){
    int node = removeSmallest(v, time);
    t += time[node];

    for(auto& ni : graph[node]){
      --indegree[ni];
      if (indegree[ni] == 0) v.push_back(ni);
    }
  }

  return t;
}

// Topological-based Solution
int minimumTime(int n, vector<vector<int>>& relations, vector<int>& time) {
  vector<vector<int>> graph(n);
  vector<int> indegree(n, 0), totalTime(n, 0);

  for(auto&edge: relations){
    graph[edge[0]-1].push_back(edge[1]-1);
    ++indegree[edge[1]-1];
  }

  queue<int> q;
  for(int i = 0; i < n; ++i){
    if (indegree[i] != 0) continue;
    q.push(i);
    totalTime[i] = time[i];
  }

  int tmax = 0;

  while(!q.empty()){
    int node = q.front(); q.pop();
    int t = totalTime[node];
    tmax = max(tmax, t);
    for(auto& ni : graph[node]){
      --indegree[ni];
      totalTime[ni] = max(totalTime[ni], time[ni] + t);
      if (indegree[ni] == 0) q.push(ni);
    }    
  }

  return tmax;
}

//Problem #2: LeetCode 1857 - Largest Color Value in a Directed Graph
struct Path{
  vector<int> colors;
  int frequency;

  Path(){
    colors.resize(26, 0);
    frequency = 0;
  }

  int addColor(char c){
    c = c - 'a';
    ++colors[c];
    frequency = max(frequency, colors[c]);
    return frequency;
  }

  void addPath(const Path& p){
    for(int i = 0; i < colors.size(); ++i){
      if (p.colors[i] > colors[i]){
        colors[i] = p.colors[i];
      }
    }
    frequency = max(frequency, p.frequency);
  }

};

int largestPathValue(string colors, vector<vector<int>>& edges) {
  const int N = colors.size();
  vector<vector<int>> graph(N);
  vector<int> indegree(N, 0);
  vector<Path> path(N);

  for(auto&edge: edges){
    graph[edge[0]].push_back(edge[1]);
    ++indegree[edge[1]];
  }

  queue<int> q;
  for(int i = 0; i < N; ++i){
    if (indegree[i] == 0) q.push(i);
  }

  int value = 0, count = 0;

  while(!q.empty()){
    int node = q.front(); q.pop();
    ++count;

    value = max(value, path[node].addColor(colors[node]));

    for(auto& ni : graph[node]){
      --indegree[ni];
      path[ni].addPath(path[node]);
      if (indegree[ni] == 0) q.push(ni);
    }    
  }

  if (count != N) return -1;
  return value;
}

//Problem #3: LeetCode 1591 - Strange Printer II
bool hasCycles(vector<vector<int>>& graph){
  vector<int> indegree(graph.size(), 0);
  for(auto& node : graph){
    for(auto& to : node){
      ++indegree[to];
    }
  }

  queue<int> q;
  for(int i = 0; i < graph.size(); ++i){
    if (indegree[i] == 0) q.push(i);
  }

  int count = 0;
  while(!q.empty()){
    int node = q.front(); q.pop();
    ++count;

    for(auto& ni: graph[node]){
      --indegree[ni];
      if (indegree[ni] == 0) q.push(ni);
    }
  }

  return (count != graph.size());
}

bool isPrintable(vector<vector<int>>& targetGrid) {
  const int ROWS = targetGrid.size(), COLS = targetGrid[0].size();
  const int COLORS = 60 + 1;
  vector<vector<int>> graph(COLORS);
  vector<int> rmax(COLORS, -1), rmin(COLORS, ROWS);
  vector<int> cmax(COLORS, -1), cmin(COLORS, COLS);

  for(int r = 0; r < ROWS; ++r){
    for(int c = 0; c < COLS; ++c){
      int color = targetGrid[r][c];
      rmin[color] = min(rmin[color], r);
      rmax[color] = max(rmax[color], r);
      cmin[color] = min(cmin[color], c);
      cmax[color] = max(cmax[color], c);
    }
  }

  for (int color = 0; color < COLORS; ++color)
    for (int r = rmin[color]; r <= rmax[color]; ++r)
      for (int c = cmin[color]; c <= cmax[color]; ++c)
        if (targetGrid[r][c] != color)
          graph[color].push_back(targetGrid[r][c]);

  return !hasCycles(graph);
}

#pragma endregion
