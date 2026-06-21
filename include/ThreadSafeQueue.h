#pragma once

#include <queue>
#include <mutex>
#include <condition_variable>

// OS Concept: Thread-Safe Data Structures
// A standard std::queue is not thread-safe. If two threads try to push/pop at the same time,
// the program will crash (Segmentation Fault). We use a Mutex to "lock" the queue.
template <typename T>
class ThreadSafeQueue {
private:
    std::queue<T> queue;
    std::mutex mtx;
    std::condition_variable condVar;

public:
    ThreadSafeQueue() = default;

    // Push an item onto the queue safely
    void push(T item) {
        // Lock the mutex before modifying the queue
        std::unique_lock<std::mutex> lock(mtx);
        queue.push(item);
        lock.unlock();
        
        // OS Concept: Condition Variable
        // Notify one waiting thread that data is now available
        condVar.notify_one();
    }

    // Pop an item from the queue safely. This will block (wait) if the queue is empty.
    T pop() {
        std::unique_lock<std::mutex> lock(mtx);
        
        // Wait until the queue is NOT empty. 
        // This prevents the CPU from burning 100% in a while-loop.
        condVar.wait(lock, [this]() { return !queue.empty(); });
        
        T item = queue.front();
        queue.pop();
        return item;
    }

    bool empty() {
        std::unique_lock<std::mutex> lock(mtx);
        return queue.empty();
    }
};
