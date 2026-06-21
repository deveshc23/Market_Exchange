#include "MarketDataPublisher.h"
#include <chrono>
#include <random>
#include <iostream>
#include <cmath>

MarketDataPublisher::MarketDataPublisher(ThreadSafeQueue<Order>& queue)
    : orderQueue(queue), isRunning(false) {}

MarketDataPublisher::~MarketDataPublisher() { stop(); }

void MarketDataPublisher::start() {
    isRunning = true;
    publisherThread = std::thread(&MarketDataPublisher::run, this);
}

void MarketDataPublisher::stop() {
    isRunning = false;
    if (publisherThread.joinable()) publisherThread.join();
}

void MarketDataPublisher::run() {
    std::random_device rd;
    std::mt19937 gen(rd()); 
    std::uniform_int_distribution<> sideDist(0, 1);
    std::uniform_real_distribution<> priceDist(140.0, 160.0);
    std::uniform_int_distribution<> qtyDist(1, 10);
    std::uniform_int_distribution<> clientDist(1, 5); // Simulating 5 different market participants
    
    uint64_t currentOrderId = 10000;
    
    while (isRunning) {
        std::this_thread::sleep_for(std::chrono::milliseconds(500)); // Slower ticks so you can watch

        OrderSide side = sideDist(gen) == 0 ? OrderSide::BUY : OrderSide::SELL;
        double price = std::round(priceDist(gen) * 100.0) / 100.0;
        uint32_t qty = qtyDist(gen) * 10;
        uint64_t timestamp = std::chrono::system_clock::now().time_since_epoch().count();
        uint64_t clientId = clientDist(gen);

        Order newOrder(currentOrderId++, clientId, "AAPL", side, price, qty, timestamp);
        orderQueue.push(newOrder);
    }
}
