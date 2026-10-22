#include "Holding.h"

Holding::Holding(const std::string& broker, const std::string& ticker)
    : broker_(broker), ticker_(ticker), quantity_(0),
      invested_(0.0), currentPrice_(0.0) {}

void Holding::applyTrade(const Trade& t) {
    if (t.isBuy()) {
        quantity_ += t.quantity();
        invested_ += t.amount();
    } else {
        int sold = -t.quantity();
        if (sold > quantity_) sold = quantity_;         
        if (quantity_ > 0) {
            double avgCost = invested_ / quantity_;       
            invested_ -= avgCost * sold;
        }
        quantity_ -= sold;
        if (quantity_ == 0) invested_ = 0.0;
    }
}
