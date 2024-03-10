#include <gtest/gtest.h>

#include "../src/x_thread.h"

int add(int left, int right) { return left + right; }

void void_func() { return; }

class ThreadPoolTest : public testing::Test {
 protected:
  void SetUp() override {
    pool = std::make_shared<MyThreadPool::ThreadPool>(6);
  }

  std::shared_ptr<MyThreadPool::ThreadPool> pool;
};

// Demonstrate usage of function with return value
TEST_F(ThreadPoolTest, SimpleFuncWithRetVal) {
  auto promise = pool->SubmitTask(add, 1, 2);
  ASSERT_EQ(promise.get_future().get(), 3);
}

// Demonstrate usage of function without return value
TEST_F(ThreadPoolTest, SimpleFuncWithOutRetVal) {
  auto promise = pool->SubmitTask(void_func);
  ASSERT_EQ(promise.get_future().get(), true);
}

// concurrent testing
TEST_F(ThreadPoolTest, ConcurrencyWithRetTest) {
  int concurrency = 10;
  std::vector<int> src;
  for (int i = 0; i < concurrency; ++i) {
    src.push_back(i + 1);
  }
  std::vector<int> res;
  for (auto data : src) {
    res.push_back(pool->SubmitTask(add, data, data).get_future().get());
  }
  ASSERT_EQ(res.size(), concurrency);
  for (int i = 0; i < concurrency; ++i) {
    ASSERT_EQ(res.at(i), src.at(i) * 2);
  }
}

// concurrent testing
TEST_F(ThreadPoolTest, ConcurrencyWithoutRetTest) {
  int concurrency = 10;
  std::vector<bool> res;
  for (int i = 0; i < concurrency; ++i) {
    res.push_back(pool->SubmitTask(void_func).get_future().get());
  }
  ASSERT_EQ(res.size(), concurrency);
  for (int i = 0; i < concurrency; ++i) {
    ASSERT_TRUE(res.at(i));
  }
}