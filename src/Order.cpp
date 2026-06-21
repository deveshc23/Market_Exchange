#include "Order.h"
#include <stdexcept>

Order::Order(uint64_t id, uint64_t cId, std::string t, OrderSide s, double p, uint32_t q, uint64_t ts)
    : orderId(id), clientId(cId), ticker(t), side(s), price(p), quantity(q), timestamp(ts) {
    
    if (price <= 0) {
        throw std::invalid_argument("Price must be greater than 0");
    }
    if (quantity == 0) {
        throw std::invalid_argument("Quantity must be greater than 0");
    }
}

uint64_t Order::getOrderId() const { return orderId; }
uint64_t Order::getClientId() const { return clientId; }
std::string Order::getTicker() const { return ticker; }
OrderSide Order::getSide() const { return side; }
double Order::getPrice() const { return price; }
uint32_t Order::getQuantity() const { return quantity; }
uint64_t Order::getTimestamp() const { return timestamp; }

void Order::fill(uint32_t filledQuantity) {
    if (filledQuantity > quantity) {
        throw std::invalid_argument("Cannot fill more than the remaining quantity.");
    }
    quantity -= filledQuantity;
}
