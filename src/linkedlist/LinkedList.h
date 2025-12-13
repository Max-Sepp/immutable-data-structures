#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include <memory>
#include <stdexcept>

// Immutable singly-linked list with structural sharing.
// Representation:
// - An empty list is represented by a node whose value_ == std::nullopt.
// - Non-empty lists have value_ engaged and next_ pointing to the tail.
// Design notes:
// - All public operations are functional: they return new lists and preserve
//   existing lists (structural sharing via shared_ptr).
// - Many operations are recursive for clarity rather than performance.
// - Methods that require a non-empty list throw std::runtime_error when
// violated.
template <typename T>
class LinkedList {
 private:
  struct Node {
    T value_;
    LinkedList next_;
    Node(const T& value, const LinkedList& next) : value_(value), next_(next) {}
  };
  // If value == nullptr then the list is empty
  std::shared_ptr<Node> value_;

 public:
  // Copy and move constructors / assignments preserve structural sharing.
  LinkedList(const LinkedList& other) : value_(other.value_) {}

  LinkedList(LinkedList&& other) noexcept : value_(std::move(other.value_)) {}

  LinkedList& operator=(const LinkedList& other) {
    if (this == &other) return *this;
    value_ = other.value_;
    return *this;
  }

  LinkedList& operator=(LinkedList&& other) noexcept {
    if (this == &other) return *this;
    value_ = std::move(other.value_);
    return *this;
  }

  LinkedList() : value_(nullptr) {}

  LinkedList(T element, LinkedList next)
      : value_(std::make_shared<Node>(element, next)) {}

  ~LinkedList() {
    // Eagerly destroy unique tail chain to avoid deep recursive destruction
    // when this instance is the unique owner of the tail nodes.
    LinkedList cur = std::move(value_->next_);
    while (cur.value_ != nullptr && cur.value_.unique()) {
      LinkedList next = std::move(cur.value_->next_);
      cur.value_.reset();
      cur = std::move(next);
    }
  }

  // Return true if list is nullptr or represents an empty node.
  [[nodiscard]] bool IsEmpty() const { return value_ == nullptr; }

  // Return head value. Throws std::runtime_error if list is null/empty.
  [[nodiscard]] T Head() const {
    if (IsEmpty())
      throw std::runtime_error("Cannot call head on an empty list");
    return value_->value_;
  }

  // Return tail pointer. Throws std::runtime_error if list is null/empty.
  [[nodiscard]] LinkedList Tail() const {
    if (IsEmpty())
      throw std::runtime_error("Cannot call tail on an empty list");
    return value_->next_;
  }

  // True if list contains exactly one element.
  [[nodiscard]] bool IsSingle() const { return !IsEmpty() && Tail().IsEmpty(); }

  // Iterative length computation; nullptr/empty treated as 0.
  [[nodiscard]] int Length() const {
    int count = 0;
    Node* cur = this->value_.get();
    while (cur != nullptr) {
      count++;
      cur = cur->next_.value_.get();
    }
    return count;
  }

  // Create an empty list node.
  [[nodiscard]] static LinkedList Empty() { return LinkedList(); }

  // Prepend element (functional): returns new list with element as head.
  [[nodiscard]] LinkedList Cons(const T& element) const {
    return LinkedList(element, *this);
  }

  // Use the += operator to be an infix cons as it is right associative unlike +
  // which is left associative and hence does not work
  [[nodiscard]] friend LinkedList operator+=(const T& element,
                                             const LinkedList& list) {
    return list.Cons(element);
  }

  // Single-element list helper.
  [[nodiscard]] static LinkedList Single(const T& element) {
    return element += Empty();
  }

  // Snoc: append an element to the end (functional). Builds a new list.
  [[nodiscard]] LinkedList Snoc(const T& element) const {
    return this->Append(Single(element));
  }

  // Init: return all but the last element. Throws on null/empty.
  [[nodiscard]] LinkedList Init() const {
    LinkedList head{};
    Node* tail = nullptr;
    Node* cur = this->value_.get();
    while (cur != nullptr && cur->next_.value_ != nullptr) {
      if (tail == nullptr) {
        // First element of the list
        head.value_ = std::make_shared<Node>(cur->value_, LinkedList());
        tail = head.value_.get();
      } else {
        tail->next_ = LinkedList(cur->value_, LinkedList());
        tail = tail->next_.value_.get();
      }
      cur = cur->next_.value_.get();
    }
    return head;
  }

  // Return last element. Throws on null/empty.
  [[nodiscard]] T Last() const {
    if (IsEmpty())
      throw std::runtime_error("Cannot call last on an empty list");
    Node* cur = this->value_.get();
    while (!cur->next_.IsEmpty()) {
      cur = cur->next_.value_.get();
    }
    return cur->value_;
  }

  // Append (concatenate) two lists: returns a new list representing listA ++
  // listB.
  [[nodiscard]] LinkedList Append(const LinkedList& other) const {
    if (IsEmpty()) return other;
    LinkedList head{};
    Node* tail = nullptr;
    Node* cur = this->value_.get();
    while (cur != nullptr) {
      if (tail == nullptr) {
        // First element of the list
        head.value_ = std::make_shared<Node>(cur->value_, LinkedList());
        tail = head.value_.get();
      } else {
        tail->next_ = LinkedList(cur->value_, LinkedList());
        tail = tail->next_.value_.get();
      }
      cur = cur->next_.value_.get();
    }
    tail->next_ = other;
    return head;
  }

  // Indexing: 0-based. Throws std::out_of_range if index invalid.
  [[nodiscard]] T Index(int index) const {
    if (index < 0) throw std::out_of_range("Index out of range");
    Node* cur = this->value_.get();
    for (int i = 0; i < index; i++) {
      if (cur == nullptr) throw std::out_of_range("Index out of range");
      cur = cur->next_.value_.get();
    }
    if (cur == nullptr) throw std::out_of_range("Index out of range");
    return cur->value_;
  }
};

#endif  // LINKED_LIST_H
