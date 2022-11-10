#include<vector>
#include<list>

template<typename T, typename H = std::hash<T>> class HashSet {
public:
#pragma region Type Declarations
	class Iterator;
	using value_type = T;
	using hasher_type = H;
	using size_type = unsigned;
#pragma endregion

#pragma region The Class Interface
	explicit HashSet(size_type table_size = MIN_TABLE_SIZE, float max_load_factor = 0.8)
		: _table{ regulate_size(table_size) }, _max_load_factor{ max_load_factor }{};

	HashSet(const std::initializer_list<value_type>& items)
		:_table{ regulate_size(items.size(), 2) } {
		insert(items);
	}
#pragma endregion

#pragma region The Standard Interface

	void insert(const std::initializer_list<value_type>& items) {
		for (auto& item : items) insert(item);
	}

	void insert(const value_type& value) {
		if (has(value)) return;
		TableIterator bucket = _table.begin() + index(value);
		bucket->insert(bucket->end(), value);
		++_size;
		auto_rehash();
	};

	Iterator find(const value_type& value) {
		TableIterator tab = _table.begin() + index(value);
		if (tab->empty()) return end();
		for (auto bkt = tab->begin(); bkt != tab->end(); ++bkt) {
			if (*bkt == value) return Iterator(_table, tab, bkt);
		}
		return end();
	};
	
	bool has(const value_type& value) {
		if (find(value) == end()) return false;
		return true;
	}

	void erase(const value_type& value) throw (std::exception) {
		 erase(find(value));
	}

	void erase(const Iterator& pointer) throw (std::exception) {
		if (pointer == end()) throw std::exception("no such item");
		pointer.remove();
		--_size;
	};
	
	void reserve(size_type table_size) {
		if (table_size > capacity()) rehash(table_size);
	}

	void reset() {
		_table.clear();
		_table.resize(MIN_TABLE_SIZE);
		_table.shrink_to_fit();
		_size = 0;
	}

	void clear() {
		for (auto& bucket : _table) {
			bucket.clear();
		}
		_size = 0;
	}

	Iterator begin() {
		if (empty()) return end();
		TableIterator tab = _table.begin();
		while (tab->empty()) { ++tab; }
		return Iterator(_table, tab, tab->begin());
	};

	Iterator end() { return Iterator(_table, _table.end()); };

	float load_factor() const { return float(size()) / capacity(); }

	float max_load_factor() const { return _max_load_factor; }
	
	size_type capacity() const { return _table.size(); }
	
	size_type size() const { return _size; }
	
	bool empty() const { return size() == 0; }

#pragma endregion

private:

#pragma region Private Type Declarations
	using Bucket = std::list<value_type>;
	using Table = std::vector<Bucket>;
	using BucketIterator = typename Bucket::iterator;
	using TableIterator = typename Table::iterator;
#pragma endregion

#pragma region Member Variables
	static const size_type MIN_TABLE_SIZE = 10;
	static const size_type MAX_TABLE_SIZE = UINT_MAX;
	Table _table{ MIN_TABLE_SIZE };
	size_type _size{};
	hasher_type _hasher{};
	float _max_load_factor{ 0.8 };
#pragma endregion

#pragma region Helper Functions

	size_type index(const value_type& value, size_type table_size = 0) {
		if (table_size == 0) table_size = capacity();
		size_type hash_code = _hasher(value);
		return (hash_code % table_size);
	}
	
	void rehash(size_type new_size) throw(std::exception) {
		if (new_size == 0) throw std::exception("table size can't be zero");
		Table new_table(new_size);

		for (auto& value : *this) {
			TableIterator tab = new_table.begin() + index(value, new_size);
			tab->insert(tab->end(), value);
		}

		_table = new_table;
	}
	
	void auto_rehash() {
		if (capacity() == MAX_TABLE_SIZE) return;
		if (load_factor() > max_load_factor()) {
			rehash(regulate_size(capacity(), 2));
		}
	}

	size_type regulate_size(size_type table_size, size_type multiplier = 1) {
		if (table_size == MAX_TABLE_SIZE) return table_size;
		table_size = std::max(table_size, MIN_TABLE_SIZE);
		if (table_size * multiplier >= table_size) {
			table_size = table_size * multiplier;
		}
		else {
			table_size = MAX_TABLE_SIZE;
		}
		return table_size;
	}

#pragma endregion
};

template<typename T, typename H> class HashSet<T,H>::Iterator {
public:
	Iterator(const Table& table, const TableIterator& tab, const BucketIterator& bkt = {})
		: tab_ptr{ &table }, tab_itr{ tab }, bkt_itr{ bkt }{};

	const value_type& operator*() const { return *bkt_itr; };

	const value_type* operator->() const { return &(*bkt_itr); };

	bool operator==(const Iterator& pointer) const {
		if (tab_ptr != pointer.tab_ptr) return false;
		if (tab_itr != pointer.tab_itr) return false;
		if (tab_itr == tab_ptr->end()) return true;
		return (bkt_itr == pointer.bkt_itr);
	};

	bool operator!=(const Iterator& pointer) const {
		return !(*this == pointer);
	};

	Iterator& operator++() throw(std::exception) {
		if (end_of_table()) throw std::exception("out of range: no more items.");
		++bkt_itr;
		if (end_of_bucket()) {
			// get next bucket
			do { ++tab_itr; } while (!end_of_table() && tab_itr->empty());
			if (!end_of_table()) bkt_itr = tab_itr->begin();
		}
		return *this;
	};

	Iterator operator++(int) throw(std::exception) {
		Iterator curr = *this;
		++(*this);
		return curr;
	};

	Iterator& operator--() throw(std::exception) {
		if (end_of_table() || start_of_bucket()) {
			// get prev bucket
			do { --tab_itr; } while (!start_of_table() && tab_itr->empty());
			if (start_of_table()) throw std::exception("out of range: no more items.");
			bkt_itr = tab_itr->end();
		}
		--bkt_itr;
		return *this;
	};

	Iterator operator--(int) throw(std::exception) {
		Iterator curr = *this;
		--(*this);
		return curr;
	};

private:
	const Table* tab_ptr{};
	TableIterator tab_itr{};
	BucketIterator bkt_itr{};
	friend class HashSet;

#pragma region Helper Functions
	bool start_of_table() const { return (tab_itr == tab_ptr->begin()); }
	bool end_of_table() const { return (tab_itr == tab_ptr->end()); }

	bool start_of_bucket() const { return (bkt_itr == tab_itr->begin()); }
	bool end_of_bucket() const { return (bkt_itr == tab_itr->end()); }

	void remove() const { tab_itr->erase(bkt_itr); }
#pragma endregion

};