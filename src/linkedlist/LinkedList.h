#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include <memory>
#include <optional>
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
  std::optional<T> value_;
  // next_ points to the tail; use the template parameter to avoid compilation
  // errors.
  std::shared_ptr<LinkedList<T>> next_;

 public:
  // Copy and move constructors / assignments preserve structural sharing.
  LinkedList(const LinkedList& other)
      : value_(other.value_), next_(other.next_) {}

  LinkedList(LinkedList&& other) noexcept
      : value_(std::move(other.value_)), next_(std::move(other.next_)) {}

  LinkedList& operator=(const LinkedList& other) {
    if (this == &other) return *this;
    value_ = other.value_;
    next_ = other.next_;
    return *this;
  }

  LinkedList& operator=(LinkedList&& other) noexcept {
    if (this == &other) return *this;
    value_ = std::move(other.value_);
    next_ = std::move(other.next_);
    return *this;
  }

  // Construct a node with an optional value and a tail pointer.
  LinkedList(const std::optional<T>& value,
             const std::shared_ptr<LinkedList<T>>& next)
      : value_(value), next_(next) {}

  ~LinkedList() {
    // Eagerly destroy unique tail chain to avoid deep recursive destruction
    // when this instance is the unique owner of the tail nodes.
    std::shared_ptr<LinkedList<T>> cur = std::move(next_);
    while (cur != nullptr && cur.unique()) {
      std::shared_ptr<LinkedList<T>> next = std::move(cur->next_);
      cur.reset();
      cur = std::move(next);
    }
  }

  // Return true if list is nullptr or represents an empty node.
  static bool IsEmpty(const std::shared_ptr<LinkedList<T>>& list) {
    return !list || list->value_ == std::nullopt;
  }

  // Return head value. Throws std::runtime_error if list is null/empty.
  static T Head(const std::shared_ptr<LinkedList<T>>& list) {
    if (IsEmpty(list))
      throw std::runtime_error("Cannot call head on an empty list");
    return list->value_.value();
  }

  // Return tail pointer. Throws std::runtime_error if list is null/empty.
  static std::shared_ptr<LinkedList<T>> Tail(
      const std::shared_ptr<LinkedList<T>>& list) {
    if (IsEmpty(list))
      throw std::runtime_error("Cannot call tail on an empty list");
    return list->next_;
  }

  // True if list contains exactly one element.
  static bool IsSingle(const std::shared_ptr<LinkedList<T>>& list) {
    if (IsEmpty(list)) return false;
    return IsEmpty(list->next_);
  }

  // Iterative length computation; nullptr/empty treated as 0.
  static int Length(const std::shared_ptr<LinkedList<T>>& list) {
    int count = 0;
    std::shared_ptr<LinkedList<T>> cur = list;
    while (!IsEmpty(cur)) {
      ++count;
      cur = cur->next_;
    }
    return count;
  }

  // Create an empty list node.
  static std::shared_ptr<LinkedList<T>> Empty() {
    return std::make_shared<LinkedList<T>>(std::nullopt, nullptr);
  }

  // Prepend element (functional): returns new list with element as head.
  static std::shared_ptr<LinkedList<T>> Cons(
      const T& element, const std::shared_ptr<LinkedList<T>>& list) {
    return std::make_shared<LinkedList<T>>(std::make_optional<T>(element),
                                           list);
  }

  // Single-element list helper.
  static std::shared_ptr<LinkedList<T>> Single(const T& element) {
    return Cons(element, Empty());
  }

  // Snoc: append an element to the end (functional). Builds a new list.
  static std::shared_ptr<LinkedList<T>> Snoc(
      const std::shared_ptr<LinkedList<T>>& list, const T& element) {
    return Append(list, Single(element));
  }

  // Init: return all but the last element. Throws on null/empty.
  static std::shared_ptr<LinkedList<T>> Init(
      const std::shared_ptr<LinkedList<T>>& list) {
    std::shared_ptr<LinkedList<T>> head = Empty();
    std::shared_ptr<LinkedList<T>> tail = head;
    std::shared_ptr<LinkedList<T>> cur = list;
    while (!IsSingle(cur)) {
      tail->next_ = Single(cur->value_.value());
      tail = tail->next_;
      cur = cur->next_;
    }
    tail->next_ = Empty();
    return head->next_;
  }

  // Return last element. Throws on null/empty.
  static T Last(const std::shared_ptr<LinkedList<T>>& list) {
    if (IsEmpty(list))
      throw std::runtime_error("Cannot call last on an empty list");
    std::shared_ptr<LinkedList> cur = list;
    while (!IsSingle(cur)) {
      cur = cur->next_;
    }
    return cur->value_.value();
  }

  // Append (concatenate) two lists: returns a new list representing listA ++
  // listB.
  static std::shared_ptr<LinkedList<T>> Append(
      const std::shared_ptr<LinkedList<T>>& listA,
      const std::shared_ptr<LinkedList<T>>& listB) {
    std::shared_ptr<LinkedList<T>> head = Empty();
    std::shared_ptr<LinkedList<T>> tail = head;
    std::shared_ptr<LinkedList<T>> cur = listA;
    while (!IsEmpty(cur)) {
      tail->next_ = Single(cur->value_.value());
      tail = tail->next_;
      cur = cur->next_;
    }
    tail->next_ = listB;
    return head->next_;
  }

  // Indexing: 0-based. Throws std::out_of_range if index invalid.
  static T Index(const std::shared_ptr<LinkedList<T>>& list, int index) {
    if (index < 0) throw std::out_of_range("Index out of range");
    std::shared_ptr<LinkedList> cur = list;
    while (index >= 0 && !IsEmpty(cur)) {
      if (index == 0) return Head(cur);
      index--;
      cur = cur->next_;
    }
    throw std::out_of_range("Index out of range");
  }
};

#endif  // LINKED_LIST_H
