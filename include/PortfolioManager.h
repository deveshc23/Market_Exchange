#pragma once

#include "Trade.h"
#include <map>
#include <string>
#include <mutex>
#include <iostream>
#include <iomanip>

class PortfolioManager {
private:
    uint64_t trackedClientId;
    std::map<std::string, int32_t> inventory;
    std::map<std::string, double> averagePrice;
    std::mutex mtx;

public:
    PortfolioManager(uint64_t clientId);

    void onTradeExecuted(const Trade& trade);
    void printPortfolio(double currentMarketPrice);
};
