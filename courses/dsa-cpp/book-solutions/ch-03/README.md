# Chapter 3 Solutions

## Reinforcement Problems

- **[R-3.1](./r-01.cpp)**
- **R-3.2**: They will appear in the same relative order; since the algorithm only shifts elements `A[j]` as long as they are greater than the `cur` element.
- **[R-3.3](./r-03.cpp)**
- **[R-3.4](./r-04.cpp)**
- **R-3.5**:
  > **FP Definition**
  >
  > `List` is _one of_:
  >
  > - `Empty`
  > - `Cons(value, List)`
- **R-3.6**: Implemented in [Singly-Linked List](../../implementations/singly-linked-list.cpp) using private `length` variable to keep track of list size.
- **R-3.7**:

  ```C++
  //... inside the Singly-Linked List class
  Node* penultimate() throw(std::runtime_error) {
    if (size() < 2) throw std::runtime_error("list isn't long enough to have a penultimate node");
    Node* curr = head;
    while (curr->next != tail) curr = curr->next;
    return curr;
  }
  ```

- **R-3.8** and **R-3.9**: Implemented in [Doubly-Linked List](../../implementations/doubly-linked-list.cpp)

- **R-3.10**: Implemented in [MCS-DLL: Problem 1 in Problem Set 2](../../mcs-solutions/section-05-dll.cpp)

- **[R-3.11](./r-11.cpp)**:
  ```C++
  template<typename T>
  T max(T* begin, T* end) throw(std::out_of_range) {
  	if (begin == end) throw std::out_of_range("No max in an empty array");
  	T* next = begin; next++;
  	T value1 = *begin;
  	if (next == end) return value1;
  	T value2 = max(next, end);
  	return (value1 > value2) ? value1 : value2;
  }
  ```
  - Time Complexity: O(n)
  - Space Complexity: O(n) call stacks, _can be O(1) in a language that supports TCO_
- **R-3.12**:

  ```C++
  template<typename T>
  void remove_randomly(T* arr, unsigned n) {
    if (n == 0) return;
    for (auto i = (rand() % n); i < (n-1); ++i) {
      arr[i] = arr[i + 1];
    }
    arr[n - 1] = {};
    remove_randomly(arr, n - 1);
  }
  ```
