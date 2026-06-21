#pragma once

#include <string>
#include <cstdint>

class Trade {
private:
    uint64_t tradeId;
    uint64_t buyerOrderId;
    uint64_t sellerOrderId;
    uint64_t buyerId;
    uint64_t sellerId;
    std::string ticker;
    double price;
    uint32_t quantity;
    uint64_t timestamp;

public:
    Trade(uint64_t tId, uint64_t bOrderId, uint64_t sOrderId, uint64_t bId, uint64_t sId, std::string tick, double p, uint32_t q, uint64_t ts);

    uint64_t getTradeId() const;
    uint64_t getBuyerOrderId() const;
    uint64_t getSellerOrderId() const;
    uint64_t getBuyerId() const;
    uint64_t getSellerId() const;
    std::string getTicker() const;
    double getPrice() const;
    uint32_t getQuantity() const;
    uint64_t getTimestamp() const;
};
