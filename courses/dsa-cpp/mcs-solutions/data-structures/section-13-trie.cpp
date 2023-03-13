/**
* Mahmoud Elnagar
*/
#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <algorithm>
using namespace std;

#pragma region MCS: Problem Set 1 Solutions
//Problem #1: Iterative version
//Problem #2: minimal prefix
class Trie{
public:
  //Problem #1: Iterative version
  void insert(const string& str) {
    Trie* t = this;

    for(int i = 0; i < str.size(); ++i){
      int idx = str[i] - 'a';
      if (t->child[idx] == NULL){
        t->child[idx] = new Trie();
      }
      t = t->child[idx];
    }

    t->is_leaf = true;
	}

	bool word_exist(const string& str) {
    Trie* t = this;

    for(int i = 0; i < str.size(); ++i){
      int idx = str[i] - 'a';
      if (t->child[idx] == NULL) return false;
      t = t->child[idx];
    }

		return t->is_leaf;
	}

	bool prefix_exist(const string& str) {
    Trie* t = this;

    for(int i = 0; i < str.size(); ++i){
      int idx = str[i] - 'a';
      if (t->child[idx] == NULL) return false;
      t = t->child[idx];
    }

		return true;
	}

  //Problem #2: minimal prefix
  string first_word_prefix(const string &str) {
    Trie* t = this;

    for(int i = 0; i < str.size(); ++i){
      int idx = str[i] - 'a';
      if (t->child[idx] == NULL) return str;
      t = t->child[idx];
      if (t->is_leaf) return str.substr(0, i+1); 
    }

		return str;
  }

private:
  static const int MAX_CHAR = 26; 
  bool is_leaf {false};
  Trie* child[MAX_CHAR] {};
};

//Problem #3: Is suffix
class SuffTrie{
public:

  void insert(const string& str) {
    SuffTrie* t = this;

    for(int i = (int)str.size() - 1; i >= 0; --i){
      int idx = str[i] - 'a';
      if (t->child[idx] == NULL){
        t->child[idx] = new SuffTrie();
      }
      t = t->child[idx];
    }

    t->is_leaf = true;
	}

	bool word_exist(const string& str) {
    SuffTrie* t = this;

    for(int i = (int)str.size() - 1; i >= 0; --i){
      int idx = str[i] - 'a';
      if (t->child[idx] == NULL) return false;
      t = t->child[idx];
    }

		return t->is_leaf;
	}

	bool suffix_exist(const string& str) {
    SuffTrie* t = this;

    for(int i = (int)str.size() - 1; i >= 0; --i){
      int idx = str[i] - 'a';
      if (t->child[idx] == NULL) return false;
      t = t->child[idx];
    }

		return true;
	}

private:
  static const int MAX_CHAR = 26; 
  bool is_leaf {false};
  SuffTrie* child[MAX_CHAR] {};
};

//Problem #4: Memory Efficient
class MapTrie{
public:
  void insert(const string& str) {
    MapTrie* t = this;

    for(int i = 0; i < str.size(); ++i){
      char idx = str[i];
      if (t->child[idx] == NULL){
        t->child[idx] = new MapTrie();
      }
      t = t->child[idx];
    }

    t->is_leaf = true;
	}

	bool word_exist(const string& str) {
    MapTrie* t = this;

    for(int i = 0; i < str.size(); ++i){
      char idx = str[i];
      if (t->child[idx] == NULL) return false;
      t = t->child[idx];
    }

		return t->is_leaf;
	}

	bool prefix_exist(const string& str) {
    MapTrie* t = this;

    for(int i = 0; i < str.size(); ++i){
      char idx = str[i];
      if (t->child[idx] == NULL) return false;
      t = t->child[idx];
    }

		return true;
	}

private:
  bool is_leaf {false};
  map<char, MapTrie*> child;
};

//Problem #5: OS Paths
class PathTrie{
public:
  void insert(const vector<string> &path) {
    PathTrie* t = this;

    for(int i = 0; i < path.size(); ++i){
      string idx = path[i];
      if (t->child[idx] == NULL){
        t->child[idx] = new PathTrie();
      }
      t = t->child[idx];
    }

    t->is_leaf = true;
	}

	bool path_exist(const vector<string> &path) {
    PathTrie* t = this;

    for(int i = 0; i < path.size(); ++i){
      string idx = path[i];
      if (t->child[idx] == NULL) return false;
      t = t->child[idx];
    }

		return t->is_leaf;
	}

	bool subpath_exist(const vector<string> &path) {
    PathTrie* t = this;

    for(int i = 0; i < path.size(); ++i){
      string idx = path[i];
      if (t->child[idx] == NULL) return false;
      t = t->child[idx];
    }

		return true;
	}

private:
  bool is_leaf {false};
  map<string, PathTrie*> child;
};

#pragma endregion

#pragma region MCS: Problem Set 2 Solutions

class Trie2{
public:
  void insert(const string& str) {
    Trie2* t = this;

    for(int i = 0; i < str.size(); ++i){
      char idx = str[i];
      if (t->child[idx] == NULL){
        t->child[idx] = new Trie2();
      }
      t = t->child[idx];
    }

    t->is_leaf = true;
	}

	bool word_exist(const string& str) {
    Trie2* t = this;

    for(int i = 0; i < str.size(); ++i){
      char idx = str[i];
      if (t->child[idx] == NULL) return false;
      t = t->child[idx];
    }

		return t->is_leaf;
	}

	bool prefix_exist(const string& str) {
    Trie2* t = this;

    for(int i = 0; i < str.size(); ++i){
      char idx = str[i];
      if (t->child[idx] == NULL) return false;
      t = t->child[idx];
    }

		return true;
	}

