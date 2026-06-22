# High-Frequency Trading (HFT) Exchange Engine

A high-performance simulated trading exchange engine built in C++17. This project demonstrates core concepts of quantitative finance, concurrent programming, and order matching systems by simulating a real-time, multi-threaded market environment.

## 🚀 Overview

The **Market Exchange** engine processes incoming buy and sell orders, matches them based on price-time priority, manages a simulated portfolio for an algorithmic trading bot, and safely logs executed trades to a database—all asynchronously. 

The architecture leverages a **4-thread Producer-Consumer model** using thread-safe queues to ensure the central matching engine is never blocked by slower operations like database I/O or client order generation.

## 🏗️ Architecture & Core Components

### 1. Multi-Threaded Design
The system runs on four independent threads communicating via `ThreadSafeQueue`s to avoid race conditions and ensure thread safety:
- **Main Thread (Exchange Engine)**: Pops orders from the central `exchangeQueue`, processes them through the `OrderBook`, calculates portfolio risk via `PortfolioManager`, and pushes executed trades to the `databaseQueue`.
- **Market Data Publisher Thread (Producer)**: Acts as the broader market, injecting random "noise" orders into the `exchangeQueue` to simulate market liquidity and activity.
- **Trading Bot Thread (Producer)**: Represents an automated algorithmic trader (Client ID: 999). It analyzes market conditions and pushes algorithmic orders to the `exchangeQueue`.
- **Database Logger Thread (Consumer)**: A background thread that listens to the `databaseQueue` and writes executed trades to an SQLite database (`exchange.db`). This ensures database latency does not slow down the critical path of the matching engine.

### 2. The Order Book (`OrderBook.h` & `OrderBook.cpp`)
The heart of the exchange. It matches incoming orders against resting orders using **Price-Time Priority**.
- **Bids (Buy Orders)**: Stored in a `std::map` sorted in descending order (highest price first).
- **Asks (Sell Orders)**: Stored in a `std::map` sorted in ascending order (lowest price first).
- **FIFO Queues**: Orders at the same price level are stored in a `std::queue` to guarantee that the earliest arriving orders are executed first.

### 3. Portfolio Manager (`PortfolioManager.h` & `PortfolioManager.cpp`)
Tracks the real-time risk and Profit & Loss (PnL) of the algorithmic trading bot.
- Calculates **Mark-to-Market (MTM) PnL** based on executed trades and the current market price.
- Tracks positions (long/short inventory) and updates risk metrics asynchronously without blocking the matching engine.

### 4. Database Logger (`DatabaseLogger.h` & `DatabaseLogger.cpp`)
Integrates with SQLite3 (`lib/sqlite3.c`) to maintain a persistent ledger of all executed trades. It operates completely decoupled from the main matching logic.

## ⚙️ Build Instructions

This project uses **CMake** as its build system and requires a compiler that supports C++17.

### Prerequisites
- CMake 3.10 or higher
- C++17 compatible compiler (GCC, Clang, or MSVC)

### Building the Project
```bash
# Clone the repository
git clone https://github.com/deveshc23/Market_Exchange.git
cd Market_Exchange

# Create a build directory
mkdir build
cd build

# Generate build files and compile
cmake ..
cmake --build .
```

### Running the Engine
Once built, execute the compiled binary:
```bash
# On Linux/macOS
./MarketExchange

# On Windows (if using MinGW/MSVC)
MarketExchange.exe
# or
Debug\MarketExchange.exe
```

## 📊 Simulation Output

When you run the simulation, you will observe the engine booting up its background threads and processing sample orders. The console will display:
1. Incoming orders from the noise publisher and the trading bot.
2. Trade execution notifications when a bid and ask match.
3. Periodic portfolio summaries showing the trading bot's inventory and Mark-to-Market PnL.
4. A safe shutdown sequence that terminates all threads cleanly.
