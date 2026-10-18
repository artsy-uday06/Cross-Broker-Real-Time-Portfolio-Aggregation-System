#ifndef PORTFOLIO_H
#define PORTFOLIO_H
#include <string>
#include "Trade.h"
#include "Holding.h"

class Portfolio {
private:
    Trade**   trades_;   int tradeCount_;   int tradeCap_;
    Holding** holdings_; int holdingCount_; int holdingCap_;

    Holding* findHolding(const std::string& broker, const std::string& ticker) const;
    void growTrades();
    void growHoldings();

    Portfolio(const Portfolio&);              // no copying (owns raw pointers)
    Portfolio& operator=(const Portfolio&);

public:
    Portfolio();
    ~Portfolio();                              // deletes every Trade and Holding

    void addTrade(Trade* t);                   // takes ownership of t
    void updatePrice(const std::string& ticker, double price);

    int    tradeCount() const { return tradeCount_; }
    double brokerInvested(const std::string& broker) const;
    double brokerValue(const std::string& broker) const;
    double totalInvested() const;
    double totalValue() const;

    void printHoldings() const;
    void printBrokerSummary() const;
    void printTradeHistory() const;
};
#endif
