#include <gtest/gtest.h>

#include <vector>

#include "deque/Deque.h"

// Helper to convert a Deque<int> to std::vector<int> via ToList().
static std::vector<int> to_vector(const Deque<int>& deque) {
  std::vector<int> out;
  auto list = deque.ToList();
  while (!list.IsEmpty()) {
    out.push_back(list.Head());
    list = list.Tail();
  }
  return out;
}

TEST(DequeTest, EmptyAndSingle) {
  const auto empty = Deque<int>::Empty();
  EXPECT_TRUE(empty.IsEmpty());
  EXPECT_EQ(empty.Length(), 0);

  const auto single = Deque<int>::Single(7);
  EXPECT_FALSE(single.IsEmpty());
  EXPECT_TRUE(single.IsSingle());
  EXPECT_EQ(single.Head(), 7);
  EXPECT_EQ(single.Last(), 7);
  EXPECT_EQ(single.Length(), 1);
  EXPECT_EQ(to_vector(single), std::vector<int>({7}));
}

TEST(DequeTest, ConsAndIndex) {
  auto list = Deque<int>::Empty();
  list = list.Cons(1);
  list = list.Cons(2);
  list = list.Cons(3);  // prepends
  EXPECT_EQ(to_vector(list), std::vector({3, 2, 1}));
  EXPECT_EQ(list.Head(), 3);
  EXPECT_EQ(list.Index(0), 3);
  EXPECT_EQ(list.Index(2), 1);
  EXPECT_EQ(list.Length(), 3);
  EXPECT_FALSE(list.IsSingle());
  EXPECT_FALSE(list.IsEmpty());
}

TEST(DequeTest, TailAndAppend) {
  auto list1 = Deque<int>::Empty();
  list1 = list1.Cons(3).Cons(2).Cons(1);  // build
  const auto tail = list1.Tail();
  EXPECT_EQ(to_vector(tail), std::vector({2, 3}));

  auto list2 = Deque<int>::Empty();
  constexpr int kFour = 4;
  constexpr int kFive = 5;
  list2 = list2.Cons(kFive).Cons(kFour);  // [4,5]
  const auto appended = list1.Append(list2);
  EXPECT_EQ(to_vector(appended), std::vector({1, 2, 3, kFour, kFive}));
  EXPECT_EQ(appended.Length(), kFive);
  EXPECT_EQ(appended.Last(), kFive);
}

TEST(DequeTest, SnocAndInit) {
  auto base = Deque<int>::Empty();
  base = base.Cons(2).Cons(1);      // [1,2]
  const auto snocd = base.Snoc(9);  // [1,2,9]
  EXPECT_EQ(to_vector(snocd), std::vector<int>({1, 2, 9}));
  EXPECT_EQ(snocd.Last(), 9);
  // original must remain unchanged
  EXPECT_EQ(to_vector(base), std::vector<int>({1, 2}));

  const auto init = snocd.Init();
  EXPECT_EQ(to_vector(init), std::vector<int>({1, 2}));
  // Init of single yields empty
  const auto single = Deque<int>::Single(42);
  const auto init_single = single.Init();
  EXPECT_TRUE(init_single.IsEmpty());
}

TEST(DequeTest, IndexOutOfRangeAndExceptions) {
  const auto empty = Deque<int>::Empty();
  EXPECT_THROW(empty.Head(), std::invalid_argument);
  EXPECT_THROW(empty.Last(), std::invalid_argument);
  const auto list = empty.Cons(1);
  EXPECT_THROW(list.Index(-1), std::out_of_range);
  EXPECT_THROW(empty.Index(0), std::out_of_range);
}

TEST(DequeTest, CopyAndAssignment) {
  auto original = Deque<int>::Empty();
  original = original.Cons(3).Cons(2).Cons(1);  // [1,2,3]

  // copy construction
  Deque<int> copy_constructed = original;
  EXPECT_EQ(to_vector(copy_constructed), to_vector(original));

  // assignment
  Deque<int> assigned = Deque<int>::Empty();
  assigned = original;
  EXPECT_EQ(to_vector(assigned), to_vector(original));
}

TEST(DequeTest, StructuralSharingImmutability) {
  auto base = Deque<int>::Empty().Cons(2).Cons(1);  // [1,2]
  auto extended = base.Cons(0);                     // [0,1,2]

  // base must remain unchanged
  EXPECT_EQ(to_vector(base), std::vector<int>({1, 2}));
  EXPECT_EQ(to_vector(extended), std::vector<int>({0, 1, 2}));
}

TEST(DequeTest, IndexBounds) {
  auto list = Deque<int>::Empty().Cons(3).Cons(2).Cons(1);  // [1,2,3]
  EXPECT_THROW(list.Index(3), std::out_of_range);
  EXPECT_THROW(list.Index(100), std::out_of_range);
}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
