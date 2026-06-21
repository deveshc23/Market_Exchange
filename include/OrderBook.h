#pragma once

#include <map>
#include <queue>
#include <vector>
#include "Order.h"
#include "Trade.h"

// OOP Concept: Abstraction
// The complex logic of matching orders and maintaining sorted price levels
// is hidden behind a simple public interface: addOrder().
class OrderBook {
private:
    // Bids (Buy orders) - Sorted descending (highest price first)
    std::map<double, std::queue<Order>, std::greater<double>> bids;

    // Asks (Sell orders) - Sorted ascending (lowest price first)
    std::map<double, std::queue<Order>, std::less<double>> asks;

    // Helper method to generate unique trade IDs
    uint64_t nextTradeId = 1;

    // Internal matching logic hidden from the user
    void matchOrders(Order& incomingOrder, std::vector<Trade>& executedTrades);

public:
    OrderBook() = default;

    // The only method the outside world needs to interact with
    std::vector<Trade> addOrder(Order order);
};
