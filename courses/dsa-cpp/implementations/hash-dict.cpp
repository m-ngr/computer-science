#include<vector>
#include<list>

template<typename K, typename V, typename H = std::hash<K>> class HashDict {
public:
#pragma region Type Declarations
	class Iterator;
	class Entry;
	class Range;
	using key_type = K;
	using value_type = V;
	using hasher_type = H;
	using size_type = unsigned;
#pragma endregion

#pragma region The Class Interface
	explicit HashDict(size_type table_size = MIN_TABLE_SIZE, float max_load_factor = 0.8)
		: _table{ regulate_size(table_size) }, _max_load_factor{ max_load_factor }{};

	HashDict(const std::initializer_list<Entry>& items)
		:_table{ regulate_size(items.size(), 2) } {
		insert(items);
	}
#pragma endregion

#pragma region The Standard Interface

	value_type& at(const key_type& key) throw (std::exception) {
		auto item = find(key);
		if (item == end()) throw std::exception("no such item");
		return item->value();
	}

	value_type& operator[](const key_type& key) {
		auto item = find(key);
		if (item == end()) item = insert(key, {});
		return item->value();
	}

	void insert(const std::initializer_list<Entry>& items) {
		for (auto& item : items) insert(item);
	}

	Iterator insert(const Entry& item) { return insert(item.key(), item.value()); }

	Iterator insert(const key_type& key, const value_type& value) {
		++_size;
		auto_rehash();
		Iterator item = find(key);
		if (item == end()) {	
			TableIterator tab = _table.begin() + index(key);
			BucketIterator bkt = tab->insert(tab->end(), Entry(key, value));
			item = Iterator(_table, tab, bkt);
		}
		else {
			item = item.insert(key, value);
		}
		return item;
	};

	Iterator find(const key_type& key) {
		TableIterator tab = _table.begin() + index(key);
		if (tab->empty()) return end();
		for (auto bkt = tab->begin(); bkt != tab->end(); ++bkt) {
			if (bkt->key() == key) return Iterator(_table, tab, bkt);
		}
		return end();
	};
	
	Range find_all(const key_type& key) {
		Iterator first = find(key);
		if (first == end()) return Range(end(), end());
		Iterator last = first;
		while (last != end() && last->key() == key) ++last;
		return Range(first, last);
	}

	size_type count(const key_type& key) {
		size_type c = 0;
		for (auto& e : find_all(key)) ++c;
		return c;
	}

	bool has(const key_type& key) {
		if (find(key) == end()) return false;
		return true;
	}

	void erase(const key_type& key) throw (std::exception) {
		 erase(find(key));
	}

	void erase(const Iterator& pointer) throw (std::exception) {
		if (pointer == end()) throw std::exception("no such item");
		pointer.remove();
		--_size;
	};
	
	void erase(const Range& range) {
		for (auto it = range.begin(); it != range.end(); ++it) {
			erase(it);
		}
	};

	void erase_all(const key_type& key) { erase(find_all(key)); }

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

#pragma region The Functional Interface

	template<typename F>
	HashDict filter(F& predicate) {
		HashDict result(capacity(), max_load_factor());
		for (auto& entry : *this) {
			if (predicate(entry)) result.insert(entry);
		}
		return result;
	}

	template <typename K2, typename V2, typename H2 = std::hash<K2>>
	HashDict<K2, V2, H2> map(typename HashDict<K2, V2, H2>::Entry(*transform) (const Entry&)) {
		HashDict<K2, V2, H2> result(capacity(), max_load_factor());
		for (auto& entry : *this) {
			result.insert(transform(entry));
		}
		return result;
	}

	template <typename R>
	R reduce(const R& init, R(*accumulate) (const R&, const Entry&)) {
		R result = init;
		for (auto& entry : *this) {
			result = accumulate(result, entry);
		}
		return result;
	}

	template <typename R>
	R reduce_values(const R& init, R(*accumulate) (const R&, const value_type&)) {
		R result = init;
		for (auto& entry : *this) {
			result = accumulate(result, entry.value());
		}
		return result;
	}

	template <typename F>
	bool exists(F& predicate) {
		for (auto& entry : *this) {
			if (predicate(entry)) return true;
		}
		return false;
	}

	template <typename F>
	Iterator find(F& predicate) {
		for (auto it = begin(); it != end(); ++it) {
			if (predicate(*it)) return it;
		}
		return end();
	}

	template<typename F>
	HashDict& filter_self(F& predicate) {

		for (auto it = begin(); it != end();) {
			auto next = it; ++next;
			if (!predicate(*it)) erase(it);
			it = next;
		}

		return *this;
	}

	HashDict& remove_all(const value_type& value) {
		auto pred = [=](auto e) {return e.value() != value; };
		return filter_self(pred);
	}

#pragma endregion

private:

#pragma region Private Type Declarations
	using Bucket = std::list<Entry>;
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

	size_type index(const key_type& key, size_type table_size = 0) {
		if (table_size == 0) table_size = capacity();
		size_type hash_code = _hasher(key);
		return (hash_code % table_size);
	}
	
	void rehash(size_type new_size) throw(std::exception) {
		if (new_size == 0) throw std::exception("table size can't be zero");
		Table new_table(new_size);

		for (auto& entry : *this) {
			TableIterator tab = new_table.begin() + index(entry.key(), new_size);
			tab->insert(tab->end(), entry);
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

template<typename K, typename V, typename H> class HashDict<K,V,H>::Iterator {
public:
	Iterator(const Table& table, const TableIterator& tab, const BucketIterator& bkt = {})
		: tab_ptr{ &table }, tab_itr{ tab }, bkt_itr{ bkt }{};

	Entry& operator*() const { return *bkt_itr; };

	Entry* operator->() const { return &(*bkt_itr); };

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
	friend class HashDict;

#pragma region Helper Functions
	bool start_of_table() const { return (tab_itr == tab_ptr->begin()); }
	bool end_of_table() const { return (tab_itr == tab_ptr->end()); }

	bool start_of_bucket() const { return (bkt_itr == tab_itr->begin()); }
	bool end_of_bucket() const { return (bkt_itr == tab_itr->end()); }

	void remove() const { tab_itr->erase(bkt_itr); }

	Iterator insert(const key_type& key, const value_type& value) const {
		BucketIterator new_bkt = tab_itr->insert(bkt_itr, Entry(key, value));
		return Iterator(*tab_ptr, tab_itr, new_bkt);
	}
#pragma endregion

};

template<typename K, typename V, typename H> class HashDict<K,V,H>::Entry {
public:
	Entry(const key_type& key = {}, const value_type& value = {})
		:_key{ key }, _value{ value } {};

	const key_type& key() const { return _key; }
	const value_type& value() const { return _value; }
	value_type& value() { return _value; }
	void set_value(const value_type& value) { _value = value; }

private:
	key_type _key{};
	value_type _value{};
};

template<typename K, typename V, typename H> class HashDict<K,V,H>::Range {
public:
	Range(const Iterator& begin, const Iterator& end) :_begin{ begin }, _end{ end } {};
	Iterator begin() const { return _begin; }
	Iterator end() const { return _end; }
private:
	Iterator _begin;
	Iterator _end;
};