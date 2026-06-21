#include "DatabaseLogger.h"
#include <iostream>
#include <chrono>

DatabaseLogger::DatabaseLogger(ThreadSafeQueue<Trade>& queue)
    : tradeQueue(queue), isRunning(false), db(nullptr) {
    initDatabase();
}

DatabaseLogger::~DatabaseLogger() {
    stop();
    if (db) {
        sqlite3_close(db);
    }
}

void DatabaseLogger::initDatabase() {
    int rc = sqlite3_open("exchange.db", &db);
    if (rc) {
        std::cerr << "Can't open database: " << sqlite3_errmsg(db) << "\n";
        return;
    }
    sqlite3_exec(db, "DROP TABLE IF EXISTS Trades;", 0, 0, 0);

    const char* sql = "CREATE TABLE IF NOT EXISTS Trades ("
                      "ID INT PRIMARY KEY NOT NULL,"
                      "BUYER_ID INT NOT NULL,"
                      "SELLER_ID INT NOT NULL,"
                      "TICKER TEXT NOT NULL,"
                      "PRICE REAL NOT NULL,"
                      "QUANTITY INT NOT NULL,"
                      "TIMESTAMP INT NOT NULL);";

    char* errMsg = 0;
    rc = sqlite3_exec(db, sql, 0, 0, &errMsg);
    if (rc != SQLITE_OK) {
        std::cerr << "SQL error: " << errMsg << "\n";
        sqlite3_free(errMsg);
    }
}

void DatabaseLogger::start() {
    isRunning = true;
    loggerThread = std::thread(&DatabaseLogger::run, this);
}

void DatabaseLogger::stop() {
    isRunning = false;
    // Push a dummy trade to wake up the thread if it's sleeping on an empty queue
    tradeQueue.push(Trade(0,0,0,0,0,"STOP",0,0,0));
    if (loggerThread.joinable()) loggerThread.join();
}

void DatabaseLogger::run() {
    while (isRunning) {
        Trade t = tradeQueue.pop();
        if (!isRunning && t.getTicker() == "STOP") break;

        // DBMS Concept: ACID Properties (Transactions)
        sqlite3_exec(db, "BEGIN TRANSACTION;", NULL, NULL, NULL);

        std::string sql = "INSERT INTO Trades (ID, BUYER_ID, SELLER_ID, TICKER, PRICE, QUANTITY, TIMESTAMP) VALUES ("
                        + std::to_string(t.getTradeId()) + ", "
                        + std::to_string(t.getBuyerId()) + ", "
                        + std::to_string(t.getSellerId()) + ", "
                        + "'" + t.getTicker() + "', "
                        + std::to_string(t.getPrice()) + ", "
                        + std::to_string(t.getQuantity()) + ", "
                        + std::to_string(t.getTimestamp()) + ");";

        char* errMsg = 0;
        int rc = sqlite3_exec(db, sql.c_str(), 0, 0, &errMsg);
        if (rc != SQLITE_OK) {
            std::cerr << "SQL insert error: " << errMsg << "\n";
            sqlite3_free(errMsg);
        }

        sqlite3_exec(db, "COMMIT;", NULL, NULL, NULL);
    }
}
