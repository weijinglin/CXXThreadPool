#include "x_thread.h"

using namespace MyThreadPool;

void TaskQueue::AddTask(std::function<void()> &&f) {
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
    for (uint64_t i = 0;i < num;++i) {
        // this->threads.push_back(std::thread(work,this));
    }
}

void ThreadPool::work() {
    // todo : enable the exiting ability of thread worker
    while (true)
    {
        /* code */
        std::function<void()> fn;
        {
            std::unique_lock<std::mutex> ulock{task_guard};        
            wake_signal.wait(ulock,[this]{return !this->isEmpty();});
            fn = this->task_queue.Pop();
        }
        fn();
    }
}

bool ThreadPool::isEmpty() {
    return task_queue.isEmpty();
}

template <class fn,class... Args>
auto ThreadPool::SubmitTask(fn&& f, Args&&... args) -> std::future<decltype(f(std::forward<Args>(args)...))> {
    auto task = std::bind(f,std::forward<Args>(args)...);
    std::promise<decltype(f(std::forward<Args>(args)...))> promise;
    std::function<void()> wrapper_fn = [task,&promise]() {
        auto res = task();
        promise.set_value(res);
    };
    this->task_queue.AddTask(wrapper_fn);
    return promise.get_future();
}
