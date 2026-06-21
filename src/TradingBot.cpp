#include "TradingBot.h"
#include <chrono>
#include <random>
#include <iostream>
#include <cmath>

TradingBot::TradingBot(uint64_t clientId, ThreadSafeQueue<Order>& queue)
    : botClientId(clientId), orderQueue(queue), isRunning(false) {}

TradingBot::~TradingBot() { stop(); }

void TradingBot::start() {
    isRunning = true;
    botThread = std::thread(&TradingBot::run, this);
}

void TradingBot::stop() {
    isRunning = false;
    if (botThread.joinable()) botThread.join();
}

void TradingBot::run() {
    std::random_device rd;
    std::mt19937 gen(rd());
    
    // Bot acts as a Market Maker - mostly placing resting orders near mid-price
    std::uniform_real_distribution<> priceDist(149.0, 151.0); 
    
    uint64_t botOrderId = 900000; // Bot uses 900k+ series for easy tracking
    
    while (isRunning) {
        std::this_thread::sleep_for(std::chrono::milliseconds(800)); // Slower algo ticks

        double bidPrice = std::round((priceDist(gen) - 0.5) * 100.0) / 100.0;
        double askPrice = std::round((priceDist(gen) + 0.5) * 100.0) / 100.0;
        
        uint64_t timestamp = std::chrono::system_clock::now().time_since_epoch().count();

        // Place a BID
        Order bidOrder(botOrderId++, botClientId, "AAPL", OrderSide::BUY, bidPrice, 50, timestamp);
        orderQueue.push(bidOrder);

        // Place an ASK
        Order askOrder(botOrderId++, botClientId, "AAPL", OrderSide::SELL, askPrice, 50, timestamp);
        orderQueue.push(askOrder);
    }
}
