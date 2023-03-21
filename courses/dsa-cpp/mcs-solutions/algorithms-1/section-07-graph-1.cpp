/**
* Mahmoud Elnagar
*/
#include <iostream>
#include <vector>
#include <algorithm>
#include <unordered_set>
#include <set>
#include <map>
using namespace std;

#pragma region MCS: Problem Set 1 Solutions

//Problem #1: Edge List representation
struct edge{
  int from;         // source
  int to;           // destination
  int weight;       // cost

  edge(int src, int dst, int cost)
    : from{src}, to{dst}, weight{cost} {};

  bool operator<(const edge& e) const { return weight < e.weight; }
  bool operator>(const edge& e) const { return weight > e.weight; }
  bool operator<=(const edge& e) const { return weight <= e.weight; }
  bool operator>=(const edge& e) const { return weight >= e.weight; }
  bool operator==(const edge& e) const { return weight == e.weight; }
  bool operator!=(const edge& e) const { return weight != e.weight; }

  void print() const {
    cout<<from<<" --("<<weight<<")--> "<<to<<endl;
  }
};

typedef vector<edge> EDGRAPH;

void add_edge(EDGRAPH &graph, int from, int to, int cost){ //O(1)
  graph.push_back({from, to, cost}); 
}

void print_adjaceny_matrix(EDGRAPH &graph){ //O(E)
  for(auto& e: graph ) e.print();
}

//Problem #2: Adjacency Hashset Representation
typedef vector<unordered_set<int>> HGRAPH;

void add_directed_edge(HGRAPH &graph, int from, int to) {
  graph[from].insert(to);
}

void add_undirected_edge(HGRAPH &graph, int from, int to) {
  graph[from].insert(to);
  graph[to].insert(from);
}

void print_adjaceny_matrix(HGRAPH &graph) {
  for(int i = 0; i < graph.size(); ++i){
    cout<<i<<": ";
    for(auto& dst: graph[i]) cout<<dst<<" ";
    cout<<endl;
  }
}

//Problem #3: Adjacency matrix with multiple edges

typedef vector<vector<vector<int>>> MGRAPH;

void add_directed_edge(MGRAPH &graph, int from, int to, int cost){ // O(1)
  graph[from][to].push_back(cost);
}

void print_adjaceny_matrix(MGRAPH &graph){
  for(int from = 0; from < graph.size(); ++from){
    for(int to = 0; to < graph[from].size(); ++to){
      for(auto& cost: graph[from][to]){
        cout<<from<<" --("<< cost <<")--> "<< to<<endl;
      }
    }
  }
}

#pragma endregion

#pragma region MCS: Problem Set 2 Solutions
//Problem #1: Adjacency-based repr for flights v1

struct AirportDest{
  string name;  // Destination
  int cost;     // Flight Cost

  AirportDest(const string& to, int cost) : name{to}, cost{cost} {};

  bool operator<(const AirportDest& a) const {
    if (name == a.name) return cost < a.cost;
    return name < a.name;
  }
};

typedef map<string, multiset<AirportDest>> FLIGHTS1;

void add_flight(FLIGHTS1& flights, const string& from, const string& to, int cost){
  flights[from].insert({to, cost});
}

void print_flights(const FLIGHTS1& flights){
  for(auto& from: flights){
    cout<<"Flights from "<<from.first<<" :"<<endl;
    for(auto& to: from.second){
      cout<<"\tTo "<<to.name<<" with cost "<<to.cost<<endl;
    }
  }
}

//Problem #2: Adjacency-based repr for flights v2
class Flights2{
public:
  Flights2(size_t nodes){
    graph.resize(nodes, vector<edge>(0));
  }

  void add_flight(const string& from, const string& to, int cost){
    int from_id = get_airport_id(from);
    int to_id = get_airport_id(to);
    add_directed_edge(from_id, to_id, cost);
  }

  void print_flights(){
    for(int from = 0; from < graph.size(); ++from){
      cout<<"Flights from "<<get_airport_name(from)<<" :"<<endl;
      for(auto& flight: graph[from]){
        cout<<"\tTo "<<get_airport_name(flight.to)<<" with cost "<<flight.cost<<endl;
      }
    }
  }

private:

#pragma region Standard Graph
  struct edge{
    int to;
    int cost;
  };

  vector<vector<edge>> graph;

  void add_directed_edge(int from, int to, int cost) {
    graph[from].push_back({to, cost});
  }
  
  void add_undirected_edge(int from, int to, int cost) {
    graph[from].push_back({to, cost});
    graph[to].push_back({from, cost});
  }
#pragma endregion

  map<string, int> airportsID;
  map<int, string> airportName;

  int get_airport_id(const string& name){
    if (airportsID.count(name)) return airportsID[name];
    int id = airportsID.size();
    airportsID[name] = id;
    airportName[id] = name;
    return id;
  }

  string get_airport_name(int id){ return airportName[id]; }

};

//Problem #3: Image as a graph
HGRAPH image_graph(int r, int c){
  HGRAPH graph (r*c);
  int di[] = {1,-1,0,0};
  int dj[] = {0,0,1,-1};

  for(int i = 0; i < r; ++i){
    for(int j = 0; j < c; ++j){
      int node_num = i * c + j;
      for(int d = 0; d < 4 ; ++d){
        int ni = i + di[d];
        int nj = j + dj[d];
        if (ni < 0 || ni >= r || nj < 0 || nj >= c) continue;
        int adj_num = ni * c + nj;
        add_directed_edge(graph, node_num, adj_num);
      }
    }
  }

  return graph;
}

//Problem #4: Print Chains
void print_chain(HGRAPH &graph, int from){
  if (from >= graph.size()) return;
  cout<<from<<" ";
  if (graph[from].size() == 0) return;
  int next = *(graph[from].begin());
  print_chain(graph, next);
}

//Problem #5: Print Paths of length 2
void print_paths_len_2(HGRAPH &graph){
  for(int a = 0; a < graph.size(); ++a){
    for(auto& b : graph[a]){
      for(auto& c: graph[b]) cout<<a<<" -> "<<b<<" -> "<<c<<endl;
    }
  }
}

#pragma endregion

#pragma region MCS: Problem Set 3 Solutions
//Problem #1: Find a sink
typedef vector<vector<int>> GRAPH;

void add_directed_edge(GRAPH &graph, int from, int to) {
	graph[from][to] += 1;
}

bool is_sink(GRAPH& graph, int n){ //O(N)
  for(int i = 0 ; i < graph.size(); ++i){
    if (graph[n][i] != 0) return false;
    if (n == i) continue;
    if (graph[i][n] == 0) return false;   
  }
  return true;
}

int find_universal_sink_slow(GRAPH &graph){ //O(N^2)
  for(int i = 0; i < graph.size(); ++i){
    if (is_sink(graph, i)) return i;
  }
  return -1;
}

int find_universal_sink_fast(GRAPH &graph){ //O(N)
  int sz = graph.size(), i = 0, j = 0;

  while((i < sz) && (j < sz)){
    if (graph[i][j]) ++i; else ++j;
  }

  if (i >= sz || !is_sink(graph, i)) return -1;

  return i;
}

#pragma endregion
