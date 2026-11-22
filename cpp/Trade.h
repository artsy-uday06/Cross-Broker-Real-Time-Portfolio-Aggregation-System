#ifndef TRADE_H
#define TRADE_H
#include "TextUtil.h"

class Trade {
public:
    char broker[TEXT_LEN];
    char ticker[TEXT_LEN];
    int quantity;
    double price;
    char date[DATE_LEN];

    Trade();
    Trade(const char b[], const char t[], int q, double p, const char d[]);
    bool isBuy();
    double amount();
};
#endif
