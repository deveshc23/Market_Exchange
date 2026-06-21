#pragma once

#include "Trade.h"
#include "ThreadSafeQueue.h"
#include "../lib/sqlite3.h"
#include <thread>
#include <atomic>

class DatabaseLogger {
private:
    ThreadSafeQueue<Trade>& tradeQueue;
    std::atomic<bool> isRunning;
    std::thread loggerThread;
    sqlite3* db;

    void run();
    void initDatabase();

public:
    DatabaseLogger(ThreadSafeQueue<Trade>& queue);
    ~DatabaseLogger();

    void start();
    void stop();
};
