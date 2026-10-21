#ifndef HOLDING_H
#define HOLDING_H
#include <string>
#include "Trade.h"

// One stock held at one broker.
class Holding {
private:
    std::string broker_;
    std::string ticker_;
    int         quantity_;
    double      invested_;      // cost basis of shares still held
    double      currentPrice_;

public:
    Holding(const std::string& broker, const std::string& ticker);

    void applyTrade(const Trade& t);       // updates quantity and invested
    void setCurrentPrice(double price) { currentPrice_ = price; }

    const std::string& broker() const { return broker_; }
    const std::string& ticker() const { return ticker_; }
    int    quantity() const { return quantity_; }
    double invested() const { return invested_; }
    double currentValue() const { return quantity_ * currentPrice_; }
    double profitLoss() const { return currentValue() - invested_; }
};
#endif
