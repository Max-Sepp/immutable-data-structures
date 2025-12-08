#include <gtest/gtest.h>

#include <vector>

#include "linkedlist/LinkedList.h"

// Helper to convert list to vector for easy comparisons.
static std::vector<int> ToVector(const std::shared_ptr<LinkedList<int>>& list) {
  std::vector<int> out;
  auto cur = list;
  while (!LinkedList<int>::IsEmpty(cur)) {
    out.push_back(LinkedList<int>::Head(cur));
    cur = LinkedList<int>::Tail(cur);
  }
  return out;
}

TEST(LinkedList, EmptyAndSingle) {
  const auto empty = LinkedList<int>::Empty();
  EXPECT_TRUE(LinkedList<int>::IsEmpty(empty));
  EXPECT_EQ(LinkedList<int>::Length(empty), 0);

  const auto single = LinkedList<int>::Single(7);
  EXPECT_FALSE(LinkedList<int>::IsEmpty(single));
  EXPECT_TRUE(LinkedList<int>::IsSingle(single));
  EXPECT_EQ(LinkedList<int>::Head(single), 7);
  EXPECT_EQ(LinkedList<int>::Last(single), 7);
  EXPECT_EQ(LinkedList<int>::Length(single), 1);
  EXPECT_EQ(ToVector(single), std::vector({7}));
}

TEST(LinkedList, ConsAndIndex) {
  auto list = LinkedList<int>::Empty();
  list = LinkedList<int>::Cons(1, list);
  list = LinkedList<int>::Cons(2, list);
  list = LinkedList<int>::Cons(3, list);  // [3,2,1]
  EXPECT_EQ(ToVector(list), std::vector<int>({3, 2, 1}));
  EXPECT_EQ(LinkedList<int>::Head(list), 3);
  EXPECT_EQ(LinkedList<int>::Index(list, 0), 3);
  EXPECT_EQ(LinkedList<int>::Index(list, 2), 1);
  EXPECT_EQ(LinkedList<int>::Length(list), 3);
  EXPECT_FALSE(LinkedList<int>::IsSingle(list));
  EXPECT_FALSE(LinkedList<int>::IsEmpty(list));
}

TEST(LinkedList, TailAndAppend) {
  const auto list1 = LinkedList<int>::Cons(
      1, LinkedList<int>::Cons(
             2, LinkedList<int>::Cons(3, LinkedList<int>::Empty())));
  // l is [1,2,3]
  const auto tail = LinkedList<int>::Tail(list1);
  EXPECT_EQ(ToVector(tail), std::vector<int>({2, 3}));

  const auto list2 = LinkedList<int>::Cons(
      4, LinkedList<int>::Cons(5, LinkedList<int>::Empty()));
  const auto appended = LinkedList<int>::Append(list1, list2);
  EXPECT_EQ(ToVector(appended), std::vector<int>({1, 2, 3, 4, 5}));
  EXPECT_EQ(LinkedList<int>::Length(appended), 5);
  EXPECT_EQ(LinkedList<int>::Last(appended), 5);
}

TEST(LinkedList, SnocAndInit) {
  const auto base = LinkedList<int>::Cons(
      1, LinkedList<int>::Cons(2, LinkedList<int>::Empty()));  // [1,2]
  const auto snocd = LinkedList<int>::Snoc(base, 9);           // [1,2,9]
  EXPECT_EQ(ToVector(snocd), std::vector<int>({1, 2, 9}));
  EXPECT_EQ(LinkedList<int>::Last(snocd), 9);
  // original must remain unchanged
  EXPECT_EQ(ToVector(base), std::vector<int>({1, 2}));

  auto init = LinkedList<int>::Init(snocd);
  EXPECT_EQ(ToVector(init), std::vector<int>({1, 2}));
  // Init of single yields empty
  const auto single = LinkedList<int>::Single(42);
  const auto init_single = LinkedList<int>::Init(single);
  EXPECT_TRUE(LinkedList<int>::IsEmpty(init_single));
}

TEST(LinkedList, IndexOutOfRangeAndExceptions) {
  auto empty = LinkedList<int>::Empty();
  EXPECT_THROW(LinkedList<int>::Head(empty), std::runtime_error);
  EXPECT_THROW(LinkedList<int>::Last(empty), std::runtime_error);
  auto list = LinkedList<int>::Cons(1, LinkedList<int>::Empty());
  EXPECT_THROW(LinkedList<int>::Index(list, -1), std::out_of_range);
  EXPECT_THROW(LinkedList<int>::Index(empty, 0), std::out_of_range);
}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
