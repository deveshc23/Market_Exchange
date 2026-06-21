#pragma once

#include "Order.h"
#include "ThreadSafeQueue.h"
#include <thread>
#include <atomic>

// OOP Concept: Active Object / Producer
// This class manages its own background thread.
class MarketDataPublisher {
private:
    ThreadSafeQueue<Order>& orderQueue; // Reference to the shared queue
    std::atomic<bool> isRunning;        // Thread-safe boolean to stop the thread gracefully
    std::thread publisherThread;        // The background thread

    void run(); // The internal loop the thread executes

public:
    // We pass the shared queue via the constructor (Dependency Injection)
    MarketDataPublisher(ThreadSafeQueue<Order>& queue);
    ~MarketDataPublisher();

    void start();
    void stop();
};
