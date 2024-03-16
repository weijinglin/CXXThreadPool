# CXXThreadPool
A ThreadPool written in C++.

# Configuration
```
// install dependency
apt install googletest libgtest-dev
```

# Usage
```C++
// construct a ThreadPool
// first parameter : thread number (default get from std::thread::hardware_concurrency) 
MyThreadPool::ThreadPool thread_pool(8);

// SubmitTask will return a promise object
auto promise = thread_pool.SubmitTask(func,arg1,arg2,...);

// use std::promise object to fetch result
auto result = promise.get_future().get();
```

# Attention
for functions that return void as result type. We will return the std::promise\<bool>. 