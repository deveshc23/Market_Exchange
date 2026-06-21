#include "PortfolioManager.h"
#include <cmath>

PortfolioManager::PortfolioManager(uint64_t clientId) : trackedClientId(clientId) {}

void PortfolioManager::onTradeExecuted(const Trade& trade) {
    std::lock_guard<std::mutex> lock(mtx);
    
    int32_t qtyChange = 0;
    double tradePrice = trade.getPrice();
    
    if (trade.getBuyerId() == trackedClientId) {
        qtyChange = trade.getQuantity();
    } else if (trade.getSellerId() == trackedClientId) {
        qtyChange = -(int32_t)trade.getQuantity();
    }

    if (qtyChange == 0) return; // Not our trade

    std::string ticker = trade.getTicker();
    
    int32_t oldQty = inventory[ticker];
    double oldAvg = averagePrice[ticker];

    int32_t newQty = oldQty + qtyChange;
    
    if (newQty == 0) {
        averagePrice[ticker] = 0.0;
    } else if (oldQty == 0) {
        averagePrice[ticker] = tradePrice;
    } else if ((oldQty > 0 && qtyChange > 0) || (oldQty < 0 && qtyChange < 0)) {
        double oldCost = std::abs(oldQty) * oldAvg;
        double newCost = std::abs(qtyChange) * tradePrice;
        averagePrice[ticker] = (oldCost + newCost) / std::abs(newQty);
    }

    inventory[ticker] = newQty;
}

void PortfolioManager::printPortfolio(double currentMarketPrice) {
    std::lock_guard<std::mutex> lock(mtx);
    std::cout << "\n\033[1;36m========== PORTFOLIO MANAGER (Client " << trackedClientId << ") ==========\033[0m\n";
    if (inventory.empty()) {
        std::cout << "No positions held.\n";
    }
    for (const auto& pair : inventory) {
        std::string ticker = pair.first;
        int32_t qty = pair.second;
        double avgPx = averagePrice[ticker];
        
        double pnl = 0.0;
        if (qty > 0) {
            pnl = (currentMarketPrice - avgPx) * qty;
        } else if (qty < 0) {
            pnl = (avgPx - currentMarketPrice) * std::abs(qty);
        }
        
        std::cout << "Ticker: " << ticker << " | Qty: " << qty 
                  << " | Avg Px: $" << std::fixed << std::setprecision(2) << avgPx 
                  << " | Unrealized PnL: $" << pnl << "\n";
    }
    std::cout << "\033[1;36m====================================================\033[0m\n\n";
}
