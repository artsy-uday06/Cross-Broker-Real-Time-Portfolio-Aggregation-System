#include <iostream>
#include "Holding.h"
using namespace std;

// constructor of Holding
Holding::Holding(const char b[], const char t[]) {
    copyText(broker, b, TEXT_LEN);
    copyText(ticker, t, TEXT_LEN);
    quantity = 0;
    invested = 0;
    currentPrice = 0;
}

// updates quantity and invested money for a buy or sell
void Holding::applyTrade(Trade t) {
    if (t.isBuy()) {
        quantity = quantity + t.quantity;
        invested = invested + t.amount();
    } else {
        int sold = -t.quantity;
        if (sold > quantity) sold = quantity;
        if (quantity > 0) {
            double avgCost = invested / quantity;
            invested = invested - avgCost * sold;
        }
        quantity = quantity - sold;
        if (quantity == 0) invested = 0;
    }
}

// current value = quantity * current price
double Holding::currentValue() {
    return quantity * currentPrice;
}

// profit or loss = current value - invested
double Holding::profitLoss() {
    return currentValue() - invested;
}

