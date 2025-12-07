#include <iostream>
#include <string>

#include "linkedlist/LinkedList.h"

constexpr int LIST_SIZE = 1000;
constexpr int LIST1_SIZE = 2000;
constexpr int LIST2_SIZE = 3000;

template <typename T>
void PrintList(const std::string& name,
               const std::shared_ptr<LinkedList<T>>& list) {
  auto cur = list;
  std::cout << name << ": ";
  while (!LinkedList<T>::IsEmpty(cur)) {
    std::cout << LinkedList<T>::Head(cur) << ' ';
    cur = LinkedList<T>::Tail(cur);
  }
  std::cout << '\n';
}

int main(int argc, char** argv) {
  auto list = LinkedList<int>::Empty();

  for (int i = 0; i < LIST_SIZE; i++) {
    list = LinkedList<int>::Cons(i, list);
  }

  auto list1 = list;
  for (int i = LIST_SIZE; i < LIST1_SIZE; i++) {
    list1 = LinkedList<int>::Cons(i, list1);
  }

  auto list2 = list;
  for (int i = LIST1_SIZE; i < LIST2_SIZE; i++) {
    list2 = LinkedList<int>::Cons(i, list2);
  }

  return 0;
}
