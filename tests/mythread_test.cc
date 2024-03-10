#include <gtest/gtest.h>
#include "../src/x_thread.h"

int add(int left,int right) {
    return left + right;
}

void void_func() {
    return;
}

class ThreadPoolTest : public testing::Test {
    protected:
    void SetUp() override {
        pool = std::make_shared<MyThreadPool::ThreadPool>(6);
    }

    std::shared_ptr<MyThreadPool::ThreadPool> pool;
};

// Demonstrate usage of function with return value
TEST_F(ThreadPoolTest, SimpleFuncWithRetVal) {
    auto promise = pool->SubmitTask(add,1,2);
    ASSERT_EQ(promise.get_future().get(),3);
}

// Demonstrate usage of function without return value
TEST_F(ThreadPoolTest, SimpleFuncWithOutRetVal) {
    auto promise = pool->SubmitTask(void_func);
    ASSERT_EQ(promise.get_future().get(),true);
}

// concurrent testing
TEST_F(ThreadPoolTest, ConcurrencyTest) {

}