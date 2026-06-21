#include "OrderBook.h"
#include <algorithm> // for std::min

void OrderBook::matchOrders(Order& incomingOrder, std::vector<Trade>& executedTrades) {
    if (incomingOrder.getSide() == OrderSide::BUY) {
        while (incomingOrder.getQuantity() > 0 && !asks.empty()) {
            auto bestAsk = asks.begin();
            double askPrice = bestAsk->first;

            if (incomingOrder.getPrice() < askPrice) break;

            std::queue<Order>& askQueue = bestAsk->second;
            while (incomingOrder.getQuantity() > 0 && !askQueue.empty()) {
                Order& restingAsk = askQueue.front();
                uint32_t fillQuantity = std::min(incomingOrder.getQuantity(), restingAsk.getQuantity());
                
                executedTrades.emplace_back(
                    nextTradeId++, 
                    incomingOrder.getOrderId(), 
                    restingAsk.getOrderId(),
                    incomingOrder.getClientId(),
                    restingAsk.getClientId(),
                    restingAsk.getTicker(), 
                    askPrice, 
                    fillQuantity, 
                    incomingOrder.getTimestamp()
                );

                incomingOrder.fill(fillQuantity);
                restingAsk.fill(fillQuantity);

                if (restingAsk.getQuantity() == 0) askQueue.pop();
            }
            if (askQueue.empty()) asks.erase(bestAsk);
        }
    } else {
        while (incomingOrder.getQuantity() > 0 && !bids.empty()) {
            auto bestBid = bids.begin();
            double bidPrice = bestBid->first;

            if (incomingOrder.getPrice() > bidPrice) break;

            std::queue<Order>& bidQueue = bestBid->second;
            while (incomingOrder.getQuantity() > 0 && !bidQueue.empty()) {
                Order& restingBid = bidQueue.front();
                uint32_t fillQuantity = std::min(incomingOrder.getQuantity(), restingBid.getQuantity());
                
                executedTrades.emplace_back(
                    nextTradeId++, 
                    restingBid.getOrderId(), 
                    incomingOrder.getOrderId(), 
                    restingBid.getClientId(),
                    incomingOrder.getClientId(),
                    restingBid.getTicker(), 
                    bidPrice, 
                    fillQuantity, 
                    incomingOrder.getTimestamp()
                );

                incomingOrder.fill(fillQuantity);
                restingBid.fill(fillQuantity);

                if (restingBid.getQuantity() == 0) bidQueue.pop();
            }
            if (bidQueue.empty()) bids.erase(bestBid);
        }
    }
}

std::vector<Trade> OrderBook::addOrder(Order order) {
    std::vector<Trade> executedTrades;
    matchOrders(order, executedTrades);

    if (order.getQuantity() > 0) {
        if (order.getSide() == OrderSide::BUY) {
            bids[order.getPrice()].push(order);
        } else {
            asks[order.getPrice()].push(order);
        }
    }
    return executedTrades;
}
