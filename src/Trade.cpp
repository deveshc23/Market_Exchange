#include "Trade.h"

Trade::Trade(uint64_t tId, uint64_t bOrderId, uint64_t sOrderId, uint64_t bId, uint64_t sId, std::string tick, double p, uint32_t q, uint64_t ts)
    : tradeId(tId), buyerOrderId(bOrderId), sellerOrderId(sOrderId), buyerId(bId), sellerId(sId), ticker(tick), price(p), quantity(q), timestamp(ts) {}

uint64_t Trade::getTradeId() const { return tradeId; }
uint64_t Trade::getBuyerOrderId() const { return buyerOrderId; }
uint64_t Trade::getSellerOrderId() const { return sellerOrderId; }
uint64_t Trade::getBuyerId() const { return buyerId; }
uint64_t Trade::getSellerId() const { return sellerId; }
std::string Trade::getTicker() const { return ticker; }
double Trade::getPrice() const { return price; }
uint32_t Trade::getQuantity() const { return quantity; }
uint64_t Trade::getTimestamp() const { return timestamp; }
