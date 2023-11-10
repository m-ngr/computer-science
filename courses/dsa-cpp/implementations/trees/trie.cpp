#include <iostream>
#include <unordered_map>
#include <vector>

class Trie{
  struct TrieNode {
    std::unordered_map<char, TrieNode*> children;
    bool is_terminal{false};
  };
public:

#pragma region The Class Interface
  Trie(){ root = new TrieNode(); }

  ~Trie(){ remove_trie(root); }

  Trie(const std::initializer_list<std::string>& word_list) : Trie(){
    insert(word_list);
  }

  Trie(const std::vector<std::string>& word_list) : Trie(){
    insert(word_list);
  }

  Trie(const Trie& trie) : Trie(){
    insert(trie);
  }

  Trie(Trie&& trie){ move_trie(trie); }

	Trie& operator=(const Trie& trie) {
		clear();
		insert(trie);
		return *this;
	}

	Trie& operator=(Trie&& trie) {
    clear();
		move_trie(trie);
		return *this;
	}

#pragma endregion

#pragma region The Standard Interface

  void insert(const std::initializer_list<std::string>& word_list){
    for (auto& word : word_list) insert(word);
  }

  void insert(const std::vector<std::string>& word_list){
    for (auto& word : word_list) insert(word);
  }

  void insert(const Trie& trie){
    insert(trie.get_words());
  }

	void insert(const std::string& word) {
    TrieNode* t = root;
    for(auto ch : word) {
      if (t->children.count(ch) == 0){
        t->children[ch] = new TrieNode();
      }
      t = t->children[ch];
    }
    t->is_terminal = true;
    ++word_count;
	}

  void remove(const std::initializer_list<std::string>& word_list){
    for (auto& word : word_list) remove(word);
  }

  void remove(const std::vector<std::string>& word_list){
    for (auto& word : word_list) remove(word);
  }

  void remove(const std::string& word) {
    remove_helper(root, word, 0);
  }

  void clear() {
    remove_trie(root);
    root = new TrieNode();
  }

	bool has_word(const std::string& word) const {
    const TrieNode* t = node_at(word);
		return (t && t->is_terminal);
	}

	bool has_prefix(const std::string& prefix) const {
    return node_at(prefix);
	}

  std::vector<std::string> get_words(const std::string& prefix = "") const {
    const TrieNode* t = node_at(prefix);
    if (!t) return {};
    std::vector<std::string> words;
    get_words_helper(t, prefix, words);
    return words;
  }

  size_t size() const {return word_count;}

  bool empty() const {return size() == 0;}

#pragma endregion

private:
  TrieNode* root{};
  size_t word_count{};

#pragma region Helper Functions

  void get_words_helper(const TrieNode* node, const std::string& word, std::vector<std::string>& result) const {
    if (!node) return;
    if (node->is_terminal) result.push_back(word);
    for(auto& next : node->children){
      get_words_helper(next.second, word + next.first, result);
    }
  }

  const TrieNode* node_at(const std::string& str) const {
    const TrieNode* t = root;

    for(auto& ch : str){
      if (t->children.count(ch) == 0) return nullptr;
      t = t->children.at(ch);
    }

		return t;
  }

  bool remove_helper(TrieNode* node, const std::string& word, int i){
    if(i == word.size()){
      if(!node->is_terminal) return false;
      node->is_terminal = false;
      --word_count;
      return node->children.empty();
    }

    if (node->children.count(word[i]) == 0) return false;

    bool remove_child = remove_helper(node->children[word[i]], word, i + 1);

    if(remove_child){
      delete node->children[word[i]];
      node->children.erase(word[i]);
      return (node->children.empty() && !node->is_terminal);
    }

    return false;
  }

  void remove_trie(TrieNode* node){
    if(!node) return;
    for(auto& ch : node->children){
      remove_trie(ch.second);
    }
    delete node;
  }

  void move_trie(Trie& trie){
    std::swap(word_count, trie.word_count);
    std::swap(root, trie.root);
  }

#pragma endregion

};
