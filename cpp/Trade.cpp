
#include "Trade.h"
#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <cmath>

Trade::Trade(const std::string& broker, const std::string& ticker,
             int quantity, double price, const std::string& date)
    : broker_(broker), ticker_(ticker), quantity_(quantity),
      price_(price), date_(date) {}

double Trade::amount() const { return std::abs(quantity_) * price_; }

void Trade::print() const {
    std::cout << std::left << std::setw(12) << date_
              << std::setw(10) << broker_
              << std::setw(11) << ticker_
              << std::setw(6)  << (isBuy() ? "BUY" : "SELL")
              << std::right << std::setw(6) << std::abs(quantity_)
              << std::setw(12) << std::fixed << std::setprecision(2) << price_
              << "\n";
}
