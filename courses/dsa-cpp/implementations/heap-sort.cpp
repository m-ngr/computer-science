
template<typename T> void swap(T& a, T& b) {
	T temp = a;
	a = b;
	b = temp;
}

template<typename T, typename C>
void heapify_down(T begin, T end, C heap_order, int index = 0) {
	int size = end - begin;
	int end_index = size / 2;
	while (index < end_index) {
		int child_index = 2 * index + 1; //left-child
		int r_index = child_index + 1; //right-child
		if ((r_index < size) && (heap_order(begin[r_index], begin[child_index]))) {
			child_index = r_index;
		}
		if (heap_order(begin[index], begin[child_index])) return;
		swap(begin[index], begin[child_index]);
		index = child_index;
	}
}

template<typename T, typename C>
void heap_sort(T begin, T end, C heap_order) {
	int size = end - begin;
	int end_index = size / 2;

	auto reverse_order = [&](auto a, auto b) {
		return !heap_order(a, b);
	};
	// heapify: bottom-up approach O(n)
	for (int i = end_index - 1; i >= 0; --i) {
		heapify_down(begin, end, reverse_order, i);
	}
	// poping items from front to back: O(nlogn)
	for (int i = 0; i < size; ++i) {
		--end;
		swap(*begin, *end);
		heapify_down(begin, end, reverse_order, 0);
	}
}

template<typename T>
void heap_sort(T begin, T end, bool ascending = true) {
	int size = end - begin;
	int end_index = size / 2;

	auto reverse_order = [&](auto a, auto b) {
		bool r = a < b;
		return (ascending && !r) || (!ascending && r);
	};
	// heapify: bottom-up approach O(n)
	for (int i = end_index - 1; i >= 0; --i) {
		heapify_down(begin, end, reverse_order, i);
	}
	// poping items from front to back: O(nlogn)
	for (int i = 0; i < size; ++i) {
		--end;
		swap(*begin, *end);
		heapify_down(begin, end, reverse_order, 0);
	}
}

