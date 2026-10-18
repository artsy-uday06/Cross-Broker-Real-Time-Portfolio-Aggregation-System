// Portfolio.cpp  -  Owner: Uday Raj Singh
#include "Portfolio.h"
#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>

Portfolio::Portfolio()
    : trades_(new Trade*[16]),     tradeCount_(0),   tradeCap_(16),
      holdings_(new Holding*[16]), holdingCount_(0), holdingCap_(16) {}

Portfolio::~Portfolio() {
    for (int i = 0; i < tradeCount_; ++i)   delete trades_[i];
    for (int i = 0; i < holdingCount_; ++i) delete holdings_[i];
    delete[] trades_;
    delete[] holdings_;
}

void Portfolio::growTrades() {
    int newCap = tradeCap_ * 2;
    Trade** bigger = new Trade*[newCap];
    for (int i = 0; i < tradeCount_; ++i) bigger[i] = trades_[i];
    delete[] trades_;
    trades_ = bigger;
    tradeCap_ = newCap;
}

void Portfolio::growHoldings() {
    int newCap = holdingCap_ * 2;
    Holding** bigger = new Holding*[newCap];
    for (int i = 0; i < holdingCount_; ++i) bigger[i] = holdings_[i];
    delete[] holdings_;
    holdings_ = bigger;
    holdingCap_ = newCap;
}

Holding* Portfolio::findHolding(const std::string& broker, const std::string& ticker) const {
    for (int i = 0; i < holdingCount_; ++i)
        if (holdings_[i]->broker() == broker && holdings_[i]->ticker() == ticker)
            return holdings_[i];
    return nullptr;
}

void Portfolio::addTrade(Trade* t) {
    if (!t) return;
    if (tradeCount_ == tradeCap_) growTrades();
    trades_[tradeCount_++] = t;

    Holding* h = findHolding(t->broker(), t->ticker());
    if (!h) {
        if (holdingCount_ == holdingCap_) growHoldings();
        h = new Holding(t->broker(), t->ticker());
        holdings_[holdingCount_++] = h;
    }
    h->applyTrade(*t);
}

void Portfolio::updatePrice(const std::string& ticker, double price) {
    for (int i = 0; i < holdingCount_; ++i)
        if (holdings_[i]->ticker() == ticker) holdings_[i]->setCurrentPrice(price);
}

double Portfolio::brokerInvested(const std::string& broker) const {
    double sum = 0;
    for (int i = 0; i < holdingCount_; ++i)
        if (holdings_[i]->broker() == broker) sum += holdings_[i]->invested();
    return sum;
}

double Portfolio::brokerValue(const std::string& broker) const {
    double sum = 0;
    for (int i = 0; i < holdingCount_; ++i)
        if (holdings_[i]->broker() == broker) sum += holdings_[i]->currentValue();
    return sum;
}

double Portfolio::totalInvested() const {
    double sum = 0;
    for (int i = 0; i < holdingCount_; ++i) sum += holdings_[i]->invested();
    return sum;
}

double Portfolio::totalValue() const {
    double sum = 0;
    for (int i = 0; i < holdingCount_; ++i) sum += holdings_[i]->currentValue();
    return sum;
}

void Portfolio::printHoldings() const {
    std::cout << "\n=== HOLDINGS (per broker) ===\n"
              << std::left << std::setw(10) << "Broker" << std::setw(11) << "Stock"
              << std::right << std::setw(6) << "Qty" << std::setw(14) << "Invested"
              << std::setw(14) << "Current" << std::setw(14) << "P/L" << "\n";
    std::cout << std::fixed << std::setprecision(2);
    for (int i = 0; i < holdingCount_; ++i) {
        const Holding* h = holdings_[i];
        if (h->quantity() == 0) continue;
        std::cout << std::left << std::setw(10) << h->broker() << std::setw(11) << h->ticker()
                  << std::right << std::setw(6) << h->quantity()
                  << std::setw(14) << h->invested()
                  << std::setw(14) << h->currentValue()
                  << std::setw(14) << h->profitLoss() << "\n";
    }
}

void Portfolio::printBrokerSummary() const {
    std::vector<std::string> brokers;
    for (int i = 0; i < holdingCount_; ++i)
        if (std::find(brokers.begin(), brokers.end(), holdings_[i]->broker()) == brokers.end())
            brokers.push_back(holdings_[i]->broker());

    std::cout << "\n=== BROKER SUMMARY ===\n" << std::fixed << std::setprecision(2);
    for (size_t i = 0; i < brokers.size(); ++i) {
        double inv = brokerInvested(brokers[i]), val = brokerValue(brokers[i]);
        std::cout << std::left << std::setw(10) << brokers[i] << std::right
                  << " invested " << std::setw(12) << inv
                  << "  value " << std::setw(12) << val
                  << "  P/L " << std::setw(12) << (val - inv) << "\n";
    }
    std::cout << std::left << std::setw(10) << "TOTAL" << std::right
              << " invested " << std::setw(12) << totalInvested()
              << "  value " << std::setw(12) << totalValue()
              << "  P/L " << std::setw(12) << (totalValue() - totalInvested()) << "\n";
}

void Portfolio::printTradeHistory() const {
    std::cout << "\n=== TRADE HISTORY (" << tradeCount_ << " trades) ===\n";
    for (int i = 0; i < tradeCount_; ++i) trades_[i]->print();
}
