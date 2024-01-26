#pragma once

#include <queue>
#include <mutex>
#include <functional>
#include <future>
#include <vector>
#include <thread>
#include <condition_variable>

namespace MyThreadPool {
    class TaskQueue {
        std::queue<std::function<void()>> task_queue;
        std::mutex queue_guard;
    public:
        TaskQueue() = default;
        void AddTask(std::function<void()> &&f);
        auto Pop() -> std::function<void()>;
        bool isEmpty() {return task_queue.empty();}
    };

    class ThreadPool {
        TaskQueue task_queue;
        std::vector<std::thread> threads;
        std::condition_variable wake_signal;
        std::mutex task_guard;
    private:
        void work();
    public: 
        // a function which is not thread-safe
        bool isEmpty();

        explicit ThreadPool(uint64_t num);

        ThreadPool() = delete;

        template <class fn,class... Args>
        auto SubmitTask(fn&& f, Args&&... args) -> std::future<decltype(f(std::forward<Args>(args)...))>;
    };
}
