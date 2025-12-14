#ifndef UTILS_H
#define UTILS_H

#include <stdexcept>
#include <utility>

#include "linkedlist/LinkedList.h"

template <typename T>
std::pair<LinkedList<T>, LinkedList<T>> SplitAt(const int n,
                                                LinkedList<T> list) {
  if (n < 0) throw std::out_of_range("Invalid split Index");

  if (n == 0) return std::make_pair(LinkedList<T>::Empty(), list);

  const auto& [listA, listB] = SplitAt(n - 1, list.Tail());
  return std::make_pair(listA.Cons(list.Head()), listB);
}

template <typename T>
LinkedList<T> Reverse(LinkedList<T> list) {
  LinkedList<T> reversed_list = LinkedList<T>::Empty();
  while (!list.IsEmpty()) {
    reversed_list = reversed_list.Cons(list.Head());
    list = list.Tail();
  }
  return reversed_list;
}

#endif  // UTILS_H
