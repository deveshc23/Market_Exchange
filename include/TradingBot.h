#pragma once

#include "Order.h"
#include "ThreadSafeQueue.h"
#include <thread>
#include <atomic>

class TradingBot {
private:
    uint64_t botClientId;
    ThreadSafeQueue<Order>& orderQueue;
    std::atomic<bool> isRunning;
    std::thread botThread;

    void run();

public:
    TradingBot(uint64_t clientId, ThreadSafeQueue<Order>& queue);
    ~TradingBot();

    void start();
    void stop();
};