  //Problem #1: Listing tree content
  void get_all_strings (vector<string> &res, const string& prefix = ""){
    if (is_leaf) res.push_back(prefix);
    for(auto& entry: child){
      if (entry.second){
        entry.second->get_all_strings(res, prefix + entry.first);
      }
    }
  }

  //Problem #2: Autocomplete
  void auto_complete(const string &str, vector<string> &res){
    Trie2* t = this;

    for(int i = 0; i < str.size(); ++i){
      char idx = str[i];
      if (t->child[idx] == NULL) return;
      t = t->child[idx];
    }

    t->get_all_strings(res, str);
  }

  //Problem #3: Word with exactly single letter change
  bool word_exist_with_1_change(const string& str){
    if (str.size() == 0) return false;
    for(auto& entry: child){
      if (!entry.second ) continue;
      if (entry.first != str[0]) return entry.second->word_exist(str.substr(1));
      else return entry.second->word_exist_with_1_change(str.substr(1));
    }
		return false;
  }
  
  //P-Set #3: Problem #1 Helper
  void print_subs(const string& str){ // O(S*L)
    for(int i = 0; i < str.size(); ++i){
      Trie2* t = this;
      /* 
      O(L): this loop will only iterate L times (L: max query length (depth of the tree)).
        actually it is O(min(L, S)); since we know L < S, hence O(L);
      */
      for(int j = i; j < str.size(); ++j){
        if (t->child[str[j]] == NULL) break;
        t = t->child[str[j]];
        if (t->is_leaf) cout<<str.substr(i, j-i+1)<<endl;
      }
    }
  }

private:
  bool is_leaf {false};
  map<char, Trie2*> child;
};

#pragma endregion

#pragma region MCS: Problem Set 3 Solutions
//Problem #1: Find all substrings
void list_substrs_v1(const string& str, vector<string> &queries){
  Trie2 trie;
  // O(S^2)
  for(int i = 0; i < str.size(); ++i){
    for(int w = 1; w <= str.size() - i; ++w){
      trie.insert(str.substr(i, w));
    }
  }
  // O(Q*L)
  for(auto& q: queries){
    if (trie.prefix_exist(q)) cout<<q<<endl;
  }
}

void list_substrs_v2(const string& str, vector<string> &queries){
  Trie2 trie;
  for(auto& q: queries) trie.insert(q); // O(Q*L)
  trie.print_subs(str); //O(S*L)
}

//Problem #2: Prefix and Suffix Search
class WordTrie_1{
public:

  void insert(const string& str, int index){
    WordTrie_1* t = this;
    for(int i = 0; i < str.size(); ++i){
      char key = str[i];
      if (t->childs[key] == NULL){
        t->childs[key] = new WordTrie_1();
      }
      t = t->childs[key];
      t->indexs.push_back(index);
    }
  }

  vector<int> find_indexs(const string& str){
    WordTrie_1* t = this;

    for(int i = 0; i < str.size(); ++i){
      char key = str[i];
      if (t->childs[key] == NULL) return {};
      t = t->childs[key];
    }

    return t->indexs;
  }

private:
  map<char, WordTrie_1*> childs;
  vector<int> indexs;
};

class WordFilter_1 {
public:
  WordFilter_1(vector<string>& words) {
    set<string> words_set;

    for(int i = words.size() - 1; i >= 0; --i) {
      if (words_set.insert(words[i]).second){
        pref_trie.insert(words[i], i);
        reverse(words[i].begin(), words[i].end());
        suff_trie.insert(words[i], i);
      }
    }
  }
    
  int f(string pref, string suff) {
    reverse(suff.begin(), suff.end());
    vector<int> sa = pref_trie.find_indexs(pref);
    vector<int> sb = suff_trie.find_indexs(suff);

		reverse(sa.begin(), sa.end());
		reverse(sb.begin(), sb.end());

		vector<int> common_data;	// It will contain all intersection between both
		set_intersection(sa.begin(), sa.end(), sb.begin(), sb.end(), back_inserter(common_data));

		if (common_data.size() == 0) return -1;
		return common_data.back();
  }

private:
  WordTrie_1 pref_trie;
  WordTrie_1 suff_trie;
};

class WordTrie_2{
public:

  void insert(const string& str, int word_index){
    WordTrie_2* t = this;
    for(int i = 0; i < str.size(); ++i){
      char key = str[i];
      if (t->childs[key] == NULL){
        t->childs[key] = new WordTrie_2();
        t->childs[key]->index = word_index;
      }
      t = t->childs[key];
    }
  }

  int find_indexs(const string& str){
    WordTrie_2* t = this;

    for(int i = 0; i < str.size(); ++i){
      char key = str[i];
      if (t->childs[key] == NULL) return -1;
      t = t->childs[key];
    }

    return t->index;
  }

private:
  map<char, WordTrie_2*> childs;
  int index = -1;
};

class WordFilter_2 {
public:
  WordFilter_2(vector<string>& words) {
    set<string> words_set;

    for(int i = words.size() - 1; i >= 0; --i) {
      if (words_set.insert(words[i]).second){
        string keyword = "$" + words[i];
        for(int j = words[i].size() - 1; j >= 0; --j){
          keyword = words[i][j] + keyword;
          trie.insert(keyword, i);
        }
      }
    }
  }
    
  int f(string pref, string suff) {
    return trie.find_indexs(suff + "$" + pref);
  }

private:
  WordTrie_2 trie;
};

#pragma endregion
