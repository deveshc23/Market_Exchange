#pragma once

#include <string>
#include <cstdint>

enum class OrderSide {
    BUY,
    SELL
};

class Order {
private:
    uint64_t orderId;
    uint64_t clientId;
    std::string ticker;
    OrderSide side;
    double price;
    uint32_t quantity;
    uint64_t timestamp;

public:
    Order(uint64_t id, uint64_t cId, std::string t, OrderSide s, double p, uint32_t q, uint64_t ts);

    uint64_t getOrderId() const;
    uint64_t getClientId() const;
    std::string getTicker() const;
    OrderSide getSide() const;
    double getPrice() const;
    uint32_t getQuantity() const;
    uint64_t getTimestamp() const;

    void fill(uint32_t filledQuantity);
};
