#ifndef HOLDING_H
#define HOLDING_H
#include "TextUtil.h"
#include "Trade.h"

class Holding {
public:
    char broker[TEXT_LEN];
    char ticker[TEXT_LEN];
    int quantity;
    double invested;
    double currentPrice;

    Holding(const char b[], const char t[]);
    void applyTrade(Trade t);
    double currentValue();
    double profitLoss();
};
#endif
