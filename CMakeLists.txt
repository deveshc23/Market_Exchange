cmake_minimum_required(VERSION 3.10)
project(MarketExchange CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

add_executable(MarketExchange 
    src/main.cpp
    src/Order.cpp
    src/Trade.cpp
    src/OrderBook.cpp
    src/MarketDataPublisher.cpp
    src/PortfolioManager.cpp
    src/TradingBot.cpp
    src/DatabaseLogger.cpp
    lib/sqlite3.c
)
