#include "x_thread.h"

using namespace MyThreadPool;

void TaskQueue::AddTask(std::function<void()>& f) {
  {
    std::lock_guard guard(queue_guard);
    this->task_queue.emplace(std::move(f));
  }
}

auto TaskQueue::Pop() -> std::function<void()> {
  {
    std::lock_guard guard(queue_guard);
    auto res = this->task_queue.front();
    this->task_queue.pop();
    return res;
  }
}

ThreadPool::ThreadPool(uint64_t num) {
  is_terminated.store(false);
  for (uint64_t i = 0; i < num; ++i) {
    this->threads.emplace_back(std::thread(&ThreadPool::work, this));
  }
}

void ThreadPool::work() {
  // todo : enable the exiting ability of thread worker
  while (true) {
    /* code */
    std::function<void()> fn;
    {
      std::unique_lock<std::mutex> ulock{task_guard};
      wake_signal.wait(ulock, [this] {
        return this->is_terminated.load() || !this->isEmpty();
      });
      if (this->is_terminated.load()) {
        break;
      }
      if (this->isEmpty()) {
        continue;
      }
      fn = this->task_queue.Pop();
    }
    fn();
  }
}

ThreadPool::~ThreadPool() {
  is_terminated.store(true);
  wake_signal.notify_all();
  std::for_each(threads.begin(), threads.end(),
                [](std::thread& t) { t.join(); });
}

bool ThreadPool::isEmpty() { return task_queue.isEmpty(); }