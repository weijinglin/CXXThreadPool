#include <iostream>

#include "src/x_thread.h"

int hello(std::string name) {
  std::cout << "hello " << name << std::endl;
  return 100;
}

void hello_not_value_return(std::string name) {
  std::cout << "hello " << name << std::endl;
  // return 100;
}

template <class fn, class... Args>
void printType(fn&& f, Args&&... args) {
  std::cout << "future type is "
            << typeid(decltype(f(std::forward<Args>(args)...))).name()
            << std::endl;
}

template <class fn, class... Args>
void printFunctionType(fn f) {
  std::cout << "function type is " << typeid(fn).name() << std::endl;
  std::cout << "expect   type is " << typeid(void(*)(Args...)).name() << std::endl;
}

int main(int argc, char* argv[]) {
  std::cout << "hello world" << std::endl;
  std::string name = "world";
  MyThreadPool::ThreadPool thread_pool(8);
  printType(hello_not_value_return, name);
  printFunctionType(hello_not_value_return);
  auto promise = thread_pool.SubmitTask(hello, name);
  std::cout << "get return value is " << promise.get_future().get()
            << std::endl;
  auto promise_not_val = thread_pool.SubmitTask(hello_not_value_return,name);
  std::cout << "get return value is " << promise_not_val.get_future().get()
            << std::endl;
  return 0;
}