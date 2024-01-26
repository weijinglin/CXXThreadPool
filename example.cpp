#include <iostream>
#include "src/x_thread.h"

int hello(std::string name) {
    std::cout << "hello " << name << std::endl;
    return 100; 
}

template<class fn, class... Args>
void printType(fn&& f, Args&&... args) {
    std::cout << "future type is " << typeid(decltype(f(std::forward<Args>(args)...))).name() << std::endl;
}

int main(int argc, char* argv[]) {
    std::cout << "hello world" << std::endl;
    std::string name = "world";
    MyThreadPool::ThreadPool thread_pool(8);
    printType(hello,name);
    auto future = thread_pool.SubmitTask(hello,name);
    std::cout << "get return value is " << future.get() << std::endl;
    return 0;
}