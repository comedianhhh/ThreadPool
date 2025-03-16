# ThreadPool
This project implements a simple **ThreadPool** using **C++17** features. The thread pool allows for multi-threaded task execution with thread safety and asynchronous operations. It uses RAII to manage the lifecycle of the pool and employs perfect forwarding to obtain function signatures for tasks.

## Features

- **Multi-thread Safe Task Queue**: Ensures safe access to the task queue with a locking mechanism.
- **Asynchronous Operations**: Task submissions (using `submit()`) are asynchronous, similar to `std::thread`.
- **Perfect Forwarding**: Tasks are wrapped using lambdas and `std::function`, and the thread pool utilizes perfect forwarding to pass callable objects.
- **RAII for ThreadPool Lifecycle**: The thread pool is automatically cleaned up when it goes out of scope, ensuring resource management.
- **Shared Locks**: The task queue is protected with a shared mutex to allow concurrent read access.

## Example Usage

Here is an example demonstrating how to use the thread pool to submit tasks:

```cpp
#include "ThreadPool.h"

int main() {
    ThreadPool pool(8); // Create a thread pool with 8 threads
    int n = 20;

    for (int i = 1; i <= n; i++) {
        pool.submit([](int id) {
            if (id % 2 == 1) {
                this_thread::sleep_for(0.2s); // Simulate work
            }
            unique_lock<mutex> lc(_m);
            cout << "id : " << id << endl; // Output the task ID
        }, i);
    }

    return 0;
}
```
![image](https://github.com/user-attachments/assets/9eefad7c-e7b2-4f13-9dcd-f0793cd26fb2)
This code will submit 20 tasks to the thread pool. Each task prints an ID, and tasks with odd IDs will simulate a delay.

## Design Overview
The design of the thread pool incorporates several modern C++ concepts:

C++17 Shared Locks: The task queue is protected by a shared mutex (```std::shared_mutex```) to handle concurrent access.

More info: [std::shared_mutex - cppreference.com](https://zh.cppreference.com/w/cpp/thread/shared_mutex).

Asynchronous Task Submission: The pool utilizes ```std::packaged_task``` to perform asynchronous operations, ensuring tasks are executed without blocking the main thread.

Perfect Forwarding: The pool uses perfect forwarding to pass tasks with their original signatures, ensuring that lambdas and ```std::function``` are handled correctly without unnecessary copies.

RAII for Lifecycle Management: The thread pool is managed through RAII, ensuring threads are automatically joined or detached when the pool goes out of scope.

## Example of Asynchronous Operation
This example demonstrates the usage of ```std::packaged_task``` and asynchronous execution in the context of the thread pool:
```cpp
packaged_task<int(int, int)> func([](int a, int b) { return a + b; });
auto ret = func.get_future();

thread t1{ [&]() {
    { unique_lock<mutex> lc(_m); cout << "======1=======\n"; }
    this_thread::sleep_for(2s); // Simulate delay
    func(3, 5); // Execute task
    { unique_lock<mutex> lc(_m); cout << "======1=======\n"; }
} };

thread t2{ [&]() {
    { unique_lock<mutex> lc(_m); cout << "======2=======\n"; }
    cout << ret.get() << "\n"; // Get the result from the packaged task
    { unique_lock<mutex> lc(_m); cout << "======2=======\n"; }
} };

t1.join();
t2.join();

```


## License
This project is licensed under the MIT License - see the LICENSE file for details.
