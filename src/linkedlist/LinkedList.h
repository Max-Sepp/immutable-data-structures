#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include <memory>
#include <optional>
#include <stdexcept>

template <typename T>
class LinkedList {
 private:
  std::optional<T> value_;
  std::shared_ptr<LinkedList<T>> next_;

 public:
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

  LinkedList(const std::optional<T>& value,
             const std::shared_ptr<LinkedList<T>>& next)
      : value_(value), next_(next) {}

  static bool IsEmpty(const std::shared_ptr<LinkedList<T>>& list) {
    return list->value_ == std::nullopt;
  }

  static T Head(const std::shared_ptr<LinkedList<T>>& list) {
    if (IsEmpty(list))
      throw std::runtime_error("Cannot call head on an empty list");
    return list->value_.value();
  }

  static std::shared_ptr<LinkedList<T>> Tail(
      const std::shared_ptr<LinkedList<T>>& list) {
    if (IsEmpty(list))
      throw std::runtime_error("Cannot call tail on an empty list");
    return list->next_;
  }

  static bool IsSingle(const std::shared_ptr<LinkedList<T>>& list) {
    return !IsEmpty(list) && IsEmpty(Tail(list));
  }

  static int Length(const std::shared_ptr<LinkedList<T>>& list) {
    if (IsEmpty(list)) return 0;
    return 1 + Length(Tail(list));
  }

  static std::shared_ptr<LinkedList<T>> Empty() {
    return std::make_shared<LinkedList<T>>(std::nullopt, nullptr);
  }

  static std::shared_ptr<LinkedList<T>> Cons(
      const T& element, const std::shared_ptr<LinkedList<T>>& list) {
    return std::make_shared<LinkedList<T>>(std::make_optional<T>(element),
                                           list);
  }

  static std::shared_ptr<LinkedList<T>> Single(const T& element) {
    return Cons(element, Empty());
  }

  static std::shared_ptr<LinkedList<T>> Snoc(
      const std::shared_ptr<LinkedList<T>>& list, const T& element) {
    return Append(list, Single(element));
  }

  static std::shared_ptr<LinkedList<T>> Init(
      const std::shared_ptr<LinkedList<T>>& list) {
    if (IsSingle(list)) return Empty();
    return Cons(Head(list), Init(Tail(list)));
  }

  static T Last(const std::shared_ptr<LinkedList<T>>& list) {
    if (IsSingle(list)) return Head(list);
    return Last(Tail(list));
  }

  static std::shared_ptr<LinkedList<T>> Append(
      const std::shared_ptr<LinkedList<T>>& listA,
      const std::shared_ptr<LinkedList<T>>& listB) {
    if (IsEmpty(listA)) return listB;
    return Cons(Head(listA), Append(Tail(listA), listB));
  }

  static T Index(const std::shared_ptr<LinkedList<T>>& list, int index) {
    if (IsEmpty(list)) throw std::out_of_range("Index out of range");
    if (index == 0) return Head(list);
    return Index(Tail(list), index - 1);
  }
};

#endif  // LINKED_LIST_H
