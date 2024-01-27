#pragma once

#include <atomic>
#include <condition_variable>
#include <functional>
#include <future>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

namespace MyThreadPool {
class TaskQueue {
  std::queue<std::function<void()>> task_queue;
  std::mutex queue_guard;

 public:
  TaskQueue() = default;
  void AddTask(std::function<void()>& f);
  auto Pop() -> std::function<void()>;
  bool isEmpty() { return task_queue.empty(); }
};

class ThreadPool {
  TaskQueue task_queue;
  std::vector<std::thread> threads;
  std::condition_variable wake_signal;
  std::mutex task_guard;
  std::atomic_bool is_terminated;

 private:
  void work();

 public:
  // a function which is not thread-safe
  bool isEmpty();

  explicit ThreadPool(uint64_t num);

  ThreadPool() = delete;

  ~ThreadPool();

  template <class fn, class... Args>
  auto SubmitTask(fn&& f, Args&&... args)
      -> std::promise<decltype(f(std::forward<Args>(args)...))>;
};

template <class fn, class... Args>
auto ThreadPool::SubmitTask(fn&& f, Args&&... args)
    -> std::promise<decltype(f(std::forward<Args>(args)...))> {
  auto task = std::bind(f, std::forward<Args>(args)...);
  std::promise<decltype(f(std::forward<Args>(args)...))> promise;
  std::function<void()> wrapper_fn = [task, &promise]() {
    auto res = task();
    promise.set_value(res);
  };
  this->task_queue.AddTask(wrapper_fn);
  wake_signal.notify_all();
  return promise;
}
}  // namespace MyThreadPool
