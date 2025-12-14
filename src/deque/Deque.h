#ifndef DEQUE_DEQUE_H
#define DEQUE_DEQUE_H

#include <algorithm>
#include <stdexcept>

#include "Utils.h"
#include "linkedlist/LinkedList.h"

template <typename T>
class Deque {
  LinkedList<T> front_;
  LinkedList<T> back_;

  Deque RebalancedIfNecessary() const {
    if (IsEmpty() || IsSingle() || (!front_.IsEmpty() && !back_.IsEmpty()))
      return *this;

    if (front_.IsEmpty()) {
      const auto [new_back, new_reversed_front] =
          SplitAt(back_.Length() / 2, back_);
      return Deque(Reverse(new_reversed_front), new_back);
    }
    const auto [new_front, new_reversed_back] =
        SplitAt(front_.Length() / 2, front_);
    return Deque(new_front, Reverse(new_reversed_back));
  }

 public:
  // Move and copy constructors
  Deque(const Deque& other) : front_(other.front_), back_(other.back_) {}
  Deque(Deque&& other) noexcept
      : front_(std::move(other.front_)), back_(std::move(other.back_)) {}

  // Move and copy assignment
  Deque& operator=(const Deque& other) {
    if (this == &other) return *this;
    front_ = other.front_;
    back_ = other.back_;
    return *this;
  }
  Deque& operator=(Deque&& other) noexcept {
    if (this == &other) return *this;
    front_ = std::move(other.front_);
    back_ = std::move(other.back_);
    return *this;
  }

  Deque(LinkedList<T> front,  // NOLINT(*-easily-swappable-parameters)
        LinkedList<T> back)
      : front_(front), back_(back) {}

  static Deque FromList(const LinkedList<T>& list) {
    const auto [listA, listB] = SplitAt(list.Length() / 2, list);
    return Deque(listA, Reverse(listB));
  };

  LinkedList<T> ToList() const { return front_.Append(Reverse(back_)); };

  static Deque Empty() {
    return Deque(LinkedList<T>::Empty(), LinkedList<T>::Empty());
  };

  static Deque Single(const T& element) {
    return Deque(LinkedList<T>::Single(element), LinkedList<T>::Empty());
  };

  Deque Cons(const T& element) const {
    if (back_.IsEmpty()) return Deque(LinkedList<T>::Single(element), front_);
    return Deque(front_.Cons(element), back_);
  };

  Deque Snoc(const T& element) const {
    if (front_.IsEmpty()) return Deque(back_, LinkedList<T>::Single(element));
    return Deque(front_, back_.Cons(element));
  };

  T Head() const {
    if (IsEmpty())
      throw std::invalid_argument("Cannot call Head on an empty list");
    if (!front_.IsEmpty()) return front_.Head();
    return back_.Last();
  };

  Deque Tail() const {
    if (IsEmpty())
      throw std::invalid_argument("Cannot call Tail on an empty list");
    if (front_.IsEmpty() || back_.IsEmpty()) return Empty();
    return Deque(front_.Tail(), back_).RebalancedIfNecessary();
  };

  Deque Init() const {
    if (IsEmpty())
      throw std::invalid_argument("Cannot call Init on an empty list");
    if (front_.IsEmpty() || back_.IsEmpty()) return Empty();
    return Deque(front_, back_.Tail()).RebalancedIfNecessary();
  };

  T Last() const {
    if (IsEmpty())
      throw std::invalid_argument("Cannot call Last on an empty list");
    if (!back_.IsEmpty()) return back_.Head();
    return front_.Last();
  };

  [[nodiscard]] bool IsEmpty() const {
    return front_.IsEmpty() && back_.IsEmpty();
  };

  [[nodiscard]] bool IsSingle() const {
    return !IsEmpty() && (front_.IsEmpty() || back_.IsEmpty());
  };

  [[nodiscard]] int Length() const { return front_.Length() + back_.Length(); };

  Deque Append(const Deque& listB) const {
    return FromList(this->ToList().Append(listB.ToList()));
  }

  T Index(int index) const {
    if (index < 0) throw std::out_of_range("Index out of range");
    const int front_length = front_.Length();
    if (index < front_length) return front_.Index(index);
    index -= front_length;
    if (const int back_length = back_.Length(); index < back_length)
      return back_.Index(back_length - 1 - index);
    throw std::out_of_range("Index out of range");
  };
};

#endif  // DEQUE_DEQUE_H
