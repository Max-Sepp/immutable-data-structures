#include <gtest/gtest.h>

#include <vector>

#include "linkedlist/LinkedList.h"

// Helper to convert a LinkedList<int> to std::vector<int> for comparisons.
static std::vector<int> to_vector(LinkedList<int> list) {
  std::vector<int> out;
  while (!list.IsEmpty()) {
    out.push_back(list.Head());
    list = list.Tail();
  }
  return out;
}

TEST(LinkedListTest, EmptyAndSingle) {
  const auto empty = LinkedList<int>::Empty();
  EXPECT_TRUE(empty.IsEmpty());
  EXPECT_EQ(empty.Length(), 0);

  const auto single = LinkedList<int>::Single(7);
  EXPECT_FALSE(single.IsEmpty());
  EXPECT_TRUE(single.IsSingle());
  EXPECT_EQ(single.Head(), 7);
  EXPECT_EQ(single.Last(), 7);
  EXPECT_EQ(single.Length(), 1);
  EXPECT_EQ(to_vector(single), std::vector<int>({7}));
}

TEST(LinkedListTest, ConsAndIndex) {
  auto list = LinkedList<int>::Empty();
  list = list.Cons(1);
  list = list.Cons(2);
  list = list.Cons(3);  // [3,2,1]
  EXPECT_EQ(to_vector(list), std::vector<int>({3, 2, 1}));
  EXPECT_EQ(list.Head(), 3);
  EXPECT_EQ(list.Index(0), 3);
  EXPECT_EQ(list.Index(2), 1);
  EXPECT_EQ(list.Length(), 3);
  EXPECT_FALSE(list.IsSingle());
  EXPECT_FALSE(list.IsEmpty());
}

TEST(LinkedListTest, InfixConsAndIndex) {
  auto list = LinkedList<int>::Empty();
  list = 1 += list;
  list = 2 += list;
  list = 3 += list;  // [3,2,1]
  EXPECT_EQ(to_vector(list), std::vector<int>({3, 2, 1}));
  EXPECT_EQ(list.Head(), 3);
  EXPECT_EQ(list.Index(0), 3);
  EXPECT_EQ(list.Index(2), 1);
  EXPECT_EQ(list.Length(), 3);
  EXPECT_FALSE(list.IsSingle());
  EXPECT_FALSE(list.IsEmpty());
}

TEST(LinkedListTest, TailAndAppend) {
  auto list1 = LinkedList<int>::Empty();
  list1 = list1.Cons(3).Cons(2).Cons(1);  // [1,2,3]
  const auto tail = list1.Tail();
  EXPECT_EQ(to_vector(tail), std::vector<int>({2, 3}));

  auto list2 = LinkedList<int>::Empty();
  constexpr int kFour = 4;
  constexpr int kFive = 5;
  list2 = list2.Cons(kFive).Cons(kFour);  // [4,5]
  const auto appended = list1.Append(list2);
  EXPECT_EQ(to_vector(appended), std::vector<int>({1, 2, 3, kFour, kFive}));
  EXPECT_EQ(appended.Length(), kFive);
  EXPECT_EQ(appended.Last(), kFive);
}

TEST(LinkedListTest, SnocAndInit) {
  auto base = LinkedList<int>::Empty();
  base = base.Cons(2).Cons(1);      // [1,2]
  const auto snocd = base.Snoc(9);  // [1,2,9]
  EXPECT_EQ(to_vector(snocd), std::vector<int>({1, 2, 9}));
  EXPECT_EQ(snocd.Last(), 9);
  // original must remain unchanged
  EXPECT_EQ(to_vector(base), std::vector<int>({1, 2}));

  const auto init = snocd.Init();
  EXPECT_EQ(to_vector(init), std::vector<int>({1, 2}));
  // Init of single yields empty
  const auto single = LinkedList<int>::Single(42);
  const auto init_single = single.Init();
  EXPECT_TRUE(init_single.IsEmpty());
}

TEST(LinkedListTest, IndexOutOfRangeAndExceptions) {
  const auto empty = LinkedList<int>::Empty();
  EXPECT_THROW((void)empty.Head(), std::runtime_error);
  EXPECT_THROW((void)empty.Last(), std::runtime_error);
  const auto list = empty.Cons(1);
  EXPECT_THROW((void)list.Index(-1), std::out_of_range);
  EXPECT_THROW((void)empty.Index(0), std::out_of_range);
}

TEST(LinkedListTest, CopyAndAssignment) {
  auto original = LinkedList<int>::Empty();
  original = original.Cons(3).Cons(2).Cons(1);  // [1,2,3]

  // copy construction
  LinkedList<int> copy_constructed = original;
  EXPECT_EQ(to_vector(copy_constructed), to_vector(original));

  // assignment
  LinkedList<int> assigned = LinkedList<int>::Empty();
  assigned = original;
  EXPECT_EQ(to_vector(assigned), to_vector(original));
}

TEST(LinkedListTest, StructuralSharingImmutability) {
  auto base = LinkedList<int>::Empty().Cons(2).Cons(1);  // [1,2]
  auto extended = base.Cons(0);                          // [0,1,2]

  // base must remain unchanged
  EXPECT_EQ(to_vector(base), std::vector<int>({1, 2}));
  EXPECT_EQ(to_vector(extended), std::vector<int>({0, 1, 2}));
}

TEST(LinkedListTest, IndexBounds) {
  auto list = LinkedList<int>::Empty().Cons(3).Cons(2).Cons(1);  // [1,2,3]
  EXPECT_THROW((void)list.Index(3), std::out_of_range);
  EXPECT_THROW((void)list.Index(100), std::out_of_range);
}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
