
template<typename T> class Range {
public:
	Range(const T& lower, const T& upper) : lower_bound{ lower }, upper_bound{ upper }{};
	T lower_bound;
	T upper_bound;
};

template<typename T, typename Iterator> 
Iterator binarySearch(Iterator begin, Iterator end, const T& value) {
	int size = end - begin;
	int low = 0, high = size - 1;
	while (low <= high) {
		int mid = low + (high - low) / 2;
		if (begin[mid] == value) {
			return begin + mid;
		} else if (begin[mid] < value) {
			low = mid + 1;
		} else {
			high = mid - 1;
		}
	}
	return end;
}

template<typename T, typename Iterator>
Iterator lowerBound(Iterator begin, Iterator end, const T& value) {
	int size = end - begin;
	int low = 0, high = size - 1;

	while (low <= high) {
		int mid = low + (high - low) / 2;
		if (begin[mid] < value) {
			low = mid + 1;
		} else {
			high = mid - 1;
		}
	}

	return begin + (high + 1);
}

template<typename T, typename Iterator>
Iterator upperBound(Iterator begin, Iterator end, const T& value) {
	int size = end - begin;
	int low = 0, high = size - 1;

	while (low <= high) {
		int mid = low + (high - low) / 2;
		if (begin[mid] <= value) {
			low = mid + 1;
		} else {
			high = mid - 1;
		}
	}

	return begin + (high + 1);
}

template<typename T, typename Iterator>
Range<Iterator> equalRange(Iterator begin, Iterator end, const T& value) {
	return { lowerBound(begin,end,value), upperBound(begin,end,value) };
}

template<typename T, typename Iterator>
Iterator firstOccurrence(Iterator begin, Iterator end, const T& value) {
	int size = end - begin;
	int low = 0, high = size - 1;
	int index = size;

	while (low <= high) {
		int mid = low + (high - low) / 2;
		if (begin[mid] == value) {
			index = mid;
			high = mid - 1;
		} else if (begin[mid] < value) {
			low = mid + 1;
		} else {
			high = mid - 1;
		}
	}

	return begin + index;
}

template<typename T, typename Iterator>
Iterator lastOccurrence(Iterator begin, Iterator end, const T& value) {
	int size = end - begin;
	int low = 0, high = size - 1;
	int index = size;

	while (low <= high) {
		int mid = low + (high - low) / 2;
		if (begin[mid] == value) {
			index = mid;
			low = mid + 1;
		} else if (begin[mid] < value) {
			low = mid + 1;
		} else {
			high = mid - 1;
		}
	}

	return begin + index;
}

#pragma region The Container Interface

template<typename T, typename Container>
auto binarySearch(const Container& container, const T& value) {
	return binarySearch(container.begin(), container.end(), value);
}

template<typename T, typename Container>
auto lowerBound(const Container& container, const T& value) {
	return lowerBound(container.begin(), container.end(), value);
}

template<typename T, typename Container>
auto upperBound(const Container& container, const T& value) {
	return upperBound(container.begin(), container.end(), value);
}

template<typename T, typename Container>
auto equalRange(const Container& container, const T& value) {
	return equalRange(container.begin(), container.end(), value);
}

template<typename T, typename Container>
auto firstOccurrence(const Container& container, const T& value) {
	return firstOccurrence(container.begin(), container.end(), value);
}

template<typename T, typename Container>
auto lastOccurrence(const Container& container, const T& value) {
	return lastOccurrence(container.begin(), container.end(), value);
}

#pragma endregion