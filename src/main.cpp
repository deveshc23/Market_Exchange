#include <iostream>
#include <vector>
#include <chrono>
#include "OrderBook.h"
#include "ThreadSafeQueue.h"
#include "MarketDataPublisher.h"
#include "TradingBot.h"
#include "DatabaseLogger.h"
#include "PortfolioManager.h"

int main() {
    std::cout << "=== HIGH-FREQUENCY TRADING EXCHANGE ENGINE ===\n";
    std::cout << "Starting 4-Thread Architecture (Producer-Consumer-Logger)\n\n";

    // 1. Shared Queues
    ThreadSafeQueue<Order> exchangeQueue;
    ThreadSafeQueue<Trade> databaseQueue;

    // 2. Core Modules
    OrderBook book;
    PortfolioManager pm(999); // Tracking our TradingBot (Client ID 999)

    // 3. Start Background Threads
    MarketDataPublisher noisePublisher(exchangeQueue);
    TradingBot algoBot(999, exchangeQueue);
    DatabaseLogger dbLogger(databaseQueue);

    std::cout << "[SYSTEM] Booting up background threads...\n";
    dbLogger.start();
    noisePublisher.start();
    algoBot.start();

    // 4. Main Exchange Loop
    int ordersProcessed = 0;
    double currentMarketPrice = 150.00; // Track roughly for MTM PnL

    while (ordersProcessed < 100) { 
        Order incomingOrder = exchangeQueue.pop(); // Blocks safely until data is available
        ordersProcessed++;
        
        currentMarketPrice = incomingOrder.getPrice();

        std::string sideStr = (incomingOrder.getSide() == OrderSide::BUY) ? "BUY " : "SELL";
        std::cout << "[EXCHANGE] Received Order from Client " << incomingOrder.getClientId() 
                  << " | " << sideStr << " " << incomingOrder.getQuantity() 
                  << " @ $" << incomingOrder.getPrice() << "\n";

        // Execute Matches
        auto trades = book.addOrder(incomingOrder);

        // Process Trades (Async Logging & Risk Management)
        for (const auto& trade : trades) {
            std::cout << "  -> \033[1;32m[TRADE EXECUTED]\033[0m Price:$" << trade.getPrice() << " | Qty:" << trade.getQuantity() << "\n";
            
            databaseQueue.push(trade); // Send to background database thread (Non-blocking!)
            pm.onTradeExecuted(trade); // Calculate risk
        }

        // Periodically print the portfolio
        if (ordersProcessed % 5 == 0) {
            pm.printPortfolio(currentMarketPrice);
        }
    }

    std::cout << "\n[SYSTEM] Simulation finished. Initiating safe shutdown sequence...\n";
    
    noisePublisher.stop();
    algoBot.stop();
    dbLogger.stop();

    std::cout << "[SYSTEM] All threads terminated safely.\n";
    std::cout << "=== ENGINE OFFLINE ===\n";

    std::cout << "\nPress ENTER to exit the simulation...";
    std::cin.get();

    return 0;
}
